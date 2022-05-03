#ifndef RAPIDEVENT_RAPIDNORM_H_
#define RAPIDEVENT_RAPIDNORM_H_

#include <vector>
#include <map>
#include <fstream>
#include <iostream>

#include "TString.h"
#include "TFile.h"
#include "TH1D.h"

#include "RapidConfig.h"

class RapidNorm {

    public:

        //! Default constructor.
        RapidNorm();
        //! Standard constructor.
        RapidNorm(RapidConfig* config);
        //! Destructor
        ~RapidNorm();

        //! Return the mean number of the specified particle
        //! expected in an event.
        Double_t GetMeanNumber(const TString part_name);

    private:

        typedef std::map<TString, Double_t> PartNorm;

        Int_t ComputeNorm();

        RapidConfig* config_;
        PartNorm     norm_map_;
};

#endif // RAPIDEVENT_RAPIDNORM_H_
