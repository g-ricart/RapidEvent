#ifndef RAPIDEVENT_RAPIDWRITER_H_
#define RAPIDEVENT_RAPIDWRITER_H_

#include "TFile.h"
#include "TTree.h"
#include "TSTring.h"

#include "RapidConfig.h"
#include "RapidEvent.h"

/*!
 * \class RapidWriter
 * Class used to write the tracks of an event in a ROOT file.
*/

class RapidWriter {

    public:
        // Default constructor
        RapidWriter();
        // Standard constructor
        RapidWriter(TSTring out_file_path);
        // Destructor
        ~RapidWriter();

    private:
        TTree* out_tree_;
        TFile* out_file_;

};

#endif // RAPIDEVENT_RAPIDWRITER_H_
