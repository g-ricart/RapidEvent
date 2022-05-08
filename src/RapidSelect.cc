#include "RapidSelect.h"

#include "TString.h"
#include "TTree.h"

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
int RapidSelect::SelectTrack(RapidTrack* track, TTree* tree)
{
    return 0;
}
