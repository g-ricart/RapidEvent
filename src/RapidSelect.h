#ifndef RAPIDEVENT_RAPIDSELECT_H_
#define RAPIDEVENT_RAPIDSELECT_H_

#include "TString.h"

#include "RapidConfig.h"
#include "RapidTrack.h"

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

        int SelectTrack(const TString part_name, RapidTrack* track);

    private:
        RapidConfig* config_;
};

#endif // RAPIDEVENT_RAPIDSELECT_H_
