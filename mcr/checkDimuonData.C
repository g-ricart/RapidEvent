#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TLorentzVector.h"

using namespace std;

const Double_t kMmu = 0.10565837;
const Double_t kC   = 299792458;

void checkDimuons(TString input_file_name) {
    TFile* input_file = new TFile(input_file_name, "READ");
    TTree* decay_tree = (TTree*)input_file->Get("DecayTree");

    TH1D* dimu_m_h = new TH1D("dimu_m_h", "", 100, 0, 10);

    Double_t mup_px, mup_py, mup_pz, mup_p, mup_e;
    Double_t mum_px, mum_py, mum_pz, mum_p, mum_e;

    decay_tree->SetBranchAddress("mup_0_E",  &mup_e);
    decay_tree->SetBranchAddress("mup_0_P",  &mup_p);
    decay_tree->SetBranchAddress("mup_0_PX", &mup_px);
    decay_tree->SetBranchAddress("mup_0_PY", &mup_py);
    decay_tree->SetBranchAddress("mup_0_PZ", &mup_pz);

    decay_tree->SetBranchAddress("mum_0_E",  &mum_e);
    decay_tree->SetBranchAddress("mum_0_P",  &mum_p);
    decay_tree->SetBranchAddress("mum_0_PX", &mum_px);
    decay_tree->SetBranchAddress("mum_0_PY", &mum_py);
    decay_tree->SetBranchAddress("mum_0_PZ", &mum_pz);

    decay_tree->SetBranchStatus("*", 0);
    decay_tree->SetBranchStatus("mup_0_E",  1);
    decay_tree->SetBranchStatus("mup_0_P",  1);
    decay_tree->SetBranchStatus("mup_0_PX", 1);
    decay_tree->SetBranchStatus("mup_0_PY", 1);
    decay_tree->SetBranchStatus("mup_0_PZ", 1);

    decay_tree->SetBranchStatus("mum_0_E",  1);
    decay_tree->SetBranchStatus("mum_0_P",  1);
    decay_tree->SetBranchStatus("mum_0_PX", 1);
    decay_tree->SetBranchStatus("mum_0_PY", 1);
    decay_tree->SetBranchStatus("mum_0_PZ", 1);

    size_t n_entries = decay_tree->GetEntries();

    for (size_t i = 0; i < n_entries; i++) {
        // Double_t E_mup = TMath::Sqrt(mup_p*mup_p+kMmu*kMmu); // Compute energy.
        TLorentzVector mup = TLorentzVector();
        mup.SetPxPyPzE(mup_px, mup_py, mup_pz, mup_e);

        // Double_t E_mum = TMath::Sqrt(mum_p*mum_p+kMmu*kMmu); // Compute energy.
        TLorentzVector mum = TLorentzVector();
        mum.SetPxPyPzE(mum_px, mum_py, mum_pz, mum_e);

        // Compute dimuon invariant mass.
        Double_t dimu_m = (mum + mup).M();

        // Fill histogram.
        dimu_m_h->Fill(dimu_m);
    }

    dimu_m_h->Draw();
}
