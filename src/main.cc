#include <iostream>
#include <vector>
#include <ctime>

#include "TString.h"

#include "RapidConfig.h"
#include "RapidNorm.h"
#include "RapidEvent.h"
#include "RapidWriter.h"

using namespace std;

int rapidEvent(const int kNEvtToGen, const TString kEvtFileName) {

    clock_t t0, t1;

    // Load configuration.
    RapidConfig* conf = new RapidConfig();
    if (conf->Load(kEvtFileName)) {
        cout << "Configuration failed!"
             << "             Terminating" << endl;
        return 1;
    } else {
        cout << "INFO in main : Successfully loaded configuration !" << endl;
    }

    // Writer
    RapidWriter* writer = new RapidWriter(kEvtFileName + "_tree.root", conf);

    // Normalisator
    RapidNorm* norm = new RapidNorm(conf);

    // Selector
    RapidSelect* select = new RapidSelect(conf);

    cout << "INFO in main : Initialisation done !" << endl;

    cout << "INFO in main : Generating events" << endl;

    Float_t delta_t = 0; // Total time used to generate the events.

    for (Ssiz_t i = 0; i < kNEvtToGen; i++) {
        t0 = clock();

        RapidEvent* event = new RapidEvent(conf, norm, select, i+1);
        event->BuildEvent();

        // Save event
        writer->SaveEvent(event);

        delete event;

        // Display mean event per second.
        t1 = clock();
        delta_t += (float(t1) - float(t0)) / CLOCKS_PER_SEC;
        cout << "\r" << "     " << i+1 << "/" << kNEvtToGen;
        cout << "    " << delta_t/(i+1) << " s/evt" << flush;
    }
    cout << "\r";

    cout << "INFO in main : Generated " << kNEvtToGen << " events in ";
    cout << delta_t << " seconds (" << delta_t/kNEvtToGen << " s/evt).";
    cout << endl;

    delete select;
    delete norm;
    delete writer;
    delete conf;
    return 0;
}

int main(int argc, char const *argv[]) {

    //ios_base::sync_with_stdio(false); // Faster cout

    if (argc != 3) { // Check usage
		cout << "Usage: " << argv[0] << " event_file_name "
             << "number_of_events" << endl;
		return 1;
	}

    if(!getenv("RAPIDEVENT_ROOT")) {
		cout << "ERROR in main : Environment variable "
             << "'RAPIDEVENT_ROOT' is not set" << endl
			 << "                    Terminating" << endl;
		return 1;
	}

    if(!getenv("RAPIDEVENT_EVTS")) {
        TString rapid_evts_path = getenv("RAPIDEVENT_ROOT");
        rapid_evts_path += "/events";
        setenv("RAPIDEVENT_EVTS", rapid_evts_path, 1);
        cout << "INFO in main : Environment variable "
             << "'RAPIDEVENT_EVTS' was set to '" << getenv("RAPIDEVENT_EVTS")
             << "'" << endl;
    }

    if(!getenv("RAPIDEVENT_DATA")) {
        TString rapid_data_path = getenv("RAPIDEVENT_ROOT");
        rapid_data_path += "/data";
        setenv("RAPIDEVENT_DATA", rapid_data_path, 1);
        cout << "INFO in main : Environment variable "
             << "'RAPIDEVENT_DATA' was set to '" << getenv("RAPIDEVENT_DATA")
             << "'" << endl;
    }

    if(!getenv("RAPIDEVENT_NORM")) {
        TString rapid_norm_path = getenv("RAPIDEVENT_ROOT");
        rapid_norm_path += "/norm";
        setenv("RAPIDEVENT_NORM", rapid_norm_path, 1);
        cout << "INFO in main : Environment variable "
             << "'RAPIDEVENT_NORM' was set to '" << getenv("RAPIDEVENT_NORM")
             << "'" << endl;
    }

    const int kNEvtToGen = static_cast<int>(atof(argv[2]));
    const TString kEvtFileName = argv[1];

    int status = rapidEvent(kNEvtToGen, kEvtFileName);

    cout << "STATUS : ";

    switch (status) {
        case 0:
            cout << "success" << endl;
            break;
        case 1:
            cout << "failed" << endl;
            break;
        default:
            cout << "unknown" << endl;
            break;
    }

    return status;
}
