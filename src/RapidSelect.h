#ifndef RAPIDEVENT_RAPIDSELECT_H_
#define RAPIDEVENT_RAPIDSELECT_H_

#include "RapidConfig.h"
#include "RapidNorm.h"

class RapidSelect {

    public:
        //! Constructor.
        RapidSelect(RapidConfig* config, RapidNorm* norm);
        //! Destructor.
        ~RapidSelect();

    private:
        RapidConfig* config_;
        RapidNorm*   norm_;
};

#endif
