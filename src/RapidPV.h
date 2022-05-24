#ifndef RAPIDEVENT_RAPIDPV_H_
#define RAPIDEVENT_RAPIDPV_H_

#include "RapidConfig.h"

#include "Math/Point3D.h"
#include "TRandom3.h"

/*!
 * \class RapidPV
 * Class used to define the primary vertex and smear it.
*/

class RapidPV {
    public:
        //! Default constructor.
        RapidPV();
        //! Standard constructor.
        RapidPV(RapidConfig* config);
        //! Destructor.
        ~RapidPV();

        //! Set the number of tracks from the PV.
        void SetNTracks(Ssiz_t n_tracks);
        //! Set the coordinates of the PV.
        void SetXYZ(Double_t x, Double_t y, Double_t z);

        ROOT::Math::XYZPoint GetSmearedPV() {return vertex_smeared_;}
        ROOT::Math::XYZPoint GetTruePV() {return vertex_true_;}

    private:
        void SmearVertex();

        RapidConfig* config_;
        TRandom3*    random_;

        Ssiz_t               n_tracks_;
        ROOT::Math::XYZPoint vertex_true_;
		ROOT::Math::XYZPoint vertex_smeared_;
};

#endif // RAPIDEVENT_RAPIDPV_H_
