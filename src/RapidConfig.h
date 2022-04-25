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

        //! Load the event config file.
        int LoadEvent(const TString file_name);

    private:
        TString  file_name_; //! The name of the config file.
        TString  file_path_; //! Path to the config file.
};

#endif // RAPIDEVENT_RAPIDCONFIG_H_
