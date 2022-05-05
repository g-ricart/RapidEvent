#include "RapidEvent.h"

#include "RapidConfig.h"
#include "RapidNorm.h"
#include "RapidTrack.h"

using namespace std;

//______________________________________________________________________________
RapidEvent::RapidEvent() {}

//______________________________________________________________________________
RapidEvent::~RapidEvent()
{
    delete config_;

    while(!tracks_.empty()) {
		delete tracks_[tracks_.size()-1];
		tracks_.pop_back();
	}
}
