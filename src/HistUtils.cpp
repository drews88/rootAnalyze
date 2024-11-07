#include "HistUtils.h"
#include <TFile.h>
#include <TH1F.h>
#include <iostream>

// Function to retrieve the histogram from the file
void getData(const std::string &flnm, TH1F* &hist, std::string type, bool norm) {
    TFile *mTFile = TFile::Open(flnm.c_str());
    hist = (TH1F*)mTFile->Get(type.c_str());
    
    // Make sure the histogram is found
    if (!hist) {
        std::cerr << "Failed to retrieve histogram: " << type << " from " << flnm << std::endl;
        return;
    }

    // Normalize the histogram if requested
    if (norm) {
        hist->Scale(1.0 / hist->Integral());
    }
}

// Function to prepare the "Cathode/charge/chargeL3" histogram

void prepareCathodeChargeHist(TH1F *hist) {
    hist->Rebin(64);                  // Rebinning for Cathode/charge/chargeL3
    hist->GetXaxis()->SetRangeUser(100.5, 4000.5);  // X-axis range for Cathode/charge/chargeL3
}

// Function to prepare the "Cathode/strip/stripL3" histogram
//void prepareCathodeStripHist(TH1F* hist) {
//        //not currently used
//}
