#include "RapidEvent.h"

#include <fstream>

#include "TFile.h"
#include "TTree.h"
#include "TString.h"

#include "RapidConfig.h"
#include "RapidNorm.h"
#include "RapidTrack.h"
#include "RapidSelect.h"
#include "RapidPV.h"

using namespace std;

//______________________________________________________________________________
RapidEvent::RapidEvent()
{
    config_ = nullptr;
    norm_   = nullptr;
    select_ = nullptr;
    pv_     = nullptr;

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

    pv_ = new RapidPV(config_);

    event_number_ = event_number;
    n_tracks_     = 0;
}

//______________________________________________________________________________
RapidEvent::~RapidEvent()
{
    while(!tracks_.empty()) {
		delete tracks_[tracks_.size()-1];
		tracks_.pop_back();
	}

    delete pv_;
}

//______________________________________________________________________________
Ssiz_t RapidEvent::GetEventNumber()
{
    return event_number_;
}

//______________________________________________________________________________
Ssiz_t RapidEvent::GetNumberOfTracks()
{
    return n_tracks_;
}

//______________________________________________________________________________
vector<RapidTrack*> RapidEvent::GetTracks()
{
    return tracks_;
}

//______________________________________________________________________________
int RapidEvent::BuildEvent()
{
    vector<TString> particles = config_->GetParticles();

    for(auto particle: particles) {

        Int_t n_particles = norm_->GetPoisson(particle);
        n_tracks_ += n_particles;

        auto tracks_to_add = select_->SelectTracks(particle, n_particles,
                                                        event_number_);

        // Append tracks_to_add at the end of tracks_
        tracks_.insert(end(tracks_), begin(tracks_to_add), end(tracks_to_add));
    }

    // Setup primary vertex of the event.
    pv_->SetNTracks(n_tracks_);
    pv_->SetXYZ(0., 0., 0.);

    return 0;
}
