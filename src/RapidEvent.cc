#include "RapidEvent.h"

#include <vector>
#include <map>

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

    tracks_.clear();

    event_number_ = 0;
    n_tracks_     = 0;
    pv_n_tracks_  = 0;
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

    tracks_.clear();

    event_number_ = event_number;
    n_tracks_     = 0;
    pv_n_tracks_  = 0;
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
RapidPV* RapidEvent::GetPV()
{
    return pv_;
}

//______________________________________________________________________________
vector<RapidTrack*> RapidEvent::GetTracks()
{
    return tracks_;
}

//______________________________________________________________________________
int RapidEvent::BuildEvent()
{
    vector<TString> prompt_particles     = config_->GetPrompts();
    map<TString, vector<TString>> decays = config_->GetDecays();

    // First loop to get the number of prompt tracks.
    for(auto particle: prompt_particles) {

        n_part_map_[particle] = norm_->GetPoisson(particle);
        pv_n_tracks_ += n_part_map_.at(particle);
    }

    // Setup primary vertex of the event.
    pv_->SetNTracks(pv_n_tracks_);
    pv_->SetXYZ(0., 0., 0.);

    Size_t first_ID = 1;

    // Second loop to select prompts tracks.
    for (auto particle: prompt_particles) {

        Int_t n_particles = n_part_map_.at(particle);

        vector<RapidTrack*> tracks_to_add = select_->SelectPromptTracks(particle,
                                     n_particles, event_number_, pv_, first_ID);

        // Append tracks_to_add to the end of tracks_
        tracks_.insert(end(tracks_), begin(tracks_to_add), end(tracks_to_add));

        // Setup first ID for next loop.
        first_ID = tracks_.back()->GetID() + 1;
    }

    // Third loop for decays.
    for (auto &it: decays) {

        TString mother            = it.first;
        vector<TString> daughters = it.second;

        Int_t n_decays = norm_->GetPoisson(mother);

        if (n_decays != 0) {

            vector<RapidTrack*> tracks_to_add = select_->SelectDecays(mother,
                             daughters, n_decays, event_number_, pv_, first_ID);

            // Append tracks_to_add to the end of tracks_
            tracks_.insert(end(tracks_), begin(tracks_to_add), end(tracks_to_add));

            // Setup first ID for next loop.
            first_ID = tracks_.back()->GetID() + 1;
        }
    }

    return 0;
}
