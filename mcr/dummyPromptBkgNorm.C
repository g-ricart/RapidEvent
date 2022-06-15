#include "TFile.h"
#include "TH1D.h"

void dummyPromptBkgNorm() {

    // Normalisation histograms for pions, kaons and protons at mid-rapidity.
    TH1D* pip_hist = new TH1D("pip", "", 1, -0.5, 0.5);
    TH1D* pim_hist = new TH1D("pim", "", 1, -0.5, 0.5);
    TH1D* Kp_hist  = new TH1D("Kp",  "", 1, -0.5, 0.5);
    TH1D* Km_hist  = new TH1D("Km",  "", 1, -0.5, 0.5);
    TH1D* pp_hist  = new TH1D("pp",  "", 1, -0.5, 0.5);
    TH1D* pm_hist  = new TH1D("pm",  "", 1, -0.5, 0.5);

    // ALICE measurments at mid-rapidity (0-5% + 5-10% in centrality).
    // Divide by two to account for charges.
    Double_t pi_N = (1699.80 + 1377.49)/2.;
    Double_t k_N  = (273.41 + 222.48)/2.;
    Double_t p_N  = (74.56 + 61.51)/2.;

    pip_hist->SetBinContent(1, pi_N);
    pim_hist->SetBinContent(1, pi_N);

    Kp_hist->SetBinContent(1, k_N);
    Km_hist->SetBinContent(1, k_N);

    pp_hist->SetBinContent(1, p_N);
    pm_hist->SetBinContent(1, p_N);

    TFile* pip_file = new TFile("pip_norm.root", "RECREATE");

    pip_hist->Write();
    pip_file->Close();

    TFile* pim_file = new TFile("pim_norm.root", "RECREATE");

    pim_hist->Write();
    pim_file->Close();

    TFile* Kp_file = new TFile("Kp_norm.root", "RECREATE");

    Kp_hist->Write();
    Kp_file->Close();

    TFile* Km_file = new TFile("Km_norm.root", "RECREATE");

    Km_hist->Write();
    Km_file->Close();

    TFile* pp_file = new TFile("pp_norm.root", "RECREATE");

    pp_hist->Write();
    pp_file->Close();

    TFile* pm_file = new TFile("pm_norm.root", "RECREATE");

    pm_hist->Write();
    pm_file->Close();
}
