#ifndef RAPIDEVENT_RAPIDSELECT_H_
#define RAPIDEVENT_RAPIDSELECT_H_

#include <vector>

#include "TTree.h"
#include "TString.h"
#include "TRandomGen.h"
#include "TObjArray.h"

#include "RapidConfig.h"
#include "RapidTrack.h"
#include "RapidPV.h"

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

        //! Select the prompt tracks to add in an event.
        //! Tracks are randomly selected in the data file corresponding to
        //! `part_name`. Their event numbers are set as well as their PVs.
        std::vector<RapidTrack*> SelectPromptTracks(TString  part_name,
                                                    Ssiz_t   n_tracks,
                                                    Ssiz_t   event_number,
                                                    RapidPV* pv,
                                                    Size_t   first_ID);

        //! Select the tracks associated to the decay defined by `mother`
        //! and `daughters`.
        //! Track are randomly slected in the data file corresponding to
        // `mother`. Their event number are set.
        std::vector<RapidTrack*> SelectDecays(TString              mother,
                                              std::vector<TString> daughters,
                                              Ssiz_t               n_decays,
                                              Ssiz_t               event_number,
                                              RapidPV*             pv,
                                              Size_t               first_ID);

    private:
        // Track selectors.
        Int_t SelectPromptTrack(RapidTrack* track, TTree*     tree,
                                                   TObjArray* branches,
                                                   Ssiz_t     n_entries);
        Ssiz_t SelectMotherTrack(RapidTrack* track, TString   part_name,
                                                    TTree*     tree,
                                                    TObjArray* branches,
                                                    Ssiz_t     n_entries);
        Int_t SelectDaughterTrack(RapidTrack* track, TString    part_name,
                                                     TTree*     tree,
                                                     TObjArray* branches,
                                                     Ssiz_t     entry_index);

        // Track parameters setters.
        Int_t SetTrackParams(RapidTrack* track, TObjArray* tokens,
                                                Double_t   value);
        Int_t SetTrackParamsTrue(RapidTrack* track, TObjArray* tokens,
                                                    Double_t   value);

        // Check if parameter needs to be stored.
        Bool_t KeepParam(TString param);

        RapidConfig* config_;
        TRandomMT64* random_;
        std::vector<TString> params_to_keep_;
};

#endif // RAPIDEVENT_RAPIDSELECT_H_
