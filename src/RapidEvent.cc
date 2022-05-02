#include "RapidEvent.h"

#include "RapidConfig.h"

using namespace std;

//______________________________________________________________________________
RapidEvent::RapidEvent() {}

//______________________________________________________________________________
RapidEvent::~RapidEvent()
{
    delete config_;
}
