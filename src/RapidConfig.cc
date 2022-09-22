#include "RapidConfig.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <map>

#include <unistd.h>

#include "TString.h"

using namespace std;

//______________________________________________________________________________
RapidConfig::RapidConfig()
{
    config_file_name_ = "";
    config_file_path_ = "";
    prompts_in_event_.clear();
    decays_in_event_.clear();
    params_.clear();
    perfect_pid_ = false;
}

//______________________________________________________________________________
RapidConfig::~RapidConfig()
{
}

//______________________________________________________________________________
Int_t RapidConfig::Load(const TString file_name)
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
        Int_t colon = buffer.Index(":");
        // Get the string before the colon and strip it
		TString command = buffer(0,colon);
		command = command.Strip(TString::kBoth);
        // Get the string after the colon and strip it
		TString value = buffer(colon+1, buffer.Length()-colon-1);
		value = value.Strip(TString::kBoth);

        if (command == "prompt") {
            ParsePrompts(value);
        } else if (command == "decay") {
            ParseDecay(value);
        } else if (command == "params") {
            ParseParams(value);
        } else if (command == "perfectPID") {
            perfect_pid_ = true;
            params_.push_back(TString("ProbNNpi"));
            params_.push_back(TString("ProbNNp"));
            params_.push_back(TString("ProbNNK"));
            params_.push_back(TString("ProbNNmu"));
        } else {
            cout << "ERROR in RapidConfig::Load : Unknown setting '"
                 << command << "' in " << config_file_path_ << endl;
            return 1;
        }
    }
    fin.close();

    // Append prompts and mothers to list of particles coming from the PV.
    from_PV_.insert(end(from_PV_), begin(prompts_in_event_),
                                   end(prompts_in_event_));
    from_PV_.insert(end(from_PV_), begin(mothers_in_event_),
                                   end(mothers_in_event_));


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

//______________________________________________________________________________
map<TString, vector<TString>> RapidConfig::GetDecays()
{
    return decays_in_event_;
}

//______________________________________________________________________________
vector<TString> RapidConfig::GetMothers()
{
    return mothers_in_event_;
}

//______________________________________________________________________________
vector<TString> RapidConfig::GetAllFromPV()
{
    return from_PV_;
}

//______________________________________________________________________________
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

    for(auto part_name: from_PV_) {

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

    for(auto part_name: from_PV_) {

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
Int_t RapidConfig::ParsePrompts(const TString event_str)
{
    TString token;
    Ssiz_t  from = 0;

    while (event_str.Tokenize(token, from, " ")) {
        prompts_in_event_.push_back(SanitizeName(token));
    }

    return 0;
}

//______________________________________________________________________________
Int_t RapidConfig::ParseDecay(const TString decay_str)
{
    // Get the position of the "->".
    Ssiz_t arrow_start = decay_str.Index("->");
    Ssiz_t arrow_end   = arrow_start + 1;

    // Get the mother particle name and strip it.
    TString mother = decay_str(0, arrow_start);
    mother = mother.Strip(TString::kBoth);

    // Get the daughter particles string and strip it.
    TString daughters_str = decay_str(arrow_end+1,
                                      decay_str.Length()-arrow_end-1);
    daughters_str = daughters_str.Strip(TString::kBoth);

    // Parse daughter names and store them in a vector.
    vector<TString> daughters;
    TString token;
    Ssiz_t  from = 0;
    while (daughters_str.Tokenize(token, from, " ")) {
        daughters.push_back(SanitizeName(token));
    }

    // Fill the map of decays.
    decays_in_event_[SanitizeName(mother)] = daughters;

    // Add mother to mothers vector.
    mothers_in_event_.push_back(mother);

    return 0;
}

//______________________________________________________________________________
Int_t RapidConfig::ParseParams(const TString params_str)
{
    TString token;
    Ssiz_t  from = 0;

    while (params_str.Tokenize(token, from, " ")) {
        params_.push_back(token);
    }

    return 0;
}

//______________________________________________________________________________
Bool_t RapidConfig::MissingFile()
{
    for(auto part: from_PV_) {

        TString data_file_path = GetDataFile(part);

        TString norm_file_path = GetNormFile(part);

        if(access(data_file_path, R_OK) == -1) { // Check the file can be read.
            cout << "ERROR in RapidConfig::MissingFile : "
                 << "Missing data file : " << data_file_path << endl;
            return true;
        }
        if(access(norm_file_path, R_OK) == -1) {
            cout << "ERROR in RapidConfig::MissingFile : "
                 << "Missing normalisation file : " << norm_file_path << endl;
            return true;
        }
    }
    return false;
}

//______________________________________________________________________________
Bool_t RapidConfig::IsPIDPerfect()
{
    return perfect_pid_;
}

//______________________________________________________________________________
Int_t RapidConfig::PrintParams()
{
    if (params_.size() == 0) {
        cout << "WARNING in RapidConfig::PrintParams : "
             << "Parameters list is empty." << endl;
        return 1;
    }
    return 0;
}
