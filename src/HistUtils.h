#ifndef HISTUTILS_H
#define HISTUTILS_H

#include <TFile.h>
#include <TH1F.h>
#include <string>

/*
    Header file to initialize functions(for c++ reasons)
*/

// Function to retrieve the histogram from the file
void getData(const std::string &flnm, TH1F* &hist, std::string type, bool norm = false);

// Function to prepare the "Cathode/charge/chargeL3" histogram
void prepareCathodeChargeHist(TH1F *hist);

// Function to prepare the "Cathode/strip/stripL3" histogram
//void prepareCathodeStripHist(TH1F* hist);

#endif // HISTOGRAM_UTILS_H
