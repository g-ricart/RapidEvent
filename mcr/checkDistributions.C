#include <iostream>

#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TRatioPlot.h"

#include "lhcbStyle.C"

void checkPions() {

    //lhcbStyle();
    gStyle->SetOptStat(0);

    // Histograms for RapidSim.
    TH1D* pi_pt_h = new TH1D("pi_pt_h", "pions p_{T} comparison", 100, 0, 2);
    pi_pt_h->GetXaxis()->SetTitle("p_{T} (GeV)");
    TH1D* pi_p_h  = new TH1D("pi_p_h",  "pions momentum comparison", 100, 0, 50);
    pi_p_h->GetXaxis()->SetTitle("p (GeV)");
    TH1D* pi_eta_h = new TH1D("pi_eta_h", "pions #eta comparison", 25, 2, 5);
    pi_eta_h->GetXaxis()->SetTitle("#eta");

    // Histograms for RapidEvent.
    TH1D* pi_pt_event_h = new  TH1D("pi_pt_event_h",  "RapidEvent pions pT", 100, 0, 2);
    TH1D* pi_p_event_h  = new  TH1D("pi_p_event_h",   "RapidEvent pions momentum", 100, 0, 50);
    TH1D* pi_eta_event_h = new TH1D("pi_eta_event_h", "RapidEvent pions pseudo-rapidity", 25, 2, 5);

    TFile* pip_file = TFile::Open("../data/pip_tree.root");
    TTree* pip_tree = (TTree*)pip_file->Get("DecayTree");
    pip_tree->SetBranchStatus("*", 0);
    pip_tree->SetBranchStatus("pip_0_PT", 1);
    pip_tree->SetBranchStatus("pip_0_P",  1);
    pip_tree->SetBranchStatus("pip_0_eta",  1);

    Double_t pip_pt = 0;
    Double_t pip_p  = 0;
    Double_t pip_eta = 0;
    pip_tree->SetBranchAddress("pip_0_PT",  &pip_pt);
    pip_tree->SetBranchAddress("pip_0_P",   &pip_p);
    pip_tree->SetBranchAddress("pip_0_eta", &pip_eta);
    for (size_t i = 0; i < pip_tree->GetEntries(); i++) {
        pip_tree->GetEntry(i);
        pi_pt_h->Fill(pip_pt);
        pi_p_h->Fill(pip_p);
        pi_eta_h->Fill(pip_eta);
    }

    pip_file->Close();

    TFile* pim_file = TFile::Open("../data/pim_tree.root");
    TTree* pim_tree = (TTree*)pim_file->Get("DecayTree");
    pim_tree->SetBranchStatus("*", 0);
    pim_tree->SetBranchStatus("pim_0_PT",  1);
    pim_tree->SetBranchStatus("pim_0_P",   1);
    pim_tree->SetBranchStatus("pim_0_eta", 1);

    Double_t pim_pt  = 0;
    Double_t pim_p   = 0;
    Double_t pim_eta = 0;
    pim_tree->SetBranchAddress("pim_0_PT",  &pim_pt);
    pim_tree->SetBranchAddress("pim_0_P",   &pim_p);
    pim_tree->SetBranchAddress("pim_0_eta", &pim_eta);

    for (size_t i = 0; i < pim_tree->GetEntries(); i++) {
        pim_tree->GetEntry(i);
        pi_pt_h->Fill(pim_pt);
        pi_p_h->Fill(pim_p);
        pi_eta_h->Fill(pim_eta);
    }

    pim_file->Close();

    TFile* track_file = TFile::Open("../output/test_tree.root");
    TTree* track_tree = (TTree*)track_file->Get("trackTree");
    track_tree->SetBranchStatus("*",        0);
    track_tree->SetBranchStatus("PT",       1);
    track_tree->SetBranchStatus("P",        1);
    track_tree->SetBranchStatus("eta",      1);
    track_tree->SetBranchStatus("ProbNNpi", 1);

    Double_t pt  = 0;
    Double_t p   = 0;
    Double_t eta = 0;
    Double_t ProbNNpi = 0;
    track_tree->SetBranchAddress("PT",  &pt);
    track_tree->SetBranchAddress("P",   &p);
    track_tree->SetBranchAddress("eta", &eta);
    track_tree->SetBranchAddress("ProbNNpi", &ProbNNpi);

    for (size_t i = 0; i < track_tree->GetEntries(); i++) {
        track_tree->GetEntry(i);
        if (ProbNNpi > 0.8) {
            pi_pt_event_h->Fill(pt);
            pi_p_event_h->Fill(p);
            pi_eta_event_h->Fill(eta);
        }
    }

    track_file->Close();

    // Normalize histograms.
    pi_pt_event_h->Scale(pi_pt_h->GetEntries()/pi_pt_event_h->GetEntries());
    pi_p_event_h->Scale(pi_p_h->GetEntries()/pi_p_event_h->GetEntries());
    pi_eta_event_h->Scale(pi_eta_h->GetEntries()/pi_eta_event_h->GetEntries());

//_______________________________plot___________________________________________

    TCanvas* c1 = new TCanvas("c1", "", 0, 0, 1200, 1000);
    TRatioPlot* pt_plot = new TRatioPlot(pi_pt_h, pi_pt_event_h);
    pt_plot->Draw();
    pt_plot->GetLowerRefYaxis()->SetTitle("ratio");
    pt_plot->GetUpperRefYaxis()->SetTitle("number of tracks / 20 MeV");

    TCanvas* c2 = new TCanvas("c2", "", 0, 0, 1200, 1000);
    TRatioPlot* p_plot = new TRatioPlot(pi_p_h, pi_p_event_h);
    p_plot->Draw();
    p_plot->GetLowerRefYaxis()->SetTitle("ratio");
    p_plot->GetUpperRefYaxis()->SetTitle("number of tracks / 500 MeV");

    TCanvas* c3 = new TCanvas("c3", "", 0, 0, 1200, 1000);
    TRatioPlot* eta_plot = new TRatioPlot(pi_eta_h, pi_eta_event_h);
    eta_plot->Draw();
    eta_plot->GetLowerRefYaxis()->SetTitle("ratio");
    eta_plot->GetUpperRefYaxis()->SetTitle("number of tracks");
}
