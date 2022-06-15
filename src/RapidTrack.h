#ifndef RAPIDEVENT_RAPIDTRACK_H_
#define RAPIDEVENT_RAPIDTRACK_H_

#include <map>
#include <vector>

#include "TString.h"

#include "RapidPV.h"

/*!
 * \class RapidTrack
 * Class used to define a track.
*/

class RapidTrack {

    public:
        //! Default constructor.
        RapidTrack();
        //! Constructor specifying a name for the track.
        RapidTrack(const TString track_name);
        //! Destructor.
        ~RapidTrack();

        //! Set the name of the track.
        void SetName(const TString track_name);
        //! Set the event number of the track.
        void SetEventNumber(const Ssiz_t event_number);
        //! Set ID number of the track.
        void SetID(Ssiz_t id);
        //! Set ID number od the mother track.
        void SetMotherID(Ssiz_t id);
        //! Set the parameter `param_name` to `value`.
        void SetParam(const TString param_name, const Double_t value);
        //! Specify if a track is prompt (`true`) or not (`false`).
        void SetPrompt(const Bool_t is_prompt);
        //! Set the origin vertex of the track. Only called to set the primary
        //! vertex.
        void SetOriginVertex(RapidPV* pv);
        //! Execute PID (perfect for now). Only called when `perfectPID` is set
        //! in the config file.
        void SetPID();

        //! Get the list of parameters of the track.
        std::vector<TString> GetListOfParams();
        //! Get the parameter `param_name` value.
        Double_t GetParam(const TString param_name);
        //! Get the name of the track.
        TString  GetName();
        //! Get the event number of the track.
        Ssiz_t   GetEventNumber();
        //! Get the ID number of the track.
        Ssiz_t   GetID();
        //! Get mother ID number of the track.
        Ssiz_t   GetMotherID();
        //! Check if a track is prompt (`true`) or not (`false`).
        Bool_t   IsPrompt();

    private:
        std::map<TString, Double_t> params_map_;

        Ssiz_t  track_id_;
        Ssiz_t  mother_track_id_;
        Ssiz_t  event_number_;
        TString name_;
        Bool_t  is_prompt_;

};

#endif //RAPIDEVENT_RAPIDTRACK_H_
