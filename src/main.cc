#include <iostream>
#include <vector>

#include "TString.h"

#include "RapidConfig.h"

using namespace std;

int rapidEvent(const int kNEvtToGen, const TString kEvtFileName) {

    // Load configuration.
    RapidConfig conf;
    if (conf.LoadEvent(kEvtFileName)) {
        cout << "Configuration failed!"
             << "             Terminating" << endl;
        return 1;
    }

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

    return status;
}
