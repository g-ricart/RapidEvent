#ifndef RAPIDEVENT_RAPIDSELECT_H_
#define RAPIDEVENT_RAPIDSELECT_H_

#include "RapidConfig.h"

/*!
 * \class RapidSelect
 * Class used to select the tracks to add in an event.
*/

class RapidSelect {

    public:
        //! Default constructor.
        RapidSelect();
        //! Standard constructor.
        RapidSelect(RapidConfig* config);
        //! Destructor.
        ~RapidSelect();

    private:
        RapidConfig* config_;
};

#endif // RAPIDEVENT_RAPIDSELECT_H_
