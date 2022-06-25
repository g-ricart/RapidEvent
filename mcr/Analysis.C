#include <iostream>
#include <vector>
#include <map>

#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TH1I.h"
#include "THStack.h"
#include "TString.h"
#include "TCanvas.h"
#include "TLorentzVector.h"
#include "TMath.h"
#include "TTreeIndex.h"
#include "TObjArray.h"
#include "TLegend.h"

using namespace std;

const Double_t kMmu = 0.10565837;

Double_t computeSignificance(TH1* signal_h, vector<TH1*> bkg_vec){

    Double_t n_signal = static_cast<Double_t>(signal_h->GetEntries());
    Double_t n_bkg = 0.;
    for (auto& bkg_h: bkg_vec) {
        n_bkg += static_cast<Double_t>(bkg_h->GetEntries());
    }

    return n_signal / TMath::Sqrt(n_signal + n_bkg);
}

void checkIDs(TString input_file_name) {

    TFile* input_file = new TFile(input_file_name, "READ");
    TTree* track_tree = (TTree*)input_file->Get("trackTree");

    TH1I* id_hist = new TH1I("id_hist", "", 600, 0, 600);

    Size_t track_id;
    track_tree->SetBranchAddress("trackID", &track_id);

    track_tree->SetBranchStatus("*", 0);
    track_tree->SetBranchStatus("trackID", 1);

    size_t n_entries = track_tree->GetEntries();

    for (size_t entry_id = 0; entry_id < n_entries; entry_id++) {
        track_tree->GetEntry(entry_id);
        id_hist->Fill(track_id);
    }

    TCanvas* c1 = new TCanvas("c1", "", 1000, 800);
    id_hist->Draw();
}

void singleMuonBkgAna(TString input_file_name) {

    TFile* input_file = new TFile(input_file_name, "READ");
    TTree* track_tree = (TTree*)input_file->Get("trackTree");

    // Build TTree index
    track_tree->BuildIndex("eventNumber", "trackID");

    // Set branch addresses
    Ssiz_t event_number;
    Size_t track_id, mother_id;
    TString* part_name = new TString();
    Bool_t is_prompt;
    Double_t fd, fd_true;
    Double_t ip, ip_true;
    Double_t p, p_true;
    Double_t pt, pt_true;
    Double_t eta, eta_true;
    Double_t origX, origx_true;
    Double_t origY, origY_true;
    Double_t origZ, origZ_true;
    Double_t probNNk, probNNmu, probNNpi, probNNp;
    track_tree->SetBranchAddress("eventNumber", &event_number);
    track_tree->SetBranchAddress("trackID",     &track_id);
    track_tree->SetBranchAddress("motherID",   &mother_id);
    track_tree->SetBranchAddress("partName",   &part_name);
    track_tree->SetBranchAddress("isPrompt",   &is_prompt);
    track_tree->SetBranchAddress("ProbNNmu",   &probNNmu);

    track_tree->SetBranchStatus("*", 0);
    track_tree->SetBranchStatus("eventNumber", 1);
    track_tree->SetBranchStatus("trackID",     1);
    track_tree->SetBranchStatus("motherID",    1);
    track_tree->SetBranchStatus("partName",    1);
    track_tree->SetBranchStatus("isPrompt",    1);
    track_tree->SetBranchStatus("ProbNNmu",    1);

    Ssiz_t n_muons      = 0;
    Ssiz_t n_signal     = 0;
    Ssiz_t n_background = 0;

    Ssiz_t n_gamma  = 0;
    Ssiz_t n_D0_D0b = 0;

    size_t n_entries = track_tree->GetEntries();

    for (size_t entry_id = 0; entry_id < n_entries; entry_id++) {

        track_tree->GetEntry(entry_id);

        if (part_name->Contains("gamma")) {
            n_gamma++;
            continue;
        }

        if (part_name->Contains("D0")) {
            n_D0_D0b++;
            continue;
        }

        if (probNNmu > 0.8) {

            n_muons++;

            // If track is a muon, get mother track name.
            track_tree->GetEntryWithIndex(event_number, mother_id);

            if (part_name->Contains("gamma")) {
                n_signal++;
                continue;
            }

            if (part_name->Contains("D0")) {
                n_background++;
                continue;
            }

            cout << "wtf" << endl;
            continue;
        }
    }

    cout << endl;
    cout << "----------------------------------------"             << endl;
    cout << "gamma : "      << n_gamma                             << endl;
    cout << "D0 + D0b : "   << n_D0_D0b                            << endl;
    cout << "------------single muon data------------"             << endl;
    cout << "total : "      << n_muons                             << endl;
    cout << "signal : "     << n_signal                            << endl;
    cout << "background : " << n_background                        << endl;
    cout << "ratio : "      << float(n_signal)/float(n_background) << endl;

    input_file->Close();
    delete input_file;
    delete part_name;
}

