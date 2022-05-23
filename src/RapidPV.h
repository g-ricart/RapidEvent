#ifndef RAPIDEVENT_RAPIDPVSMEAR_H_
#define RAPIDEVENT_RAPIDPVSMEAR_H_

#include "RapidConfig.h"
#include "TRandom3.h"

/*!
 * \class RapidPV
 * Class used to define the primary vertex and smear it.
*/

class RapidPV {
    public:
        //! Default constructor.
        RapidPV();
        //! Standard constructor.
        RapidPV(RapidConfig* config);
        //! Destructor.
        ~RapidPV();

    private:
        RapidConfig* config_;
        TRandom3*    random_;
};

#endif // RAPIDEVENT_RAPIDPVSMEAR_H_
