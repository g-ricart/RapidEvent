#include "RapidTrack.h"

#include <map>
#include <vector>
#include <stdexcept> // std::out_of_range

#include "TString.h"
#include "Math/Point3D.h"

#include "RapidPV.h"

using namespace std;

//______________________________________________________________________________
RapidTrack::RapidTrack()
{
    track_id_        = 0;
    mother_track_id_ = 0;
    is_prompt_       = true;
}

//______________________________________________________________________________
RapidTrack::RapidTrack(const TString track_name)
{
    name_            = track_name;
    track_id_        = 0;
    mother_track_id_ = 0;
    is_prompt_       = true;
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
void RapidTrack::SetID(Ssiz_t id)
{
    track_id_ = id;
}


//______________________________________________________________________________
void RapidTrack::SetMotherID(Ssiz_t id)
{
    mother_track_id_ = id;
}
//______________________________________________________________________________
void RapidTrack::SetParam(const TString param_name, const Double_t value)
{
    params_map_[param_name] = value;
}

//______________________________________________________________________________
void RapidTrack::SetPrompt(const Bool_t is_prompt)
{
    is_prompt_ = is_prompt;
}

//______________________________________________________________________________
void RapidTrack::SetOriginVertex(RapidPV* pv)
{
    ROOT::Math::XYZPoint smeared_pv = pv->GetSmearedPV();
    ROOT::Math::XYZPoint true_pv    = pv->GetTruePV();

    // smeared PV params.
    SetParam("origX", smeared_pv.x());
    SetParam("origY", smeared_pv.y());
    SetParam("origZ", smeared_pv.z());

    // true PV params.
    SetParam("origX_TRUE", true_pv.x());
    SetParam("origY_TRUE", true_pv.y());
    SetParam("origZ_TRUE", true_pv.z());
}

//______________________________________________________________________________
void RapidTrack::SetPID()
{
    params_map_[TString("ProbNNpi")] = 0.;
    params_map_[TString("ProbNNp")] = 0.;
    params_map_[TString("ProbNNK")] = 0.;
    params_map_[TString("ProbNNmu")] = 0.;

    if (name_.Contains("pi")) {
        params_map_[TString("ProbNNpi")] = 1.;
    } else if (name_.Contains("p")) { // check pi then p to avoid mis-ID
        params_map_[TString("ProbNNp")] = 1.;
    } else if (name_.Contains("K")) {
        params_map_[TString("ProbNNK")] = 1.;
    } else if (name_.Contains("mu")) {
        params_map_[TString("ProbNNmu")] = 1.;
    }
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
    Double_t param;

    try {
        param = params_map_.at(param_name);
        return param;
    }
    catch (const std::out_of_range& oor) { // If param_name is not a parameter
                                           // of the track, return an absurd
                                           // value.
        return -999;
    }
}

//______________________________________________________________________________
TString RapidTrack::GetName()
{
    return name_;
}

//______________________________________________________________________________
Ssiz_t RapidTrack::GetEventNumber()
{
    return event_number_;
}

//______________________________________________________________________________
Ssiz_t RapidTrack::GetID()
{
    return track_id_;
}

//______________________________________________________________________________
Ssiz_t RapidTrack::GetMotherID()
{
    return mother_track_id_;
}

//______________________________________________________________________________
Bool_t RapidTrack::IsPrompt()
{
    return is_prompt_;
}
