#include "RapidWriter.h"

#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TBranch.h"
#include "TString.h"

#include "RapidEvent.h"
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

    SetupTree(out_file_path);
}

//______________________________________________________________________________
RapidWriter::~RapidWriter()
{
    out_tree_->AutoSave();
    out_file_->Close();
    delete out_tree_;
    delete out_file_;
}

//______________________________________________________________________________
Int_t RapidWriter::SaveEvent(RapidEvent* event)
{
    vector<RapidTrack*> tracks = event->GetTracks();

    for (auto track: tracks) {
        for (auto &it: params_map_) {
            it.second = track->GetParam(it.first);
        }
    }

    out_tree_->Fill();
    out_tree_->AutoSave();

    while(!tracks.empty()) {
		delete tracks[tracks.size()-1];
		tracks.pop_back();
	}

    return 0;
}

//______________________________________________________________________________
Int_t RapidWriter::SetupTree(TString out_file_path)
{
    out_file_ = new TFile(out_file_path, "RECREATE");
    out_tree_ = new TTree("trackTree", "trackTree");
    out_tree_->SetDirectory(out_file_);

    out_tree_->Branch("eventNumber", &event_number_);
    out_tree_->Branch("partName",    &name_);

    vector<TString> params_list = config_->GetParams();

    for (auto param: config_->GetParams()) {
        params_map_[param] = -999.;
        params_map_[param + "_TRUE"] = -999.;
    }

    // Separate loop to ensure pointer stability.
    for (auto &it: params_map_) {
        out_tree_->Branch(it.first, &(it.second));
    }

    return 0;
}
