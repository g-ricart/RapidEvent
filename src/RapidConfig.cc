#include "RapidConfig.h"

#include <iostream>
#include <fstream>

#include "TString.h"

using namespace std;

RapidConfig::RapidConfig()
{
}

RapidConfig::~RapidConfig()
{
}

int RapidConfig::LoadEvent(const TString file_name)
{
    file_name_ = file_name;
    file_path_ = "/events/" + file_name + ".event";
    file_path_ = getenv("$RAPIDEVENT_ROOT") + file_path_;

    cout << "INFO in RapidConfig::Load : "
         << "loading event descriptor from file: " << file_path_
         << ".event" << endl;

    TString event_str;
    ifstream fin;
    fin.open(file_path_);

    if(!fin.good()) {
		cout << "ERROR in RapidConfig::Load : file " << file_path_
             << " not found." << endl;
		return 1;
	}

    event_str.ReadLine(fin);
    fin.close();

    cout << "INFO in RapidConfig::Load : event descriptor is: "
         << event_str << endl;

    //TODO Parse and check event_str

    return 0;
}
