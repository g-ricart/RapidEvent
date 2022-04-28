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

    vector<TString> particles = conf.GetParticles();

    for(auto i: particles) {
        cout << i << "  ";
    }
    cout << endl;

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

    if(!getenv("RAPIDEVENT_DATA")) {
        setenv("RAPIDEVENT_DATA", "$RAPIDEVENT_ROOT/data", 1);
        cout << "INFO in main : environment variable "
             << "'RAPIDEVENT_DATA' was set to '" << getenv("RAPIDEVENT_DATA")
             << "'" << endl;
    }

    const int kNEvtToGen = static_cast<int>(atof(argv[2]));
    const TString kEvtFileName = argv[1];

    int status = rapidEvent(kNEvtToGen, kEvtFileName);

    return status;
}
