#include <iostream>

#include "TFile.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TMath.h"

void promptBckgNorm() {

    // Normalisation histograms for pions, kaons and protons.
    // Binning in ALICE paper is 0.25 unit per bin.
    TH1D* pip_hist = new TH1D("pip", "", 34, -3.5, 5);
    TH1D* pim_hist = new TH1D("pim", "", 34, -3.5, 5);
    TH1D* Kp_hist  = new TH1D("Kp",  "", 34, -3.5, 5);
    TH1D* Km_hist  = new TH1D("Km",  "", 34, -3.5, 5);
    TH1D* pp_hist  = new TH1D("pp",  "", 34, -3.5, 5);
    TH1D* pm_hist  = new TH1D("pm",  "", 34, -3.5, 5);

    // ALICE measurements at mid-rapidity (0-5% + 5-10% centrality).
    // Not accouting for charges (should divide everything by 2).
    Double_t pi_N = (1699.80 + 1377.49)/2.;
    Double_t k_N  = (273.41 + 222.48)/2.;
    Double_t p_N  = (74.56 + 61.51)/2.;

    cout << "---- ALICE measurements (dN/dy) ----" << endl;
    cout << "Number of pions : "          << pi_N << endl;
    cout << "Number of kaons : "          << k_N  << endl;
    cout << "Number of (anti)protons  : " << p_N << endl;

    // Mean pT (GeV)
    Double_t mean_pi_pt = (0.5682 + 0.5711)/2.;
    Double_t mean_k_pt  = (0.9177 + 0.9214)/2.;
    Double_t mean_p_pt  = (1.4482 + 1.4334)/2.;

    // Masses (GeV)
    Double_t m_pi = 0.139570;
    Double_t m_k  = 0.493677;
    Double_t m_p  = 0.938272;

    // Convert dN/dy to dN/deta
    pi_N *= TMath::Sqrt( 1 - TMath::Power(m_pi / TMath::Sqrt(TMath::Power(mean_pi_pt, 2) + TMath::Power(m_pi, 2)*TMath::CosH(0)), 2) );
    k_N  *= TMath::Sqrt( 1 - TMath::Power(m_k  / TMath::Sqrt(TMath::Power(mean_k_pt,  2) + TMath::Power(m_k,  2)*TMath::CosH(0)), 2) );
    p_N  *= TMath::Sqrt( 1 - TMath::Power(m_p  / TMath::Sqrt(TMath::Power(mean_p_pt,  2) + TMath::Power(m_p,  2)*TMath::CosH(0)), 2) );

    cout << "--- ALICE measurements (dN/deta) ---" << endl;
    cout << "Number of pions : "          << pi_N << endl;
    cout << "Number of kaons : "          << k_N  << endl;
    cout << "Number of (anti)protons  : " << p_N << endl;

    TFile* density_file = TFile::Open("../norm/chargedPartDensity.root");
    // 0-5% centrality
    TH1F* density_hist_1 = (TH1F*)density_file->Get("DN-DETARAP/Hist1D_y1");
    // 5-10% centrality
    TH1F* density_hist_2 = (TH1F*)density_file->Get("DN-DETARAP/Hist1D_y2");

    // 0-10% centrality
    TH1F* density_hist = new TH1F("densisty_h", "", 34, -3.5, 5);
    density_hist_1->Scale(0.5);
    density_hist_2->Scale(0.5);
    density_hist->Add(density_hist_1, density_hist_2);

    // Integrate in range -0.5 < eta < +0.5
    Double_t mid_N = density_hist->Integral(13, 16, "width");

    // Loop over bins corresponding to 2 <= eta < 5.
    for (size_t bin = 23; bin <= 34; bin++) {

        // Get density in the bin.
        Double_t fwd_N = density_hist->GetBinContent(bin);

        // Boost number of particles to forward rapidity.
        Double_t boost_N = fwd_N / mid_N;
        pip_hist->SetBinContent(bin, (pi_N/2.) * boost_N);
        pim_hist->SetBinContent(bin, (pi_N/2.) * boost_N);
        Kp_hist->SetBinContent(bin,  (k_N/2.)  * boost_N);
        Km_hist->SetBinContent(bin,  (k_N/2.)  * boost_N);
        pp_hist->SetBinContent(bin,  (p_N/2.)  * boost_N);
        pm_hist->SetBinContent(bin,  (p_N/2.)  * boost_N);
    }

    cout << "------------- Boosted -------------" << endl;
    cout << "Number of pions : "          << pip_hist->Integral("width") +  pim_hist->Integral("width") << endl;
    cout << "Number of kaons : "          << Kp_hist->Integral("width")  +  Km_hist->Integral("width")  << endl;
    cout << "Number of (anti)protons  : " << pp_hist->Integral("width")  +  pm_hist->Integral("width") << endl;

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
