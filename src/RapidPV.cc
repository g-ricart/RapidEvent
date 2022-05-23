#include "RapidPV.h"

#include "RapidPV.h"
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
