#include "RapidNorm.h"

#include <vector>
#include <map>
#include <fstream>
#include <iterator>
#include <iostream>

#include "TString.h"
#include "TFile.h"
#include "TH1D.h"
#include "TRandom3.h"

#include "RapidConfig.h"

using namespace std;

//______________________________________________________________________________
RapidNorm::RapidNorm()
{
    config_ = nullptr;
    random_ = nullptr;
    norm_map_.clear();
}

//______________________________________________________________________________
RapidNorm::RapidNorm(RapidConfig* config)
{
    config_ = config;
    norm_map_.clear();

    random_ = new TRandom3();
    random_->SetSeed(0);

    ComputeNorm();
}

//______________________________________________________________________________
RapidNorm::~RapidNorm()
{
    delete random_;
}

//______________________________________________________________________________
Double_t RapidNorm::GetMeanNumber(const TString part_name)
{
    if(norm_map_.count(part_name)) { // check if part_name is a key of the map
        return norm_map_.at(part_name);
    } else {
        cout << "ERROR in RapidNorm::GetMeanNumber : Unknown key "
             << part_name << " in normalisation map." << endl;
        return -1;
    }
}

//______________________________________________________________________________
Int_t RapidNorm::GetPoisson(const TString part_name)
{
    if(norm_map_.count(part_name)) { // check if part_name is a key of the map
        Double_t mean = norm_map_.at(part_name);
        return random_->Poisson(mean);
    } else {
        cout << "ERROR in RapidNorm::GetMeanNumber : Unknown key "
             << part_name << " in normalisation map." << endl;
        return -1;
    }
}

//______________________________________________________________________________
Int_t RapidNorm::ComputeNorm()
{
    vector<TString> prompt_particles  = config_->GetPrompts();

    for(auto part: prompt_particles) {

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
