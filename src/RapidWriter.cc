#include "RapidWriter.h"

#include <vector>
#include <map>

#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TString.h"

#include "RapidEvent.h"
#include "RapidConfig.h"
#include "RapidTrack.h"

using namespace std;

//______________________________________________________________________________
RapidWriter::RapidWriter()
{
    out_tree_ = nullptr;
    out_file_ = nullptr;
}

//______________________________________________________________________________
RapidWriter::RapidWriter(TString out_file_path, RapidConfig* config)
{
    config_ = config;
    out_file_ = nullptr;

    SetupTree(out_file_path);
}

//______________________________________________________________________________
RapidWriter::~RapidWriter()
{
    out_tree_->AutoSave();
    out_file_->Close("nodelete");
    delete out_tree_;
    delete out_file_;
}

//______________________________________________________________________________
Int_t RapidWriter::SaveEvent(RapidEvent* event)
{
    vector<RapidTrack*> tracks = event->GetTracks();

    // Event parameters.
    event_number_ = event->GetEventNumber();
    n_tracks_     = event->GetNumberOfTracks();

    for (auto track: tracks) {

        // Track parameters.
        name_      = track->GetName();
        track_ID_  = track->GetID();
        mother_ID_ = track->GetMotherID();
        isPrompt_  = track->IsPrompt();
                for (auto &it: params_map_) {
            it.second = track->GetParam(it.first);
        }
        out_tree_->Fill();
    }

    out_tree_->AutoSave();

    return 0;
}

//______________________________________________________________________________
Int_t RapidWriter::SetupTree(TString out_file_path)
{
    out_file_ = new TFile(out_file_path, "RECREATE");
    out_tree_ = new TTree("trackTree", "trackTree");
    out_tree_->SetDirectory(out_file_);

    out_tree_->Branch("eventNumber", &event_number_);
    out_tree_->Branch("nTracks",     &n_tracks_);
    out_tree_->Branch("partName",    &name_);
    out_tree_->Branch("trackID",     &track_ID_);
    out_tree_->Branch("motherID",    &mother_ID_);
    out_tree_->Branch("isPrompt",    &isPrompt_);

    vector<TString> params_list = config_->GetParams();

    for (auto param: config_->GetParams()) {
        params_map_[param] = -999.;
        if (!param.Contains("ProbNN")) { // Add true value except for ProbNN.
            params_map_[param + "_TRUE"] = -999.;
        }
    }

    // Separate loop to ensure pointer stability.
    for (auto &it: params_map_) {
        out_tree_->Branch(it.first, &(it.second));
    }

    return 0;
}
