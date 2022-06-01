#include <iostream>

#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TRatioPlot.h"

#include "lhcbStyle.C"

void checkPionPt() {

    //lhcbStyle();
    gStyle->SetOptStat(0);

    // Histogram for rapidsim pions.
    TH1D* pi_pt_h = new TH1D("pi_pt_h", "pions pT comparison", 500, 0, 2);

    // Histogram for rapidevent pions.
    TH1D* pi_pt_event_h = new TH1D("pi_pt_event_h", "RapidEvent pions pT", 500, 0, 2);

//_______________________________ pions ________________________________________
    TFile* pip_file = TFile::Open("../data/pip_tree.root");
    TTree* pip_tree = (TTree*)pip_file->Get("DecayTree");
    pip_tree->SetBranchStatus("*", 0);
    pip_tree->SetBranchStatus("pip_0_PT", 1);

    Double_t pip_pt = 0;
    pip_tree->SetBranchAddress("pip_0_PT", &pip_pt);
    for (size_t i = 0; i < pip_tree->GetEntries(); i++) {
        pip_tree->GetEntry(i);
        pi_pt_h->Fill(pip_pt);
    }

    pip_file->Close();

    TFile* pim_file = TFile::Open("../data/pim_tree.root");
    TTree* pim_tree = (TTree*)pim_file->Get("DecayTree");
    pim_tree->SetBranchStatus("*", 0);
    pim_tree->SetBranchStatus("pim_0_PT", 1);

    Double_t pim_pt = 0;
    pim_tree->SetBranchAddress("pim_0_PT", &pim_pt);
    for (size_t i = 0; i < pim_tree->GetEntries(); i++) {
        pim_tree->GetEntry(i);
        pi_pt_h->Fill(pim_pt);
    }

    pim_file->Close();

    TFile* track_file = TFile::Open("../output/incorrectPrompts.root");
    TTree* track_tree = (TTree*)track_file->Get("trackTree");
    track_tree->SetBranchStatus("*", 0);
    track_tree->SetBranchStatus("PT", 1);
    track_tree->SetBranchStatus("ProbNNpi", 1);

    Double_t pt = 0;
    Double_t ProbNNpi = 0;
    track_tree->SetBranchAddress("PT", &pt);
    track_tree->SetBranchAddress("ProbNNpi", &ProbNNpi);

    for (size_t i = 0; i < track_tree->GetEntries(); i++) {
        track_tree->GetEntry(i);
        if (ProbNNpi > 0.8) {
            pi_pt_event_h->Fill(pt);
        }
    }

    track_file->Close();

//_______________________________plot___________________________________________

    // Normalize histograms.
    pi_pt_event_h->Scale(pi_pt_h->GetEntries()/pi_pt_event_h->GetEntries());
    TCanvas* c1 = new TCanvas("c1", "", 0, 0, 1200, 1000);
    TRatioPlot* pi_plot = new TRatioPlot(pi_pt_h, pi_pt_event_h);
    pi_plot->Draw();
    pi_plot->GetLowerRefYaxis()->SetTitle("ratio");
    pi_plot->GetUpperRefYaxis()->SetTitle("number of tracks / 20 MeV");
}
