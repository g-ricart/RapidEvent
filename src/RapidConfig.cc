#include "RapidConfig.h"

#include <iostream>
#include <fstream>
#include <vector>

#include "TString.h"

using namespace std;

RapidConfig::RapidConfig()
{
    file_name_ = "";
    file_path_ = "";
    particles_in_event_.clear();
}

RapidConfig::~RapidConfig()
{
}

vector<TString> GetParticles()
{
    return particles_in_event_;
}

int RapidConfig::LoadEvent(const TString file_name)
{
    file_name_ = file_name;
    file_path_ = "/events/" + file_name + ".event";
    file_path_ = getenv("RAPIDEVENT_ROOT") + file_path_;

    cout << "INFO in RapidConfig::Load : "
         << "loading event descriptor from file: " << file_path_ << endl;

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

    if (ParseEvent(event_str)) {
        return 1;
    }

    return 0;
}

int RapidConfig::ParseEvent(const TString event_str)
{
    TString token;
    Ssiz_t from = 0;

    while (event_str.Tokenize(token, from, " ")) {
        particles_in_event_.push_back(token);
    }

    return 0;
}
