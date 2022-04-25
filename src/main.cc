#include <iostream>

#include "RapidConfig.h"

using namespace std;

int rapidEvent(const int kNEvtToGen, const TString kEvtFile) {

    RapidConfig conf;
    if (conf.Load(kEvtFile)) {
        return 1;
    }
    return 0;
}

int main(int argc, char const *argv[]) {

    if (argc != 3) {
		cout << "Usage: " << argv[0] << " event_file "
             << "number_of_events" << endl;
		return 1;
	}

    const int kNEvtToGen = static_cast<int>(atof(argv[2]));
    const TString kEvtFile = argv[1];

    int status = rapidEvent(kNEvtToGen, kEvtFile);

    return status;
}
