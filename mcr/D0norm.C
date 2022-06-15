#include "TTree.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TFile.h"

void buildD0norm() {
    auto eta_tree = new TTree();
    eta_tree->ReadFile("FONLL_eta_c_5TeV.csv", "eta/D:s");

    auto D0_hist = new TH1D("D0", "", 301, 2, 5);

    Double_t eta = 0.;
    Double_t sigma = 0.;

    eta_tree->SetBranchAddress("eta", &eta);
    eta_tree->SetBranchAddress("s",   &sigma);

    for (size_t entry_index = 0; entry_index < eta_tree->GetEntries(); entry_index++) {
        eta_tree->GetEntry(entry_index);
        D0_hist->Fill(eta, sigma);
    }

    delete eta_tree;

    Double_t pb          = 1e-12;        // Data is in pb.
    Double_t inv_mb      = 1e3;          // T_AB is in mb^-1.
    Double_t D0_frag     = 0.391;        // D0 fragmentation fraction.
    Double_t decay_gamma = 0.0341;       // Partial width of D0 -> K- mu+ numu decay.
    Double_t T_PbPb_0_10 = 23.26*inv_mb; // Nuclear overlap function for the
                                         // 10% most central Pb-Pb collisions.

    D0_hist->Scale(pb*D0_frag*decay_gamma*T_PbPb_0_10);

    D0_hist->Draw();

    cout << "Number of D0 -> K- mu+ numu : ";
    cout << D0_hist->Integral("width") << endl;

    TFile* D0_file = new TFile("D0_norm.root", "RECREATE");

    D0_hist->Write();
    D0_file->Close();
}
