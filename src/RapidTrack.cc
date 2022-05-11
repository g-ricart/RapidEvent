#include "RapidTrack.h"

#include <map>
#include <vector>

#include "TString.h"

using namespace std;

//______________________________________________________________________________
RapidTrack::RapidTrack()
{
}

//______________________________________________________________________________
RapidTrack::RapidTrack(const TString track_name)
{
    name_ = track_name;
}

//______________________________________________________________________________
RapidTrack::~RapidTrack()
{
}

//______________________________________________________________________________
void RapidTrack::SetName(const TString track_name)
{
    name_ = track_name;
}

//______________________________________________________________________________
void RapidTrack::SetEventNumber(const Ssiz_t event_number)
{
    event_number_ = event_number;
}

//______________________________________________________________________________
void RapidTrack::SetParam(const TString param_name, const Double_t value)
{
    params_map_[param_name] = value;
}

//______________________________________________________________________________
vector<TString> RapidTrack::GetListOfParams()
{
    vector<TString> list_of_params;
    for (auto &it: params_map_) {
        list_of_params.push_back(it.first);
    }

    return list_of_params;
}

//______________________________________________________________________________
Double_t RapidTrack::GetParam(const TString param_name)
{
    return params_map_.at(param_name);
}

//______________________________________________________________________________
TString RapidTrack::GetName()
{
    return name_;
}

Ssiz_t RapidTrack::GetEventNumber()
{
    return event_number_;
}
