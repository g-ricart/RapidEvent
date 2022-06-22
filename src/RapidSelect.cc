#include "RapidSelect.h"

#include <vector>
#include <map>

#include "TFile.h"
#include "TString.h"
#include "TTree.h"
#include "TBranch.h"
#include "TString.h"
#include "TRandomGen.h"
#include "TObjArray.h"
#include "TObjString.h"

#include "RapidConfig.h"
#include "RapidTrack.h"
#include "RapidPV.h"

using namespace std;

//______________________________________________________________________________
RapidSelect::RapidSelect()
{
    config_ = nullptr;
    random_ = nullptr;
    params_to_keep_.clear();
}

//______________________________________________________________________________
RapidSelect::RapidSelect(RapidConfig* config)
{
    config_ = config;
    random_ = new TRandomMT64();
    random_->SetSeed(0);
    params_to_keep_ = config_->GetParams();
}

//______________________________________________________________________________
RapidSelect::~RapidSelect()
{
    delete random_;
}

//______________________________________________________________________________
vector<RapidTrack*> RapidSelect::SelectPromptTracks(TString  part_name,
                                                    Ssiz_t   n_tracks,
                                                    Ssiz_t   event_number,
                                                    RapidPV* pv,
                                                    Size_t   first_ID)
{
    vector<RapidTrack*> selected_tracks;

    // Open data file
    TFile* data_file = TFile::Open(config_->GetDataFile(part_name), "READ");
    TTree* data_tree = (TTree*)data_file->Get("DecayTree");
    data_tree->SetDirectory(0);

    TObjArray* branch_array = data_tree->GetListOfBranches();
    branch_array->SetOwner(kTRUE); // Set the TObjArray as owner of its
                                   // content, allowing proper delete

    Ssiz_t n_entries = data_tree->GetEntriesFast();

    for (Ssiz_t i = 0; i < n_tracks; i++) {

        // Setup the track
        RapidTrack* track = new RapidTrack();
        TString track_name = part_name + Form("_%d", i);
        track->SetName(track_name);
        track->SetEventNumber(event_number);
        track->SetPrompt(true);
        track->SetID(first_ID + i);

        // Select the track
        SelectPromptTrack(track, data_tree, branch_array, n_entries);
        selected_tracks.push_back(track);

        // Change origin vertex.
        track->SetOriginVertex(pv);

        // If perfect PID is requested, set it.
        if (config_->IsPIDPerfect()) {track->SetPID();}

    }

    // cleanup
    branch_array->Delete();
    delete data_tree;
    data_file->Close("R");
    delete data_file;

    return selected_tracks;
}

//______________________________________________________________________________
vector<RapidTrack*> RapidSelect::SelectDecays(TString         mother,
                                              vector<TString> daughters,
                                              Ssiz_t          n_decays,
                                              Ssiz_t          event_number,
                                              RapidPV*        pv,
                                              Size_t          first_ID)
{
    vector<RapidTrack*> selected_tracks;

    // Open data file.
    TFile* data_file = TFile::Open(config_->GetDataFile(mother), "READ");
    TTree* data_tree = (TTree*)data_file->Get("DecayTree");
    data_tree->SetDirectory(0);

    TObjArray* branch_array = data_tree->GetListOfBranches();
    branch_array->SetOwner(kTRUE); // Set the TObjArray as owner of its
                                   // content, allowing proper delete

    Ssiz_t n_entries = data_tree->GetEntriesFast();

    Size_t n_daughters = daughters.size();

    for (Ssiz_t i = 0; i < n_decays; i++) {
        // Setup mother track.
        RapidTrack* mother_track = new RapidTrack();
        TString mother_track_name = mother + Form("_%d", int(i));
        mother_track->SetName(mother_track_name);
        mother_track->SetEventNumber(event_number);
        mother_track->SetPrompt(true);

        // Set mother ID.
        Size_t mother_ID = first_ID + i*(n_daughters+1);
        mother_track->SetID(mother_ID);

        Ssiz_t entry_index = SelectMotherTrack(mother_track, mother, data_tree,
                                                       branch_array, n_entries);

        // Change origin vertex to event PV.
        mother_track->SetOriginVertex(pv);

        // If perfect PID is requested, set it.
        if (config_->IsPIDPerfect()) {mother_track->SetPID();}

        selected_tracks.push_back(mother_track);

        for (auto part: daughters) {
            Size_t j = 0; // Loop counter.

            // Setup daughter track.
            RapidTrack* daughter_track = new RapidTrack();
            TString daughter_track_name = part + Form("_%d", int(i));
            daughter_track->SetName(daughter_track_name);
            daughter_track->SetEventNumber(event_number);
            daughter_track->SetPrompt(false);

            // Set daughter track IDs.
            Size_t daughter_ID = mother_ID + j+1;
            daughter_track->SetID(daughter_ID);
            daughter_track->SetMotherID(mother_ID);

            SelectDaughterTrack(daughter_track, part, data_tree, branch_array,
                                                                   entry_index);

            // If perfect PID is requested, set it.
            if (config_->IsPIDPerfect()) {daughter_track->SetPID();}

            selected_tracks.push_back(daughter_track);

            j++;
        }
    }

    // cleanup
    branch_array->Delete();
    delete data_tree;
    data_file->Close("R");
    delete data_file;

    return selected_tracks;
}

