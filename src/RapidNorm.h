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
#include "TString.h"
#include "TRandom3.h"

/*!
 * \class RapidNorm
 * Class used to get the number of particles to put in an event.
*/

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
        //! Return a random number according to a Poisson law for the
        //! specified particle.
        Int_t GetPoisson(const TString part_name);

    private:

        Int_t ComputeNorm();

        RapidConfig*                config_;
        std::map<TString, Double_t> norm_map_;
        TRandom3*                   random_;
};

#endif // RAPIDEVENT_RAPIDNORM_H_
