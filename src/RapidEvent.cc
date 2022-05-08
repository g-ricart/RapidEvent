#include "RapidEvent.h"

#include <fstream>

#include "TFile.h"
#include "TTree.h"
#include "TString.h"

#include "RapidConfig.h"
#include "RapidNorm.h"
#include "RapidTrack.h"
#include "RapidSelect.h"

using namespace std;

//______________________________________________________________________________
RapidEvent::RapidEvent()
{
    config_ = nullptr;
    norm_   = nullptr;
    select_ = nullptr;

    event_number_ = 0;
    n_tracks_     = 0;
}

//______________________________________________________________________________
RapidEvent::RapidEvent(RapidConfig* config, RapidNorm* norm,
                       Ssiz_t event_number)
{
    config_ = config;
    norm_   = norm;
    RapidSelect* select_ = new RapidSelect(config_);

    event_number_ = event_number;
    n_tracks_     = 0;
}

//______________________________________________________________________________
RapidEvent::~RapidEvent()
{
    delete config_;
    delete norm_;
    delete select_;

    while(!tracks_.empty()) {
		delete tracks_[tracks_.size()-1];
		tracks_.pop_back();
	}
}

//______________________________________________________________________________
Ssiz_t RapidEvent::GetNumberOfTracks()
{
    return n_tracks_;
}

//______________________________________________________________________________
int RapidEvent::BuildEvent()
{
    vector<TString> particles = config_->GetParticles();

    for(auto part: particles) {

        Int_t n_parts = norm_->GetPoisson(part);
        n_tracks_ += n_parts;

        // Open data file
        TFile* data_file = TFile::Open(config_->GetDataFile(part), "READ");
        TTree* data_tree = (TTree*)data_file->Get("DecayTree");

        TObjArray* branch_array = data_tree->GetListOfBranches();
        branch_array->SetOwner(kTRUE); // Set the TObjArray as owner of its
                                       // content, allowing proper delete

        for (Int_t i = 0; i < n_parts; i++) {

            // Setup the track
            RapidTrack* track = new RapidTrack();
            TString track_name = part + Form("_%d", i);
            track->SetName(track_name);
            track->SetEventNumber(event_number_);

            select_->SelectTrack(track, data_tree);

        }

        // cleanup
        branch_array->Delete();
        delete data_tree;
        data_file->Close();
        delete data_file;
    }

    return 0;
}
