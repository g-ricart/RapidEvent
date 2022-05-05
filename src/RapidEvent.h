#ifndef RAPIDEVENT_RAPIDEVENT_H_
#define RAPIDEVENT_RAPIDEVENT_H_

#include "RapidConfig.h"
#include "RapidNorm.h"
#include "RapidTrack.h"

/*!
 * \class RapidEvent
 * Class defining an event.
*/

class RapidEvent {

    public:
        //!Default constructor.
        RapidEvent();

        //! Destructor
        ~RapidEvent();

    private:
        RapidConfig* config_;
        RapidNorm*   norm_;
        std::vector<RapidTrack*> tracks_;
};

#endif // RAPIDEVENT_RAPIDEVENT_H_
