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
                                            RapidSelect* select,
                                            Ssiz_t event_number)
{
    config_ = config;
    norm_   = norm;
    select_ = select;

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

    for(auto particle: particles) {

        Int_t n_particles = norm_->GetPoisson(particle);
        n_tracks_ += n_particles;

        tracks_ = select_->SelectTracks(particle, n_particles, event_number_);
    }

    return 0;
}
