#include "RapidWriter.h"

#include "TString.h"

using namespace std;

//______________________________________________________________________________
RapidWriter::RapidWriter()
{
    out_tree_ = nullptr;
    out_file_ = nullptr;
}

//______________________________________________________________________________
RapidWriter::RapidWriter(TString out_file_path)
{
    out_file_ = new TFile();
    out_tree_ = new TTree();
}

//______________________________________________________________________________
RapidWriter::~RapidWriter()
{
    delete out_tree_;
    delete out_file_;
}
