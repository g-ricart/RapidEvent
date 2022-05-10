#include <iostream>
#include <vector>

#include "TString.h"
#include "TRandom3.h"

#include "RapidConfig.h"
#include "RapidNorm.h"
#include "RapidEvent.h"

using namespace std;

int rapidEvent(const int kNEvtToGen, const TString kEvtFileName) {

    gRandom->SetSeed(0.);

    // Load configuration.
    RapidConfig* conf = new RapidConfig();
    if (conf->Load(kEvtFileName)) {
        cout << "Configuration failed!"
             << "             Terminating" << endl;
        return 1;
    }

    // Check configuration
    auto particles = conf->GetParticles();
    auto params    = conf->GetParams();

    cout << "particles : ";
    for(auto part: particles) {
        cout << part << " ";
    }
    cout << endl;

    cout << "parameters : ";
    for(auto param: params) {
        cout << param << " ";
    }
    cout << endl;

    // Check normalisation
    RapidNorm* norm = new RapidNorm(conf);
    cout << norm->GetMeanNumber("pip") << endl;
    cout << norm->GetMeanNumber("pim") << endl;
    cout << norm->GetMeanNumber("Kp")  << endl;
    cout << norm->GetMeanNumber("Km")  << endl;
    cout << norm->GetMeanNumber("pp")  << endl;
    cout << norm->GetMeanNumber("pm")  << endl;

    RapidSelect* select = new RapidSelect(conf);

    RapidEvent* event = new RapidEvent(conf, norm, select, 1);
    event->BuildEvent();

    cout << "Number of tracks : " << event->GetNumberOfTracks() << endl;

    return 0;
}

int main(int argc, char const *argv[]) {

    if (argc != 3) { // Check usage
		cout << "Usage: " << argv[0] << " event_file_name "
             << "number_of_events" << endl;
		return 1;
	}

    if(!getenv("RAPIDEVENT_ROOT")) {
		cout << "ERROR in main : environment variable "
             << "'RAPIDEVENT_ROOT' is not set" << endl
			 << "                    Terminating" << endl;
		return 1;
	}

    if(!getenv("RAPIDEVENT_EVTS")) {
        TString rapid_evts_path = getenv("RAPIDEVENT_ROOT");
        rapid_evts_path += "/events";
        setenv("RAPIDEVENT_EVTS", rapid_evts_path, 1);
        cout << "INFO in main : environment variable "
             << "'RAPIDEVENT_EVTS' was set to '" << getenv("RAPIDEVENT_EVTS")
             << "'" << endl;
    }

    if(!getenv("RAPIDEVENT_DATA")) {
        TString rapid_data_path = getenv("RAPIDEVENT_ROOT");
        rapid_data_path += "/data";
        setenv("RAPIDEVENT_DATA", rapid_data_path, 1);
        cout << "INFO in main : environment variable "
             << "'RAPIDEVENT_DATA' was set to '" << getenv("RAPIDEVENT_DATA")
             << "'" << endl;
    }

    if(!getenv("RAPIDEVENT_NORM")) {
        TString rapid_norm_path = getenv("RAPIDEVENT_ROOT");
        rapid_norm_path += "/norm";
        setenv("RAPIDEVENT_NORM", rapid_norm_path, 1);
        cout << "INFO in main : environment variable "
             << "'RAPIDEVENT_NORM' was set to '" << getenv("RAPIDEVENT_NORM")
             << "'" << endl;
    }

    const int kNEvtToGen = static_cast<int>(atof(argv[2]));
    const TString kEvtFileName = argv[1];

    int status = rapidEvent(kNEvtToGen, kEvtFileName);

    switch (status) {
        case 0:
            cout << "Status : success" << endl;
            break;
        case 1:
            cout << "Status : failed" << endl;
            break;
        default:
            cout << "Status : unknown" << endl;
            break;
    }

    return status;
}
