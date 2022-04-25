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

int RapidConfig::Load(const TString file_name)
{
    file_name_ = file_name;

    cout << "INFO in RapidConfig::Load : "
         << "loading event descriptor from file: " << file_name_
         << ".event" << std::endl;

    TString event_str;
    ifstream fin;
    fin.open(file_name_ + ".event");

    if(!fin.good()) {
		cout << "ERROR in RapidConfig::Load : file " << file_name_
             << ".event not found." << endl;
		return 1;
	}

    event_str.ReadLine(fin);
    fin.close();

    cout << "Read : " << event_str << endl;

    return 0;
}
