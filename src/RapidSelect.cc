#include "RapidSelect.h"

#include "TString.h"

#include "RapidConfig.h"
#include "RapidTrack.h"

using namespace std;

//______________________________________________________________________________
RapidSelect::RapidSelect()
{
    config_ = nullptr;
}

//______________________________________________________________________________
RapidSelect::RapidSelect(RapidConfig* config)
{
    config_ = config;
}

//______________________________________________________________________________
RapidSelect::~RapidSelect()
{
    delete config_;
}

//______________________________________________________________________________
int RapidSelect::SelectTrack(TString part_name, RapidTrack* track)
{
    return 0;
}
