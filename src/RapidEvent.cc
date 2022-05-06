#include "RapidEvent.h"

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
RapidEvent::RapidEvent(RapidConfig* config, RapidNorm* norm)
{
    config_ = config;
    norm_   = norm;
    RapidSelect* select_ = new RapidSelect(config_);

    event_number_ = 0;
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
int RapidEvent::BuildEvent()
{
    return 0;
}
