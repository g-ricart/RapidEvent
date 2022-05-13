#ifndef RAPIDEVENT_RAPIDSELECT_H_
#define RAPIDEVENT_RAPIDSELECT_H_

#include <vector>

#include "TTree.h"
#include "TString.h"
#include "TRandomGen.h"
#include "TObjArray.h"

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
        Int_t SelectTrack(RapidTrack* track, TTree* tree, TObjArray* branches);
        Int_t SetTrackParams(RapidTrack* track, TObjArray* tokens,
                                                Double_t value);
        Int_t SetTrackParamsTrue(RapidTrack* track, TObjArray* tokens,
                                                    Double_t value);
        Bool_t KeepParam(TString param);

        RapidConfig* config_;
        TRandomMT64* random_;
        std::vector<TString> params_to_keep_;
};

#endif // RAPIDEVENT_RAPIDSELECT_H_
