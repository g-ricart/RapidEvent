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
#include "TText.h"
#include "TStyle.h"

#include "setStyle.h"

using namespace std;

const Double_t kMmu  = 0.10565837;
const Double_t kMKch = 0.49368;
const Double_t kMD0  = 1.86484;

const Double_t kMinDimuMass = 1.;
const Double_t kMaxDimuMass = 5.;

const Double_t kMinP = 3.;

Double_t computeSigBkgRatio(TH1* signal_h, vector<TH1*> bkg_vec){

    Double_t n_signal = static_cast<Double_t>(signal_h->GetEntries());
    Double_t n_bkg = 0.;
    for (auto& bkg_h: bkg_vec) {
        n_bkg += static_cast<Double_t>(bkg_h->GetEntries());
    }

    return n_signal / n_bkg;
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

    Int_t T2KstyleIndex = 2;
    // Official T2K style as described in http://www.t2k.org/comm/pubboard/style/index_html
    TString localStyleName = "T2K";
    // -- WhichStyle --
    // 1 = presentation large fonts
    // 2 = presentation small fonts
    // 3 = publication/paper
    Int_t localWhichStyle = T2KstyleIndex;

    TStyle* t2kstyle = T2K().SetT2KStyle(localWhichStyle, localStyleName);
    gROOT->SetStyle(t2kstyle->GetName());
    gROOT->ForceStyle();

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

    TH1D* dimu_m_h = new TH1D("dimu_m_h", "", 100, kMinDimuMass, kMaxDimuMass); // All dimuons.
    TH1D* dimu_m_sig_h = new TH1D("dimu_m_sig_h", "signal", 100, kMinDimuMass, kMaxDimuMass); // Dimuons from same gamma
    dimu_m_sig_h->SetFillColor(kGreen);
    TH1D* dimu_m_sig_comb_h = new TH1D("dimu_m_sig_comb_h", // Dimuons from two different gammas
                                       "signal combinatorial", 100, kMinDimuMass, kMaxDimuMass);
    dimu_m_sig_comb_h->SetFillColor(kBlue);
    TH1D* dimu_m_mix_comb_h = new TH1D("dimu_m_mix_comb_h", // Dimuons from gammma + D0/D0b
                                       "mixed combinatorial", 100, kMinDimuMass, kMaxDimuMass);
    dimu_m_mix_comb_h->SetFillColor(kRed);
    TH1D* dimu_m_bkg_comb_h = new TH1D("dimu_m_bkg_comb_h", // Dimuons from D0 + D0b
                                       "background combinatorial", 100, kMinDimuMass, kMaxDimuMass);
    dimu_m_bkg_comb_h->SetFillColor(kYellow);

    vector<Size_t> mup_IDs;
    vector<Size_t> mum_IDs;

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
                if (p < kMinP) { continue; } // Cut on mu P for PID.
                else if (part_name->Contains("mup")) {
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
                    if (mup_mother_name == mum_mother_name) { // Pure signal.
                        // Select pairs with 1 GeV < M < 5 GeV only.
                        if (m < kMinDimuMass || m > kMaxDimuMass) {
                            continue;
                        }
                        dimu_m_sig_h->Fill(m); // Fill with mother mass.
                        dimu_m_h->Fill(m); // All pairs.
                        continue;
                    } else {
                        // Select pairs with 1 GeV < M < 5 GeV only.
                        if (dimu_m < kMinDimuMass || dimu_m > kMaxDimuMass) {
                            continue;
                        }
                        dimu_m_h->Fill(dimu_m); // All pairs.

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

    stack->SetMinimum(0.5);

    vector<TH1*> bkg_histos;
    bkg_histos.push_back(dimu_m_sig_comb_h);
    bkg_histos.push_back(dimu_m_mix_comb_h);
    bkg_histos.push_back(dimu_m_bkg_comb_h);

    Double_t sig = computeSigBkgRatio(dimu_m_sig_h, bkg_histos);

    TCanvas* c2 = new TCanvas("c2", "", 1000, 800);
    gPad->SetLogy();
    stack->Draw();

    stack->GetXaxis()->SetTitle("M_{#mu^{+} #mu^{-}}");

    TLegend* legend = new TLegend(0.65, 0.65, 0.84, 0.84);
    // legend->SetHeader("The Legend Title","C"); // option "C" allows to center the header
    legend->AddEntry(dimu_m_sig_h, "Signal", "f");
    legend->AddEntry(dimu_m_sig_comb_h, "#gamma_{1} + #gamma_{2}", "f");
    legend->AddEntry(dimu_m_mix_comb_h, "#gamma + D^{0}", "f");
    legend->AddEntry(dimu_m_bkg_comb_h, "D^{0} + #bar{D^{0}}", "f");
    legend->SetEntrySeparation(0.4);
    legend->Draw();

    TLatex* cut_txt = new TLatex();
    cut_txt->SetTextSize(0.03);
    cut_txt->DrawLatex(0.66, 0.55, TString::Format("p_{#mu} < %g GeV", kMinP));

    cout << sig << endl;

    c2->Update();
}

void vtxTagAna(TString input_file_name) {

    Int_t T2KstyleIndex = 2;
    // Official T2K style as described in http://www.t2k.org/comm/pubboard/style/index_html
    TString localStyleName = "T2K";
    // -- WhichStyle --
    // 1 = presentation large fonts
    // 2 = presentation small fonts
    // 3 = publication/paper
    Int_t localWhichStyle = T2KstyleIndex;

    TStyle* t2kstyle = T2K().SetT2KStyle(localWhichStyle, localStyleName);
    gROOT->SetStyle(t2kstyle->GetName());
    gROOT->ForceStyle();

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
    track_tree->SetBranchAddress("ProbNNK",     &probNNk);
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
    track_tree->SetBranchStatus("ProbNNK",     1);
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

    // Histos for K-mu pairs.
    TH1D* true_D0_m = new TH1D("true_D0_m", "", 100, 0, 10);
    true_D0_m->SetFillColor(kGreen);
    TH1D* rejected_D0_m = new TH1D("rejected_D0", "", 100, 0, 10);
    // rejected_D0->SetFillColor(kBlue);
    TH1D* cor_rejected_D0_m = new TH1D("cor_rejected_D0_m", "", 100, 0, 10);
    cor_rejected_D0_m->SetFillColor(kGreen);
    cor_rejected_D0_m->SetFillStyle(3354);
    TH1D* misrejected_D0_m = new TH1D("misrejected_D0_m", "", 100, 0, 10);
    misrejected_D0_m->SetFillColor(kRed);
    TH1D* tagged_D0_m = new TH1D("tagged_D0_m", "", 100, 0, 10);
    // tagged_D0_m->SetFillColor(kYellow);
    TH1D* mistagged_D0_m = new TH1D("mistagged_D0_m", "", 100, 0, 10);
    mistagged_D0_m->SetFillColor(kYellow);

    // Histos for dimuons.
    TH1D* dimu_m_h = new TH1D("dimu_m_h", "", 100, kMinDimuMass, kMaxDimuMass); // All dimuons.
    TH1D* dimu_m_sig_h = new TH1D("dimu_m_sig_h", "signal", 100, kMinDimuMass, kMaxDimuMass); // Dimuons from same gamma
    dimu_m_sig_h->SetFillColor(kGreen);
    TH1D* dimu_m_sig_comb_h = new TH1D("dimu_m_sig_comb_h", // Dimuons from two different gammas
                                       "signal combinatorial", 100, kMinDimuMass, kMaxDimuMass);
    dimu_m_sig_comb_h->SetFillColor(kBlue);
    TH1D* dimu_m_mix_comb_h = new TH1D("dimu_m_mix_comb_h", // Dimuons from gammma + D0/D0b
                                       "mixed combinatorial", 100, kMinDimuMass, kMaxDimuMass);
    dimu_m_mix_comb_h->SetFillColor(kRed);
    TH1D* dimu_m_bkg_comb_h = new TH1D("dimu_m_bkg_comb_h", // Dimuons from D0 + D0b
                                       "background combinatorial", 100, kMinDimuMass, kMaxDimuMass);
    dimu_m_bkg_comb_h->SetFillColor(kYellow);

    vector<Size_t> mup_IDs;
    vector<Size_t> mum_IDs;
    vector<Size_t> Kp_IDs;
    vector<Size_t> Km_IDs;

    vector<Size_t> kept_mup_IDs;
    vector<Size_t> kept_mum_IDs;

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

        // Add all mu+, mu-, K+ and K- track IDs of the same event to four
        // different vectors.
        if (event_number == current_event) {
            if (p < kMinP) {continue;} // Cut on momentum for PID.
            if (probNNmu > 0.8) {
                if (part_name->Contains("mup")) {
                    mup_IDs.push_back(track_id);
                    continue;
                } else if (part_name->Contains("mum")) {
                    mum_IDs.push_back(track_id);
                    continue;
                } else {
                    cout << "Muon PID error." << endl;
                    continue;
                }
            } else if (probNNk > 0.8) {
                if (part_name->Contains("Kp")) {
                    Kp_IDs.push_back(track_id);
                    continue;
                } else if (part_name->Contains("Km")) {
                    Km_IDs.push_back(track_id);
                    continue;
                } else {
                    cout << "Kaon PID error." << endl;
                    continue;
                }
            } else { continue; }
        }
        // -------- K-MU ANALYSIS --------
        else {
            // Form every mu+K- pair.
            for (auto mup_ID: mup_IDs) {
                Bool_t is_kept = false;
                // Get entry corresponding to mu+ track.
                track_tree->GetEntryWithIndex(current_event, mup_ID);

                Double_t E_mup = TMath::Sqrt(p*p+kMmu*kMmu); // Compute energy.
                TLorentzVector mup = TLorentzVector();
                mup.SetPxPyPzE(px, py, pz, E_mup);

                // Get mu+ mother name.
                // Get entry corresponding to mu+ mother track.
                track_tree->GetEntryWithIndex(current_event, mother_id);
                TString mup_mother_name = *part_name;

                for (auto Km_ID: Km_IDs) {
                    // Get entry corresponding to K- track.
                    track_tree->GetEntryWithIndex(current_event, Km_ID);

                    Double_t E_Km = TMath::Sqrt(p*p+kMKch*kMKch); // Compute energy.
                    TLorentzVector Km = TLorentzVector();
                    Km.SetPxPyPzE(px, py, pz, E_Km);

                    Double_t mup_Km_m = (mup + Km).M();

                    // Get K- mother name.
                    // Get entry corresponding to K- mother track.
                    track_tree->GetEntryWithIndex(current_event, mother_id);
                    TString Km_mother_name = *part_name;

                    // Replace pair mass by mother mass when both particles
                    // have the same mother (problem with rapidsim ?)
                    // Still ok because RapidSim reconstruct mother mass
                    // without the neutrino.
                    if (Km_mother_name == mup_mother_name) {
                        mup_Km_m = m;
                    }

                    if (mup_Km_m > kMD0) {
                        rejected_D0_m->Fill(mup_Km_m);

                        if (Km_mother_name == mup_mother_name) { // Same mother = true D0.
                            misrejected_D0_m->Fill(mup_Km_m); // Fill with mother mass.
                            continue;
                        } else {
                            cor_rejected_D0_m->Fill(mup_Km_m);
                            continue;
                        }
                    } else {
                        tagged_D0_m->Fill(mup_Km_m);
                        is_kept = false;

                        if (Km_mother_name == mup_mother_name) { // Same mother = true D0.
                            true_D0_m->Fill(mup_Km_m); // Fill with mother mass.
                            continue;
                        } else {
                            mistagged_D0_m->Fill(mup_Km_m);
                            continue;
                        }
                    }
                }
                if (is_kept) {
                    kept_mup_IDs.push_back(mup_ID);
                }
            }
            // Form every mu-K+ pair.
            for (auto mum_ID: mum_IDs) {
                Bool_t is_kept = false;
                // Get entry corresponding to mu- track.
                track_tree->GetEntryWithIndex(current_event, mum_ID);

                Double_t E_mum = TMath::Sqrt(p*p+kMmu*kMmu); // Compute energy.
                TLorentzVector mum = TLorentzVector();
                mum.SetPxPyPzE(px, py, pz, E_mum);

                // Get mu- mother name.
                // Get entry corresponding to mu- mother track.
                track_tree->GetEntryWithIndex(current_event, mother_id);
                TString mum_mother_name = *part_name;

                for (auto Kp_ID: Kp_IDs) {
                    // Get entry corresponding to K+ track.
                    track_tree->GetEntryWithIndex(current_event, Kp_ID);

                    Double_t E_Kp = TMath::Sqrt(p*p+kMKch*kMKch); // Compute energy.
                    TLorentzVector Kp = TLorentzVector();
                    Kp.SetPxPyPzE(px, py, pz, E_Kp);

                    Double_t mum_Kp_m = (mum + Kp).M();

                    // Get K+ mother name.
                    // Get entry corresponding to K+ mother track.
                    track_tree->GetEntryWithIndex(current_event, mother_id);
                    TString Kp_mother_name = *part_name;

                    // Replace pair mass by mother mass when both particles
                    // have the same mother (problem with rapidsim ?)
                    // Still ok because RapidSim reconstruct mother mass
                    // without the neutrino.
                    if (Kp_mother_name == mum_mother_name) {
                        mum_Kp_m = m;
                    }

                    if (mum_Kp_m > kMD0) {
                        rejected_D0_m->Fill(mum_Kp_m);

                        if (Kp_mother_name == mum_mother_name) { // Same mother = true D0.
                            misrejected_D0_m->Fill(mum_Kp_m); // Fill with mother mass.
                            continue;
                        } else {
                            cor_rejected_D0_m->Fill(mum_Kp_m);
                            continue;
                        }
                    } else {
                        tagged_D0_m->Fill(mum_Kp_m);
                        is_kept = false;

                        if (Kp_mother_name == mum_mother_name) { // Same mother = true D0.
                            true_D0_m->Fill(mum_Kp_m);
                            continue;
                        } else {
                            mistagged_D0_m->Fill(mum_Kp_m);
                            continue;
                        }
                    }
                }
                if (is_kept) {
                    kept_mum_IDs.push_back(mum_ID);
                }
            }

            // ------- DIMU ANALYSIS -------
            // Form every possible dimuon pair in the event.
            for(auto mup_ID: kept_mup_IDs) { // Loop on mu+
                // Get entry corresponding to mu+ track.
                track_tree->GetEntryWithIndex(current_event, mup_ID);

                Double_t E_mup = TMath::Sqrt(p*p+kMmu*kMmu); // Compute energy.
                TLorentzVector mup = TLorentzVector();
                mup.SetPxPyPzE(px, py, pz, E_mup);

                // Get mu+ mother name.
                // Get entry corresponding to mu+ mother track.
                track_tree->GetEntryWithIndex(current_event, mother_id);
                TString mup_mother_name = *part_name;

                for(auto mum_ID: kept_mum_IDs) { // Loop on mu-
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
                    if (mup_mother_name == mum_mother_name) { // Pure signal.
                        // Select pairs with 1 GeV < M < 5 GeV only.
                        if (m < kMinDimuMass || m > kMaxDimuMass) {
                            continue;
                        }
                        dimu_m_sig_h->Fill(m); // Fill with mother mass.
                        dimu_m_h->Fill(m); // All pairs.
                        continue;
                    } else {
                        // Select pairs with 1 GeV < M < 5 GeV only.
                        if (dimu_m < kMinDimuMass || dimu_m > kMaxDimuMass) {
                            continue;
                        }
                        dimu_m_h->Fill(dimu_m); // All pairs.

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

            // Empty track id vectors.
            mup_IDs.clear();
            mum_IDs.clear();
            Kp_IDs.clear();
            Km_IDs.clear();

            kept_mum_IDs.clear();
            kept_mup_IDs.clear();
        }
    }
    cout << endl;

    Double_t n_tagged    = static_cast<Double_t>(tagged_D0_m->GetEntries());
    Double_t n_true      = static_cast<Double_t>(true_D0_m->GetEntries());
    Double_t n_mistagged = static_cast<Double_t>(mistagged_D0_m->GetEntries());

    Double_t efficiency = n_tagged / (n_true + n_mistagged);
    Double_t purity     = n_true   / (n_true + n_mistagged);

    THStack* all_D0 = new THStack("all_D0", "K#mu invariant mass distribution");
    // all_D0->SetMinimum(0.7);
    // all_D0->SetMaximum(1e4);

    all_D0->Add(true_D0_m);
    all_D0->Add(misrejected_D0_m);
    all_D0->Add(cor_rejected_D0_m);
    all_D0->Add(mistagged_D0_m);

    all_D0->SetMinimum(0.5);

    TLegend* legend_Kmu = new TLegend(0.55, 0.7, 0.84, 0.85);
    legend_Kmu->AddEntry(true_D0_m,         "Correctly tagged",   "f");
    legend_Kmu->AddEntry(cor_rejected_D0_m, "Correctly rejected", "f");
    legend_Kmu->AddEntry(misrejected_D0_m,  "Mis-rejected",       "f");
    legend_Kmu->AddEntry(mistagged_D0_m,    "Mis-tagged",         "f");

    TText* purity_txt = new TText();
    purity_txt->SetTextSize(0.03);
    TText* efficiency_txt = new TText();
    efficiency_txt->SetTextSize(0.03);

    TCanvas* c1 = new TCanvas("c1", "", 1000, 800);
    gPad->SetLogy();
    all_D0->Draw();
    all_D0->GetXaxis()->SetTitle("M_{K#mu} (GeV)");
    legend_Kmu->Draw();
    purity_txt->DrawTextNDC(0.60,     0.50, Form("Purity: %g",     purity));
    efficiency_txt->DrawTextNDC(0.60, 0.45, Form("Efficiency: %g", efficiency));

    TLatex* cutmu_txt = new TLatex();
    cutmu_txt->SetTextSize(0.03);
    cutmu_txt->SetNDC();
    cutmu_txt->DrawLatex(0.60, 0.40, TString::Format("p_{#mu} < %g GeV", kMinP));

    TLatex* cutK_txt = new TLatex();
    cutK_txt->SetTextSize(0.03);
    cutK_txt->SetNDC();
    cutK_txt->DrawLatex(0.60, 0.35, TString::Format("p_{K} < %g GeV", kMinP));

    THStack* stack = new THStack("stack", "Dimuons invariant mass");

    stack->Add(dimu_m_sig_h);
    stack->Add(dimu_m_sig_comb_h);
    stack->Add(dimu_m_mix_comb_h);
    stack->Add(dimu_m_bkg_comb_h);

    vector<TH1*> bkg_histos;
    bkg_histos.push_back(dimu_m_sig_comb_h);
    bkg_histos.push_back(dimu_m_mix_comb_h);
    bkg_histos.push_back(dimu_m_bkg_comb_h);

    Double_t sig = computeSigBkgRatio(dimu_m_sig_h, bkg_histos);

    TCanvas* c2 = new TCanvas("c2", "", 1000, 800);
    dimu_m_h->Draw();

    TCanvas* c3 = new TCanvas("c3", "", 1000, 800);
    stack->Draw();

    TLegend* legend_dimu = new TLegend(0.2, 0.2);
    // legend->SetHeader("The Legend Title","C"); // option "C" allows to center the header
    legend_dimu->AddEntry(dimu_m_sig_h, "Signal", "f");
    legend_dimu->AddEntry(dimu_m_sig_comb_h, "#gamma_{1} + #gamma_{2}", "f");
    legend_dimu->AddEntry(dimu_m_mix_comb_h, "#gamma + D^{0}", "f");
    legend_dimu->AddEntry(dimu_m_bkg_comb_h, "D^{0} + #bar{D^{0}}", "f");
    legend_dimu->SetEntrySeparation(0.4);
    legend_dimu->Draw();

    cout << sig << endl;

    c3->Update();
}
