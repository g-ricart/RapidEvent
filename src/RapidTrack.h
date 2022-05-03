#ifndef RAPIDEVENT_RAPIDTRACK_H_
#define RAPIDEVENT_RAPIDTRACK_H_

#include "RapidConfig.h"

class RapidTrack {

    public:
        //! Default constructor.
        RapidTrack();
        //! Standard constructor.
        RapidTrack(RapidConfig* config);
        //! Destructor.
        ~RapidTrack();

    private:
        RapidConfig* config_;
};

#endif //RAPIDEVENT_RAPIDTRACK_H_
