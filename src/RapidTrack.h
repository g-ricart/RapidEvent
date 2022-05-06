#ifndef RAPIDEVENT_RAPIDTRACK_H_
#define RAPIDEVENT_RAPIDTRACK_H_

#include <map>

#include "TString.h"

class RapidTrack {

    public:
        //! Default constructor.
        RapidTrack();
        //! Constructor specifying a name for the track.
        RapidTrack(const TString track_name);
        //! Destructor.
        ~RapidTrack();

        void SetName(const TString track_name);
        void SetParam(const TString param_name, const Double_t value);

        Double_t GetParam(const TString param_name);
        TString  GetName();

    private:
        std::map<TString, Double_t> params_map_;
        TString name_;

};

#endif //RAPIDEVENT_RAPIDTRACK_H_
