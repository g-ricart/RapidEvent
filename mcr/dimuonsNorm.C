#include <iostream>

#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TString.h"

using namespace std;

void buildPreliminaryDimuonsNorm() {
    const Int_t n_bins_eta = 12;
    const Double_t eta_min = 2.;
    const Double_t eta_max = 5.;

    const Double_t deta = (eta_max - eta_min)/double(n_bins_eta);

    Double_t eta_xbins[n_bins_eta+1];
    Double_t etas[n_bins_eta];
    Double_t dNdy[n_bins_eta];

    for (Int_t i = 0; i < n_bins_eta; i++) {
        eta_xbins[i] = eta_min + i*deta;
    }
    eta_xbins[n_bins_eta] = eta_max;

    TH1D* eta_hist = new TH1D("gammastar", "", n_bins_eta, eta_xbins);
    for (Int_t i = 0; i < n_bins_eta; i++) {
        Int_t bin = i + 1;
        etas[i] = eta_hist->GetBinCenter(bin);

        // Read file.
        TTree* tree = new TTree();
        TString file_name;
        file_name.Form("../norm/rawData/eta_%g.txt", etas[i]);
        tree->ReadFile(file_name, "Q/D:dNdQdY:zboui:zboue");

        Double_t dNdQdY = 0.;
        dNdy[i] = 0.;
        tree->SetBranchStatus("*", 0);
        tree->SetBranchStatus("dNdQdY", 1);
        tree->SetBranchAddress("dNdQdY", &dNdQdY);
        for (size_t entry_id = 0; entry_id < tree->GetEntries(); entry_id++) {
            tree->GetEntry(entry_id);
            dNdy[i] += dNdQdY;
        }

        eta_hist->SetBinContent(bin, dNdy[i]);

        delete tree;
    }

    eta_hist->Draw();

    cout << "Number of dimuons : ";
    cout << eta_hist->Integral("width") << endl;

    TFile* out_file = new TFile("../norm/gammastar_norm.root", "RECREATE");
    eta_hist->Write();
    out_file->Close();
}
