#include <iostream>

#include ""

using namespace std;

int rapidEvent(const int kNEvtToGen) {

    // Check environment variables
    if(!getenv("RAPIDEVENT_ROOT")) {
		std::cout << "ERROR in rapidEvent : environment variable RAPIDEVENT_ROOT is not set" << std::endl
			  << "                    Terminating" << std::endl;
		return 1;

	}
}

int main(int argc, char const *argv[]) {

    if (argc != 2) {
		cout << "Usage: " << argv[0] << " numberToGenerate" << endl;
		return 1;
	}

    const int kNEvtToGen = static_cast<int>(atof(argv[1]));

    int status = rapidEvent(kNEvtToGen);

    return status;
}
