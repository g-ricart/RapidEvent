#ifndef RAPIDEVENT_RAPIDWRITER_H_
#define RAPIDEVENT_RAPIDWRITER_H_

#include <map>

#include "TFile.h"
#include "TTree.h"
#include "TString.h"

#include "RapidConfig.h"
#include "RapidEvent.h"

/*!
 * \class RapidWriter
 * Class used to write the tracks of an event in a ROOT file.
*/

class RapidWriter {

    public:
        //! Default constructor
        RapidWriter();
        //! Standard constructor
        RapidWriter(TString out_file_path, RapidConfig* config);
        //! Destructor
        ~RapidWriter();

        //! Write event into output file.
        Int_t SaveEvent(RapidEvent* event);

    private:
        Int_t SetupTree(TString out_file_path);
        TTree*       out_tree_;
        TFile*       out_file_;
        RapidConfig* config_;

        // variables to store in tree
        Ssiz_t event_number_;
        TString name_;
        std::map<TString, Double_t> params_map_;
        Size_t track_ID_;
        Size_t mother_ID_;
        Bool_t isPrompt_;

};

#endif // RAPIDEVENT_RAPIDWRITER_H_
