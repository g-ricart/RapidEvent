#ifndef RAPIDEVENT_RAPIDEVENT_H_
#define RAPIDEVENT_RAPIDEVENT_H_

#include "RapidConfig.h"

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
};

#endif // RAPIDEVENT_RAPIDEVENT_H_