//______________________________________________________________________________
Int_t RapidSelect::SelectPromptTrack(RapidTrack* track, TTree*     tree,
                                                        TObjArray* branches,
                                                        Ssiz_t     n_entries)
{
    tree->SetBranchStatus("nEvent", 0);
    const Int_t kNBranches = branches->GetEntriesFast();
    // Map to store the branch values. Keys are branch names.
    map<TString, Double_t> var_map;

    for (Int_t i = 1; i < kNBranches; i++) { // Starting from 1 to avoid nEvent.
        TObject* b = (TObject*)branches->At(i);
        TString branch_name = b->GetName();
        var_map[branch_name] = 0.;
    }

    // Separate loop to ensure pointer stability.
    for (auto &it: var_map) {
        tree->SetBranchAddress(it.first,     // The branch name.
                              &(it.second)); // The address of the value.
    }

    // Get a random entry of the tree.
    Ssiz_t entry_index = random_->Integer(n_entries);
    tree->GetEntry(entry_index);

    for (auto &it: var_map) {
        TString  branch_name = it.first;
        Double_t value       = it.second;

        TObjArray* tokens = branch_name.Tokenize("_");

        switch (tokens->GetEntriesFast()) {
            case 3:
            {
                SetTrackParams(track, tokens, value);
                break;
            }
            case 4:
            {
                SetTrackParamsTrue(track, tokens, value);
                break;
            }
            default:
            {
                cout << "ERROR in RapidSelect::SelectTrack : ill-formed branch "
                     << "name " << branch_name << endl;
                return 1;
            }
        }
        delete tokens;
    }

    return 0;
}

//______________________________________________________________________________
Int_t RapidSelect::SelectMotherTrack(RapidTrack* track, TString part_name,
                                                        TTree* tree,
                                                        TObjArray* branches,
                                                        Ssiz_t n_entries)
{
    tree->SetBranchStatus("nEvent", 0);
    const Int_t kNBranches = branches->GetEntriesFast();
    // Map to store the branch values. Keys are branch names.
    map<TString, Double_t> var_map;

    for (Int_t i = 1; i < kNBranches; i++) { // Starting from 1 to avoid nEvent.
        TObject* b = (TObject*)branches->At(i);
        TString branch_name = b->GetName();
        tree->SetBranchStatus(branch_name, 0);

        if (branch_name.Contains(part_name)) { // Only store branches
                                               // containing the mother name.
            var_map[branch_name] = 0.;
            tree->SetBranchStatus(branch_name, 1);
        }
    }

    // Separate loop to ensure pointer stability.
    for (auto &it: var_map) {
        tree->SetBranchAddress(it.first,     // The branch name.
                              &(it.second)); // The address of the value.
    }

    // Get a random entry of the tree.
    Ssiz_t entry_index = random_->Integer(n_entries);
    tree->GetEntry(entry_index);

    for (auto &it: var_map) {
        TString  branch_name = it.first;
        Double_t value       = it.second;

        TObjArray* tokens = branch_name.Tokenize("_");

        switch (tokens->GetEntriesFast()) {
            case 3:
            {
                SetTrackParams(track, tokens, value);
                break;
            }
            case 4:
            {
                SetTrackParamsTrue(track, tokens, value);
                break;
            }
            default:
            {
                cout << "ERROR in RapidSelect::SelectTrack : ill-formed branch "
                     << "name " << branch_name << endl;
                return 1;
            }
        }
        delete tokens;
    }
    return entry_index;
}

//______________________________________________________________________________
Int_t RapidSelect::SelectDaughterTrack(RapidTrack* track, TString part_name,
                                                          TTree* tree,
                                                          TObjArray* branches,
                                                          Ssiz_t entry_index)
{
    tree->SetBranchStatus("nEvent", 0);
    const Int_t kNBranches = branches->GetEntriesFast();
    // Map to store the branch values. Keys are branch names.
    map<TString, Double_t> var_map;

    for (Int_t i = 1; i < kNBranches; i++) { // Starting from 1 to avoid nEvent.
        TObject* b = (TObject*)branches->At(i);
        TString branch_name = b->GetName();
        tree->SetBranchStatus(branch_name, 0);

        if (branch_name.Contains(part_name)) { // Only store branches
                                               // containing the mother name.
            var_map[branch_name] = 0.;
            tree->SetBranchStatus(branch_name, 1);
        }
    }

    // Separate loop to ensure pointer stability.
    for (auto &it: var_map) {
        tree->SetBranchAddress(it.first,     // The branch name.
                              &(it.second)); // The address of the value.
    }

    // Get the same entry as the mother.
    tree->GetEntry(entry_index);

    for (auto &it: var_map) {
        TString  branch_name = it.first;
        Double_t value       = it.second;

        TObjArray* tokens = branch_name.Tokenize("_");

        switch (tokens->GetEntriesFast()) {
            case 3:
            {
                SetTrackParams(track, tokens, value);
                break;
            }
            case 4:
            {
                SetTrackParamsTrue(track, tokens, value);
                break;
            }
            default:
            {
                cout << "ERROR in RapidSelect::SelectTrack : ill-formed branch "
                     << "name " << branch_name << endl;
                return 1;
            }
        }
        delete tokens;
    }
    return 0;
}

//______________________________________________________________________________
Int_t RapidSelect::SetTrackParams(RapidTrack* track, TObjArray* tokens,
                                                     Double_t   value)
{
    TString param = ((TObjString*)tokens->At(2))->GetString();

    if (KeepParam(param)) { track->SetParam(param, value); }

    return 0;
}

//______________________________________________________________________________
Int_t RapidSelect::SetTrackParamsTrue(RapidTrack* track, TObjArray* tokens,
                                                         Double_t   value)
{
    TString param = ((TObjString*)tokens->At(2))->GetString();

    if (KeepParam(param)) { track->SetParam(param + "_TRUE", value); }

    return 0;
}

//______________________________________________________________________________
Bool_t RapidSelect::KeepParam(TString param)
{
    return (find(params_to_keep_.begin(), params_to_keep_.end(), param)
           != params_to_keep_.end());
}
