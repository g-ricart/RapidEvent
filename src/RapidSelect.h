#ifndef RAPIDEVENT_RAPIDSELECT_H_
#define RAPIDEVENT_RAPIDSELECT_H_

#include <vector>

#include "TFile.h"
#include "TString.h"
#include "TTree.h"
#include "TBranch.h"
#include "TString.h"
#include "TRandomGen.h"

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

        std::vector<RapidTrack*> SelectTracks(TString part_name,
                                              Int_t n_tracks,
                                              Ssiz_t event_number);

    private:
        int SelectTrack(RapidTrack* track, TTree* tree, TObjArray* branches);

        RapidConfig* config_;
        TRandomMT64* random_;
        std::vector<TString> params_to_keep_;
};

#endif // RAPIDEVENT_RAPIDSELECT_H_
