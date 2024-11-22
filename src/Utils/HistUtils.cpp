#include "HistUtils.h"
#include <TFile.h>
#include <TH1F.h>
#include <iostream>

/*
    Helper Functions for Main file(s)
    Ideally, getData uses the reusable code from MiniCSCData.h to retrieve the histogram from the file. however this crashed root every time.
*/
// Function to retrieve the histogram from the file
void getData(const std::string &flnm, TH1D* &hist, std::string type) {
    TFile *mTFile = TFile::Open(flnm.c_str());
    hist = (TH1D*)mTFile->Get(type.c_str());
    
    // Make sure the histogram is found
    if (!hist) {
        std::cerr << "Failed to retrieve histogram: " << type << " from " << flnm << std::endl;
        return;
    }

}

// Function to prepare the "Cathode/charge/chargeL3" histogram
void prepareCathodeChargeHist(TH1D *hist) {
    hist->Rebin(64);                  // Rebinning for Cathode/charge/chargeL3
    hist->GetXaxis()->SetRangeUser(100.5, 4000.5);  // X-axis range for Cathode/charge/chargeL3
}

// Function to prepare the "Cathode/strip/stripL3" histogram
void prepareCathodeStripHist(TH1D* hist) {
        hist->GetXaxis()->SetRangeUser(80, 120);
}
