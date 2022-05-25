#ifndef RAPIDEVENT_RAPIDEVENT_H_
#define RAPIDEVENT_RAPIDEVENT_H_

#include <vector>
#include <map>

#include "TString.h"

#include "RapidConfig.h"
#include "RapidNorm.h"
#include "RapidTrack.h"
#include "RapidSelect.h"
#include "RapidPV.h"

/*!
 * \class RapidEvent
 * Class defining an event.
*/

class RapidEvent {

    public:
        //! Default constructor.
        RapidEvent();
        //! Standard constructor.
        RapidEvent(RapidConfig* config, RapidNorm* norm, RapidSelect* select,
                                                         Ssiz_t event_number);
        //! Destructor
        ~RapidEvent();

        // Getters
        Ssiz_t   GetEventNumber();
        Ssiz_t   GetNumberOfTracks();
        RapidPV* GetPV();
        std::vector<RapidTrack*> GetTracks();
        //! Build the event.
        int BuildEvent();

    private:
        RapidConfig* config_;
        RapidNorm*   norm_;
        RapidSelect* select_;
        RapidPV*     pv_;
        std::vector<RapidTrack*> tracks_;
        // Map containing the particle name as a key and the number of tracks
        // of this particle in the event as the associated value.
        std::map<TString, Int_t> n_part_map_;

        Ssiz_t event_number_;
        Ssiz_t n_tracks_;
        Ssiz_t pv_n_tracks_;
};

#endif // RAPIDEVENT_RAPIDEVENT_H_
