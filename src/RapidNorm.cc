#include "RapidNorm.h"

#include <vector>
#include <map>
#include <fstream>
#include <iterator>

#include "TString.h"
#include "TFile.h"
#include "TH1D.h"

#include "RapidConfig.h"

using namespace std;

//______________________________________________________________________________
RapidNorm::RapidNorm(RapidConfig* config) {

    config_ = config;
}

//______________________________________________________________________________
RapidNorm::~RapidNorm() {

    delete config_;
}

//______________________________________________________________________________
Double_t RapidNorm::GetMeanNumber(const TString part_name)
{
    return 0;
}

//______________________________________________________________________________
Int_t RapidNorm::ComputeNorm()
{
    vector<TString> particles  = config_->GetParticles();
    Double_t* acceptance       = config_->GetAcceptance();

    for(auto part: particles) {

        TString path = config_->GetNormFile(part);

        // fetch histogram
        TFile* in_file   = TFile::Open(path);
        TH1D*  norm_hist = (TH1D*)in_file->Get(part);

        // integrate histo in acceptance
        // -> fit histo ?

        delete norm_hist;
        in_file->Close();
        delete in_file;
    }

    delete acceptance;

    return 0;
}
