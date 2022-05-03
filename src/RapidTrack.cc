#include "RapidTrack.h"

#include "RapidConfig.h"

using namespace std;

//______________________________________________________________________________
RapidTrack::RapidTrack()
{
    config_ = nullptr;
}

//______________________________________________________________________________
RapidTrack::RapidTrack(RapidConfig* config)
{
    config_ = config;
}

//______________________________________________________________________________
RapidTrack::~RapidTrack()
{
    delete config_;
}
