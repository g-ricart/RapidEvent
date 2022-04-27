#ifndef RAPIDEVENT_RAPIDCONFIG_H_
#define RAPIDEVENT_RAPIDCONFIG_H_

#include <iostream>
#include <fstream>
#include <vector>

#include "TString.h"

/*!
 * \class RapidConfig
 * Class used to read the events configuration file.
*/

class RapidConfig {

    public:
        //! Default constructor.
        RapidConfig();

        //! Destructor.
        ~RapidConfig();

        //! Load the event config file.
        int LoadEvent(const TString file_name);
        //! Get the particles to be included in the event.
        std::vector<TString> GetParticles();

    private:
        TString SanitizeName(TString name);
        int ParseEvent(const TString event_str);
        TString  file_name_; //! The name of the config file.
        TString  file_path_; //! Path to the config file.
        std::vector<TString> particles_in_event_; //! Particles to be included in the event.
};

#endif // RAPIDEVENT_RAPIDCONFIG_H_
