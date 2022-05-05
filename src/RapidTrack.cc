#include "RapidTrack.h"

#include <map>

#include "TString.h"

using namespace std;

//______________________________________________________________________________
RapidTrack::RapidTrack()
{
}

//______________________________________________________________________________
RapidTrack::RapidTrack(TString track_name)
{
    name_ = track_name;
}

//______________________________________________________________________________
RapidTrack::~RapidTrack()
{
}

//______________________________________________________________________________
void RapidTrack::SetParam(TString param_name, Double_t value)
{
    params_map_[param_name] = value;
}

//______________________________________________________________________________
Double_t RapidTrack::GetParam(TString param_name)
{
    return params_map_.at(param_name);
}

//______________________________________________________________________________
TString RapidTrack::GetName()
{
    return name_;
}
