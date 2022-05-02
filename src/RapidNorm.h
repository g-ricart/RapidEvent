#ifndef RAPIDEVENT_RAPIDNORM_H_
#define RAPIDEVENT_RAPIDNORM_H_

#include <vector>
#include <map>
#include <fstream>

#include "TString.h"
#include "TFile.h"
#include "TH1D.h"

#include "RapidConfig.h"

class RapidNorm {

    public:

        typedef std::map<TString, Double_t> PartNorm;

        //! Default constructor.
        RapidNorm(RapidConfig* config);
        //! Destructor
        ~RapidNorm();

        //! Return the mean number of the specified particle
        //! expected in an event.
        Double_t GetMeanNumber(const TString part_name);

    private:

        Int_t ComputeNorm();

        RapidConfig* config_;
        PartNorm norm_map_;
};

#endif
