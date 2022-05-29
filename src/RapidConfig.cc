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
    prompts_in_event_.clear();
    params_.clear();
    perfect_pid_ = false;
}

//______________________________________________________________________________
RapidConfig::~RapidConfig()
{
}

//______________________________________________________________________________
int RapidConfig::Load(const TString file_name)
{
    config_file_name_ = file_name + ".event";
    config_file_path_ = "/" + config_file_name_;
    config_file_path_ = getenv("RAPIDEVENT_EVTS") + config_file_path_;

    cout << "INFO in RapidConfig::Load : "
         << "Loading event descriptor from file: " << config_file_path_ << endl;

    ifstream fin;
    fin.open(config_file_path_);

    if(!fin.good()) {
		cout << "ERROR in RapidConfig::Load : File " << config_file_path_
             << " not found." << endl;
		return 1;
	}

    TString buffer;
    while (fin.good()) {

        buffer.ReadLine(fin);
        // Ignore empty lines or comments
        if (buffer.Length() == 0 || buffer[0] == '#') {
            continue;
        }

        // Get position of the colon in the TString
        int colon = buffer.Index(":");
        // Get the string before the colon and strip it
		TString command = buffer(0,colon);
		command = command.Strip(TString::kBoth);
        // Get the string after the colon and strip it
		TString value = buffer(colon+1, buffer.Length()-colon-1);
		value = value.Strip(TString::kBoth);

        if (command == "prompt") {
            ParsePrompts(value);
        } else if (command == "params") {
            ParseParams(value);
        } else if (command == "perfectPID") {
            perfect_pid_ = true;
        } else {
            cout << "ERROR in RapidConfig::Load : Unknown setting '"
                 << command << "' in " << config_file_path_ << endl;
            return 1;
        }
    }
    fin.close();

    if (MissingFile()) {
        return 1;
    }

    return 0;
}

//______________________________________________________________________________
vector<TString> RapidConfig::GetPrompts()
{
    return prompts_in_event_;
}

Double_t* RapidConfig::GetAcceptance()
{
    static Double_t acceptance[2] = {2, 5};
    return acceptance;
}

//______________________________________________________________________________
TString RapidConfig::GetNormFile(const TString part_name)
{
    TString norm_file_path = getenv("RAPIDEVENT_NORM");
    TString norm_file_name = part_name + "_norm.root";
    norm_file_path += "/";
    norm_file_path += norm_file_name;

    return norm_file_path;
}

//______________________________________________________________________________
vector<TString> RapidConfig::GetNormFiles()
{
    vector<TString> norm_files;

    for(auto part_name: prompts_in_event_) {

        norm_files.push_back(GetNormFile(part_name));
    }

    return norm_files;
}

//______________________________________________________________________________
TString RapidConfig::GetDataFile(const TString part_name)
{
    TString data_file_path = getenv("RAPIDEVENT_DATA");
    TString data_file_name = part_name + "_tree.root";
    data_file_path += "/";
    data_file_path += data_file_name;

    return data_file_path;
}

//______________________________________________________________________________
vector<TString> RapidConfig::GetDataFiles()
{
    vector<TString> data_files;

    for(auto part_name: prompts_in_event_) {

        data_files.push_back(GetNormFile(part_name));
    }

    return data_files;
}

//______________________________________________________________________________
vector<TString> RapidConfig::GetParams()
{
    return params_;
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
int RapidConfig::ParsePrompts(const TString event_str)
{
    TString token;
    Ssiz_t  from = 0;

    while (event_str.Tokenize(token, from, " ")) {
        prompts_in_event_.push_back(SanitizeName(token));
    }

    return 0;
}

//______________________________________________________________________________
int RapidConfig::ParseParams(const TString params_str)
{
    TString token;
    Ssiz_t  from = 0;

    while (params_str.Tokenize(token, from, " ")) {
        params_.push_back(token);
    }

    return 0;
}

//______________________________________________________________________________
int RapidConfig::MissingFile()
{
    for(auto part: prompts_in_event_) {

        TString data_file_path = GetDataFile(part);

        TString norm_file_path = GetNormFile(part);

        if(access(data_file_path, R_OK) == -1) { // Check the file can be read.
            cout << "ERROR in RapidConfig::MissingFile : "
                 << "Missing data file : " << data_file_path << endl;
            return 1;
        }
        if(access(norm_file_path, R_OK) == -1) {
            cout << "ERROR in RapidConfig::MissingFile : "
                 << "Missing normalisation file : " << norm_file_path << endl;
            return 1;
        }
    }
    return 0;
}

//______________________________________________________________________________
Bool_t RapidConfig::IsPIDPerfect()
{
    return perfect_pid_;
}
