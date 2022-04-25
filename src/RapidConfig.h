#ifndef RAPIDEVENT_RAPIDCONFIG_H_
#define RAPIDEVENT_RAPIDCONFIG_H_

#include <iostream>
#include <fstream>

#include "TString.h"

/*!
 * \class RapidConfig
 * Call used to configure the generation of the events.
*/

class RapidConfig {

    public:
        //! Default constructor.
        RapidConfig();

        //! Destructor.
        ~RapidConfig();

        //! Load the config file.
        int Load(const TString file_name);

    private:
        TString file_name_; //! The name of the config file.
};

#endif // RAPIDEVENT_RAPIDCONFIG_H_
