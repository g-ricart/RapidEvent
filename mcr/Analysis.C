#include <iostream>

#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TH1I.h"
#include "TString.h"
#include "TCanvas.h"

using namespace std;

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

    // Build TTree index
    track_tree->BuildIndex("eventNumber", "trackID");

    // Set branch addresses
    Ssiz_t event_number;
    Size_t track_id, mother_id;
    TString* part_name = new TString();
    Bool_t is_prompt;
    Double_t fd,    fd_true;
    Double_t ip,    ip_true;
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
    track_tree->SetBranchAddress("PX",          &px);
    track_tree->SetBranchAddress("PY",          &py);
    track_tree->SetBranchAddress("PZ",          &pz);
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
    track_tree->SetBranchStatus("PX",          1);
    track_tree->SetBranchStatus("PY",          1);
    track_tree->SetBranchStatus("PZ",          1);
    track_tree->SetBranchStatus("PX_TRUE",     1);
    track_tree->SetBranchStatus("PY_TRUE",     1);
    track_tree->SetBranchStatus("PZ_TRUE",     1);
}
