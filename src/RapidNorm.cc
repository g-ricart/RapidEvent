#include "RapidNorm.h"

#include <vector>
#include <map>
#include <fstream>
#include <iterator>
#include <iostream>

#include "TString.h"
#include "TFile.h"
#include "TH1D.h"

#include "RapidConfig.h"

using namespace std;

//______________________________________________________________________________
RapidNorm::RapidNorm()
{
    config_ = nullptr;
    norm_map_.clear();
}

//______________________________________________________________________________
RapidNorm::RapidNorm(RapidConfig* config)
{
    config_ = config;
    norm_map_.clear();

    ComputeNorm();
}

//______________________________________________________________________________
RapidNorm::~RapidNorm()
{
    delete config_;
}

//______________________________________________________________________________
Double_t RapidNorm::GetMeanNumber(const TString part_name)
{
    if(norm_map_.count(part_name)) { // check if part_name is a key of the map
        return norm_map_.at(part_name);
    } else {
        cout << "ERROR in RapidNorm::GetMeanNumber : unknown key "
             << part_name << " in normalisation map." << endl;
        return -1;
    }
}

//______________________________________________________________________________
Int_t RapidNorm::ComputeNorm()
{
    vector<TString> particles  = config_->GetParticles();

    for(auto part: particles) {

        TString path = config_->GetNormFile(part);

        // fetch histogram
        TFile* in_file   = TFile::Open(path);
        TH1D*  norm_hist = (TH1D*)in_file->Get(part);

        Double_t integral = norm_hist->Integral();
        norm_map_[part] = integral;

        delete norm_hist;
        in_file->Close();
        delete in_file;
    }

    return 0;
}
