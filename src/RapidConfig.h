#ifndef RAPIDEVENT_RAPIDCONFIG_H_
#define RAPIDEVENT_RAPIDCONFIG_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <map>

#include <unistd.h>

#include "TString.h"

/*!
 * \class RapidConfig
 * Class used to read the event configuration file.
*/

class RapidConfig {

    public:
        //! Default constructor.
        RapidConfig();
        //! Destructor.
        ~RapidConfig();

        //! Load the event config file.
        Int_t Load(const TString file_name);
        //! Get the prompt particles to be included in the event.
        std::vector<TString> GetPrompts();
        //! Get the decays to be included in the event.
        std::map<TString, std::vector<TString>> GetDecays();
        //! Get the mothers to be included in the event.
        std::vector<TString> GetMothers();
        //! Get all the particles coming from the PV in the event.
        std::vector<TString> GetAllFromPV();
        //! Return the acceptance.
        //! For now fixed to LHCb acceptance: 2 < eta < 5
        //! \todo{Read the event config file and get acceptance from there.}
        //! \note{Not implemented yet.}
        Double_t* GetAcceptance();
        //! Return the normalisation file corresponding to the given particle.
        TString GetNormFile(const TString part_name);
        //! Return the file paths corresponding to the normalisation files.
        std::vector<TString> GetNormFiles();
        //! Return the data file corresponding to the given particle.
        TString GetDataFile(const TString part_name);
        //! Return the file paths corresponding to the data files.
        std::vector<TString> GetDataFiles();
        //! Return all the parameters found in the event config file as a
        //! vector of TString.
        std::vector<TString> GetParams();
        //! Return if PID is perfect (true) or not (false).
        Bool_t IsPIDPerfect();

    private:
        TString SanitizeName(TString name);
        Int_t   ParsePrompts(const TString event_str);
        Int_t   ParseDecay(const TString decay_str);
        Int_t   ParseParams(const TString params_str);
        Bool_t  MissingFile();

        TString                                 config_file_name_;
        TString                                 config_file_path_;
        std::vector<TString>                    prompts_in_event_;
        std::vector<TString>                    mothers_in_event_;
        std::map<TString, std::vector<TString>> decays_in_event_;
        std::vector<TString>                    params_;
        std::vector<TString>                    from_PV_;
        Bool_t                                  perfect_pid_;

};

#endif // RAPIDEVENT_RAPIDCONFIG_H_