void muonPairBkgAna(TString input_file_name) {

    TFile* input_file = new TFile(input_file_name, "READ");
    TTree* track_tree = (TTree*)input_file->Get("trackTree");

    // Set branch addresses
    Ssiz_t event_number;
    Size_t track_id, mother_id;
    TString* part_name = new TString();
    Bool_t is_prompt;
    Double_t fd,    fd_true;
    Double_t ip,    ip_true;
    Double_t m,     m_true;
    Double_t p,     p_true;
    Double_t pt,    pt_true;
    Double_t px,    px_true;
    Double_t py,    py_true;
    Double_t pz,    pz_true;
    Double_t eta,   eta_true;
    Double_t origX, origx_true;
    Double_t origY, origY_true;
    Double_t origZ, origZ_true;
    Double_t probNNk, probNNmu, probNNpi, probNNp;
    track_tree->SetBranchAddress("eventNumber", &event_number);
    track_tree->SetBranchAddress("trackID",     &track_id);
    track_tree->SetBranchAddress("motherID",    &mother_id);
    track_tree->SetBranchAddress("partName",    &part_name);
    track_tree->SetBranchAddress("isPrompt",    &is_prompt);
    track_tree->SetBranchAddress("ProbNNmu",    &probNNmu);
    track_tree->SetBranchAddress("M",           &m);
    track_tree->SetBranchAddress("P",           &p);
    track_tree->SetBranchAddress("PX",          &px);
    track_tree->SetBranchAddress("PY",          &py);
    track_tree->SetBranchAddress("PZ",          &pz);
    track_tree->SetBranchAddress("M_TRUE",      &m_true);
    track_tree->SetBranchAddress("P_TRUE",      &p_true);
    track_tree->SetBranchAddress("PX_TRUE",     &px_true);
    track_tree->SetBranchAddress("PY_TRUE",     &py_true);
    track_tree->SetBranchAddress("PZ_TRUE",     &pz_true);

    track_tree->SetBranchStatus("*", 0);
    track_tree->SetBranchStatus("eventNumber", 1);
    track_tree->SetBranchStatus("trackID",     1);
    track_tree->SetBranchStatus("motherID",    1);
    track_tree->SetBranchStatus("partName",    1);
    track_tree->SetBranchStatus("isPrompt",    1);
    track_tree->SetBranchStatus("ProbNNmu",    1);
    track_tree->SetBranchStatus("M",           1);
    track_tree->SetBranchStatus("P",           1);
    track_tree->SetBranchStatus("PX",          1);
    track_tree->SetBranchStatus("PY",          1);
    track_tree->SetBranchStatus("PZ",          1);
    track_tree->SetBranchStatus("M_TRUE",      1);
    track_tree->SetBranchStatus("P_TRUE",      1);
    track_tree->SetBranchStatus("PX_TRUE",     1);
    track_tree->SetBranchStatus("PY_TRUE",     1);
    track_tree->SetBranchStatus("PZ_TRUE",     1);

    THStack* stack = new THStack("stack", "Dimuons invariant mass");

    TH1D* dimu_m_h = new TH1D("dimu_m_h", "", 100, 0, 10); // All dimuons.
    TH1D* dimu_m_sig_h = new TH1D("dimu_m_sig_h", "signal", 100, 0, 10); // Dimuons from same gamma
    dimu_m_sig_h->SetFillColor(kGreen);
    TH1D* dimu_m_sig_comb_h = new TH1D("dimu_m_sig_comb_h", // Dimuons from two different gammas
                                       "signal combinatorial", 100, 0, 10);
    dimu_m_sig_comb_h->SetFillColor(kBlue);
    TH1D* dimu_m_mix_comb_h = new TH1D("dimu_m_mix_comb_h", // Dimuons from gammma + D0/D0b
                                       "mixed combinatorial", 100, 0, 10);
    dimu_m_mix_comb_h->SetFillColor(kRed);
    TH1D* dimu_m_bkg_comb_h = new TH1D("dimu_m_bkg_comb_h", // Dimuons from D0 + D0b
                                       "background combinatorial", 100, 0, 10);
    dimu_m_bkg_comb_h->SetFillColor(kYellow);

    std::vector<Size_t> mup_IDs;
    std::vector<Size_t> mum_IDs;

    size_t n_entries = track_tree->GetEntries();

    // Build TTree index
    track_tree->BuildIndex("eventNumber", "trackID");
    TTreeIndex* index = (TTreeIndex*)track_tree->GetTreeIndex();

    Ssiz_t current_event = 1; // Event numbering starts at 1.

    for (size_t i = 0; i < n_entries; i++) {

        if (i % 10000 == 0) {
            cout << i << "/" << n_entries << "\r";
        }

        // Get entries in event number -> track ID order
        track_tree->GetEntry(index->GetIndex()[i]);

        // Add all mu+ and mu- track IDs of the same event to two
        // different vectors.
        if (event_number == current_event) {
            if (probNNmu > 0.8) {
                if (part_name->Contains("mup")) {
                    mup_IDs.push_back(track_id);
                    continue;
                } else if (part_name->Contains("mum")) {
                    mum_IDs.push_back(track_id);
                    continue;
                } else {
                    cout << "PID error." << endl;
                    continue;
                }
            }
            else { continue; }
        }
        else {
            // Form every possible dimuon pair in the event.
            for(auto mup_ID: mup_IDs) { // Loop on mu+
                // Get entry corresponding to mu+ track.
                track_tree->GetEntryWithIndex(current_event, mup_ID);

                Double_t E_mup = TMath::Sqrt(p*p+kMmu*kMmu); // Compute energy.
                TLorentzVector mup = TLorentzVector();
                mup.SetPxPyPzE(px, py, pz, E_mup);

                // Get mu+ mother name.
                // Get entry corresponding to mu+ mother track.
                track_tree->GetEntryWithIndex(current_event, mother_id);
                TString mup_mother_name = *part_name;

                for(auto mum_ID: mum_IDs) { // Loop on mu-
                    // Get entry corresponding to mu- track.
                    track_tree->GetEntryWithIndex(current_event, mum_ID);

                    Double_t E_mum = TMath::Sqrt(p*p+kMmu*kMmu); // Compute energy.
                    TLorentzVector mum = TLorentzVector();
                    mum.SetPxPyPzE(px, py, pz, E_mum);

                    // Get mu- mother name.
                    // Get entry corresponding to mu- mother track.
                    track_tree->GetEntryWithIndex(current_event, mother_id);
                    TString mum_mother_name = *part_name;

                    // Compute dimuon invariant mass.
                    Double_t dimu_m = (mum + mup).M();

                    // Fill histograms.
                    dimu_m_h->Fill(dimu_m); // All pairs.

                    if (mup_mother_name == mum_mother_name) { // Pure signal.
                        dimu_m_sig_h->Fill(m); // Fill with mother mass.
                        continue;
                    } else {
                        TObjArray* mup_tokens = mup_mother_name.Tokenize("_");
                        TObjArray* mum_tokens = mum_mother_name.Tokenize("_");

                        TString mup_mother = ((TObjString*)mup_tokens->At(0))->GetString();
                        TString mum_mother = ((TObjString*)mum_tokens->At(0))->GetString();

                        if (mup_mother == "gammastar") {
                            if (mum_mother == "gammastar") { // Signal combinatorial.
                                dimu_m_sig_comb_h->Fill(dimu_m);
                                continue;
                            } else if (mum_mother == "D0b") { // Mixed combinatorial.
                                dimu_m_mix_comb_h->Fill(dimu_m);
                                continue;
                            } else { // Shouldn't happen.
                                cout << "wtf_1" << endl;
                                continue;
                            }
                        }
                        else if (mup_mother == "D0")  {
                            if (mum_mother == "gammastar") { // Mixed combinatorial.
                                dimu_m_mix_comb_h->Fill(dimu_m);
                                continue;
                            } else if (mum_mother == "D0b") { // Pure background combinatorial.
                                dimu_m_bkg_comb_h->Fill(dimu_m);
                                continue;
                            } else { // Shouldn't happen.
                                cout << "wtf_2" << endl;
                                continue;
                            }
                        } else { // Shouldn't happen.
                            cout << "wtf_3" << endl;
                            continue;
                        }
                    }
                }
            }
            // Re get base entry to correctly set current event.
            track_tree->GetEntry(index->GetIndex()[i]);
            current_event = event_number;

            // Empty muon vectors.
            mup_IDs.clear();
            mum_IDs.clear();
        }
    }
    cout << endl;

    stack->Add(dimu_m_sig_h);
    stack->Add(dimu_m_sig_comb_h);
    stack->Add(dimu_m_mix_comb_h);
    stack->Add(dimu_m_bkg_comb_h);

    vector<TH1*> bkg_histos;
    bkg_histos.push_back(dimu_m_sig_comb_h);
    bkg_histos.push_back(dimu_m_mix_comb_h);
    bkg_histos.push_back(dimu_m_bkg_comb_h);

    Double_t sig = computeSignificance(dimu_m_sig_h, bkg_histos);

    TCanvas* c1 = new TCanvas("c1", "", 1000, 800);
    dimu_m_h->Draw();

    TCanvas* c2 = new TCanvas("c2", "", 1000, 800);
    stack->Draw();

    TLegend* legend = new TLegend(0.2, 0.2);
    // legend->SetHeader("The Legend Title","C"); // option "C" allows to center the header
    legend->AddEntry(dimu_m_sig_h, "Signal", "f");
    legend->AddEntry(dimu_m_sig_comb_h, "#gamma_{1} + #gamma_{2}", "f");
    legend->AddEntry(dimu_m_mix_comb_h, "#gamma + D^{0}", "f");
    legend->AddEntry(dimu_m_bkg_comb_h, "D^{0} + #bar{D^{0}}", "f");
    legend->SetEntrySeparation(0.4);
    legend->Draw();

    cout << sig << endl;

    c2->Update();

}
