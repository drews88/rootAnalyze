#include <iostream>
#include <vector>
#include <TFile.h>
#include <TH1.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TText.h>
#include <TF1.h>
#include <TStyle.h>
#include <TTree.h>
#include "../Utils/HistUtils.h"

/*

File to make Background Corrected Charge Spectra Plots

root -l -e ".L ../Utils/HistUtils.cpp" -e ".x PlotBackGroundCorrected.cpp"

*/

void PlotBackGroundCorrected() {

    std::string type = "Cathode/charge/chargeL3";
    TH1D *mBkg, *mSrc;
    getData("../../root_files/241121_measurements/dark/run2/output.root", mBkg, type);
    getData("../../root_files/241121_measurements/wSrc/run2/output.root", mSrc, type);

    std::cout << "Data Loaded" << std::endl;
    mBkg->Rebin(64);
    mBkg->GetXaxis()->SetRangeUser(100.5, 4000.5);

    mSrc->Rebin(64);
    mSrc->GetXaxis()->SetRangeUser(100.5, 4000.5);

    mBkg->Scale(1.0 / mBkg->Integral());

    double mpct = 51.0 / 414.5; // (Dark TMB Rate) * (S+B TMB Rate)
    double mScaling = mpct * mSrc->Integral();
    std::cout << mScaling << std::endl;
    mBkg->Scale(mScaling);
    mSrc->Add(mBkg, -1);

    std::cout << "Starting Plotting" << std::endl;
    TCanvas *cCanv = new TCanvas("cCanvas", "cCanvas", 800, 600);
    mSrc->SetTitle("109Cd Charge Spectra HV3600 L1 Hole2, Background Corrected");
    mSrc->Draw("HIST");

    TLegend *legend4 = new TLegend(0.6, 0.8, 0.9, 0.9);
    legend4->AddEntry(mSrc, "Data (Corrected)", "l");
    legend4->Draw();

    gStyle->SetOptStat(0);

    cCanv->Draw();
    cCanv->SaveAs("../../plots/Specialized/BackgroundCorrected/241121run2.pdf");
}