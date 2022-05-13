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

using namespace std;

//______________________________________________________________________________
RapidSelect::RapidSelect()
{
    config_ = nullptr;
    random_ = nullptr;
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
    delete config_;
    delete random_;
}

//______________________________________________________________________________
vector<RapidTrack*> RapidSelect::SelectTracks(TString part_name, Int_t n_tracks,
                                              Ssiz_t event_number)
{
    vector<RapidTrack*> selected_tracks;

    // Open data file
    TFile* data_file = TFile::Open(config_->GetDataFile(part_name), "READ");
    TTree* data_tree = (TTree*)data_file->Get("DecayTree");

    TObjArray* branch_array = data_tree->GetListOfBranches();
    branch_array->SetOwner(kTRUE); // Set the TObjArray as owner of its
                                   // content, allowing proper delete

    for (Int_t i = 0; i < n_tracks; i++) {

        // Setup the track
        RapidTrack* track = new RapidTrack();
        TString track_name = part_name + Form("_%d", i);
        track->SetName(track_name);
        track->SetEventNumber(event_number);

        SelectTrack(track, data_tree, branch_array);
        selected_tracks.push_back(track);

    }

    // cleanup
    branch_array->Delete();
    delete data_tree;
    data_file->Close();
    delete data_file;

    return selected_tracks;
}

//______________________________________________________________________________
Int_t RapidSelect::SelectTrack(RapidTrack* track, TTree* tree,
                             TObjArray* branches)
{
    tree->SetBranchStatus("nEvent", 0);
    const Int_t kNBranches = branches->GetEntries();
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

    // Get a random entry of the tree
    Ssiz_t entry_index = random_->Integer(tree->GetEntries());
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
                                                     Double_t value)
{
    TString param = ((TObjString*)tokens->At(2))->GetString();

    if (KeepParam(param)) { track->SetParam(param, value); }

    return 0;
}

//______________________________________________________________________________
Int_t RapidSelect::SetTrackParamsTrue(RapidTrack* track, TObjArray* tokens,
                                                     Double_t value)
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
