#include "RapidPV.h"

#include "TMath.h"
#include "Math/Point3D.h"
#include "TRandom3.h"

#include "RapidConfig.h"

using namespace std;

//______________________________________________________________________________
RapidPV::RapidPV()
{
    config_ = nullptr;
    random_ = nullptr;
}

//______________________________________________________________________________
RapidPV::RapidPV(RapidConfig* config)
{
    config_ = config;
    random_ = new TRandom3();
    random_->SetSeed(0);
}

//______________________________________________________________________________
RapidPV::~RapidPV()
{
    delete random_;
}

//______________________________________________________________________________
void RapidPV::SetNTracks(Ssiz_t n_tracks)
{
    n_tracks_ = n_tracks;
}

//______________________________________________________________________________
void RapidPV::SetXYZ(Double_t x, Double_t y, Double_t z) {
    vertex_true_ = ROOT::Math::XYZPoint(x,y,z);
	SmearVertex();
}

//______________________________________________________________________________
void RapidPV::SmearVertex() {

    // Units are in mm.
    Double_t xS = 0.010817 + 0.03784*TMath::Exp(-0.0815*ntracks_);
    Double_t yS = 0.010817 + 0.03784*TMath::Exp(-0.0815*ntracks_);
    Double_t zS = 0.04252  + 0.2235 *TMath::Exp(-0.0814*ntracks_);

    // Stored in micro-meters.
    vertex_smeared_ = ROOT::Math::XYZPoint( \
        vertex_true_.X() + random_->Gaus(0,xS)*1000., \
        vertex_true_.Y() + random_->Gaus(0,yS)*1000., \
        vertex_true_.Z() + random_->Gaus(0,zS)*1000.);

}
