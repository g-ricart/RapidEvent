#include "RapidSelect.h"

#include <vector>

#include "TFile.h"
#include "TString.h"
#include "TTree.h"
#include "TBranch.h"
#include "TString.h"
#include "TRandomGen.h"
#include "TObjArray.h"

#include "RapidConfig.h"
#include "RapidTrack.h"

using namespace std;

//______________________________________________________________________________
RapidSelect::RapidSelect()
{
    config_ = nullptr;
}

//______________________________________________________________________________
RapidSelect::RapidSelect(RapidConfig* config)
{
    config_ = config;
    params_to_keep_ = config_->GetParams();
}

//______________________________________________________________________________
RapidSelect::~RapidSelect()
{
    delete config_;
}

//______________________________________________________________________________
int RapidSelect::SelectTrack(RapidTrack* track, TTree* tree,
                             TObjArray* branches)
{
    const Int_t kNBranches = branches->GetEntries();
    Double_t* branch_addresses[kNBranches]; // Array of pointers to store addresses
                                            // of all the branches
    // TBranch* branch_array[kNBranches];

    for (Int_t i = 0; i < kNBranches; i++) {
        TObject* b = (TObject*)branches->At(i+1); // for some reason the
                                        // removed branch is still in the array
                                        // at the index 0
        auto branch_name = b->GetName();
        tree->SetBranchAddress(branch_name, branch_addresses[i]);
    }

    // Get a random entry of the tree
    TRandomMT64* random = new TRandomMT64();
    Ssiz_t entry_index = random->Integer(tree->GetEntries());
    cout << entry_index << endl;
    tree->GetEntry(entry_index); // segfault here, maybe try ti initialise the pointers

    for(Int_t i = 0; i < branches->GetEntries(); i++) { // loop over branches
        TBranch* branch = (TBranch*)branches->At(i);
        TString branch_name = branch->GetName();
        for(auto param: params_to_keep_) { // loop over parameters
            if (branch_name.Contains(param)) {
                if (branch_name.Contains("TRUE",
                                         TString::ECaseCompare::kIgnoreCase)) {

                }
            }
        }
    }

    return 0;
}

//______________________________________________________________________________
vector<RapidTrack*> RapidSelect::SelectTracks(TString part_name, Int_t n_tracks,
                                              Ssiz_t event_number)
{
    vector<RapidTrack*> selected_tracks;

    // Open data file
    TFile* data_file = TFile::Open(config_->GetDataFile(part_name), "READ");
    TTree* data_tree = (TTree*)data_file->Get("DecayTree");

    // Remove nEvent branch because it is not a Double_t
    // DOTO: Find a better way to check and remove branches that are not Double_t.
    TBranch *b = data_tree->GetBranch("nEvent");
    data_tree->GetListOfBranches()->Remove(b);
    delete b;

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
