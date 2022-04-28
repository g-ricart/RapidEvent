#include "RapidConfig.h"

#include <iostream>
#include <fstream>
#include <vector>

#include <unistd.h>

#include "TString.h"

using namespace std;

//______________________________________________________________________________
RapidConfig::RapidConfig()
{
    config_file_name_ = "";
    config_file_path_ = "";
    particles_in_event_.clear();
}

//______________________________________________________________________________
RapidConfig::~RapidConfig()
{
}

//______________________________________________________________________________
int RapidConfig::LoadEvent(const TString file_name)
{
    config_file_name_ = file_name;
    config_file_path_ = "/events/" + file_name + ".event";
    config_file_path_ = getenv("RAPIDEVENT_ROOT") + config_file_path_;

    cout << "INFO in RapidConfig::Load : "
         << "loading event descriptor from file: " << config_file_path_ << endl;

    TString event_str;
    ifstream fin;
    fin.open(config_file_path_);

    if(!fin.good()) {
		cout << "ERROR in RapidConfig::Load : file " << config_file_path_
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

    if (MissingFile()) {
        return 1;
    }

    return 0;
}

//______________________________________________________________________________
vector<TString> RapidConfig::GetParticles()
{
    return particles_in_event_;
}

//______________________________________________________________________________
TString RapidConfig::SanitizeName(TString name)
{
    name = name.ReplaceAll("+","p");
	name = name.ReplaceAll("-","m");
	name = name.ReplaceAll("*","st");
	name = name.ReplaceAll("(","_");
	name = name.ReplaceAll(")","_");
	name = name.ReplaceAll("[","_");
	name = name.ReplaceAll("]","_");
	name = name.ReplaceAll("<","_");
	name = name.ReplaceAll(">","_");
	name = name.ReplaceAll("{","_");
	name = name.ReplaceAll("}","_");
	name = name.ReplaceAll(" ","_");
	name = name.ReplaceAll("$","");
	name = name.ReplaceAll("%","");
	name = name.ReplaceAll("&","");
	name = name.ReplaceAll("/","");
	name = name.ReplaceAll(":","");
	name = name.ReplaceAll(";","");
	name = name.ReplaceAll("=","");
	name = name.ReplaceAll("\\","");
	name = name.ReplaceAll("^","");
	name = name.ReplaceAll("|","");
	name = name.ReplaceAll(",","");
	name = name.ReplaceAll(".","");
	name.Remove(TString::kBoth,'_');

    return name;
}

//______________________________________________________________________________
int RapidConfig::ParseEvent(const TString event_str)
{
    TString token;
    Ssiz_t from = 0;

    while (event_str.Tokenize(token, from, " ")) {
        particles_in_event_.push_back(SanitizeName(token));
    }

    return 0;
}

//______________________________________________________________________________
int RapidConfig::MissingFile()
{
    for(auto part: particles_in_event_) {

        TString data_file_path = getenv("RAPIDEVENT_DATA");
        TString data_file_name = part + "_tree.root";
        data_file_path += "/";
        data_file_path += data_file_name;

        TString norm_file_path = getenv("RAPIDEVENT_NORM");
        TString norm_file_name = part + "_norm.root";
        norm_file_path += "/";
        norm_file_path += norm_file_name;

        if(access(data_file_path, R_OK) == -1) { // Check the file can be read.
            cout << "ERROR in RapidConfig::MissingFile : "
                 << "missing data file : " << data_file_path << endl;
            return 1;
        }
        if(access(norm_file_path, R_OK) == -1) {
            cout << "ERROR in RapidConfig::MissingFile : "
                 << "missing normalisation file : " << norm_file_path << endl;
            return 1;
        }
    }
    return 0;
}
