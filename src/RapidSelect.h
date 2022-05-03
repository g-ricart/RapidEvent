#ifndef RAPIDEVENT_RAPIDSELECT_H_
#define RAPIDEVENT_RAPIDSELECT_H_

#include "RapidConfig.h"
#include "RapidNorm.h"

/*!
 * \class RapidSelect
 * Class used to select the tracks to add in an event.
*/

class RapidSelect {

    public:
        //! Default constructor.
        RapidSelect();
        //! Standard constructor.
        RapidSelect(RapidConfig* config, RapidNorm* norm);
        //! Destructor.
        ~RapidSelect();

    private:
        RapidConfig* config_;
        RapidNorm*   norm_;
};

#endif // RAPIDEVENT_RAPIDSELECT_H_
