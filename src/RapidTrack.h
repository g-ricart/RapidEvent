#ifndef RAPIDEVENT_RAPIDTRACK_H_
#define RAPIDEVENT_RAPIDTRACK_H_

#include <map>

#include "TString.h"

class RapidTrack {

    public:
        //! Default constructor.
        RapidTrack();
        //! Standard constructor.
        RapidTrack(TString track_name);
        //! Destructor.
        ~RapidTrack();

        void SetParam(TString param_name, Double_t value);

        Double_t GetParam(TString param_name);
        TString  GetName();

    private:
        std::map<TString, Double_t> params_map_;
        TString name_;

};

#endif //RAPIDEVENT_RAPIDTRACK_H_
