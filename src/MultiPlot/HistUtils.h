#ifndef HISTUTILS_H
#define HISTUTILS_H

#include <TFile.h>
#include <TH1F.h>
#include <TH1D.h>
#include <string>

/*
    Header file to initialize functions(for c++ reasons)
*/

// Function to retrieve the histogram from the file
void getData(const std::string &flnm, TH1D* &hist, std::string type);

// Function to prepare the "Cathode/charge/chargeL3" histogram
void prepareCathodeChargeHist(TH1D *hist);

// Function to prepare the "Cathode/strip/stripL3" histogram
void prepareCathodeStripHist(TH1D* hist);

#endif // HISTOGRAM_UTILS_H
