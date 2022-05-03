#include "RapidSelect.h"

using namespace std;

//______________________________________________________________________________
RapidSelect::RapidSelect()
{
    config_ = nullptr;
    norm_   = nullptr;
}

//______________________________________________________________________________
RapidSelect::RapidSelect(RapidConfig* config, RapidNorm* norm)
{
    config_ = config;
    norm_   = norm;
}

//______________________________________________________________________________
RapidSelect::~RapidSelect()
{
    delete config_;
    delete norm_;
}
