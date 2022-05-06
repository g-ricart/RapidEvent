#include "RapidSelect.h"

#include "RapidConfig.h"

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
