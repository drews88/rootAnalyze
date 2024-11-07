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

void getData(const std::string &flnm, TH1F* &hist, bool norm = false) {
    /*
        * Get the data from the file and set the histogram
        * to the data in the file.        
        * @param flnm: The file name to open
        * @param hist: The histogram to set
        * @param norm: Normalize the histogram  
    */
    TFile *mTFile = TFile::Open(flnm.c_str());
    hist = (TH1F*)mTFile->Get("Cathode/charge/chargeL3");
    
    hist->Rebin(64);
    hist->GetXaxis()->SetRangeUser(100.5, 4000.5);

    // Normalize?
    if (norm) {
        hist->Scale(1.0 / hist->Integral());
    }
}

void makeText(TF1 *mfit) {
    double x = 0.5;

    TText mTitle(x, 0.7, "Gaussian Fit");
    mTitle.SetNDC();
    TText mMean(x, 0.65, Form("Mean:   %.3f", mfit->GetParameter(1)));
    mMean.SetNDC();
    TText mSigma(0.5, 0.6, Form("Sig:   %.3f", mfit->GetParameter(2)));
    mSigma.SetNDC();

    mTitle.Draw();
    mMean.Draw();
    mSigma.Draw();
}

void runAnalysis() {
    gStyle->SetOptStat(0);

    //Initialize fresh and recuperated TH1F pointers
    TH1F *mfSrc, *mrSrc;

    //get data from root files and assign to tH1F pointers
    getData("./root_files/02_dyn_freshCF4/wSrc/hole2/raw1/output.root", mfSrc);
    getData("./root_files/03_dyn_recupCF4/wSrc/hole2/raw1/output.root", mrSrc);

    //Set the titles and colors of the histograms
    mfSrc->SetTitle("miniCSC4 109Cd Spectrum HV3600 L1 Hole2");
    mfSrc->SetLineColor(600);
    mfSrc->SetFillColorAlpha(600, 0.5);
    mfSrc->SetFillStyle(4001);
    
    mrSrc->SetLineColor(632);
    mrSrc->SetFillColorAlpha(632, 0.4);
    mrSrc->SetFillStyle(4001);

    //Draw the histograms on the same canvas
    TCanvas *prevCanv = new TCanvas("prevCanvas", "prevCanvas", 800, 600);
    mfSrc->Draw();
    mrSrc->Draw("Same");

    //Create a legend and add the histograms to it
    TLegend *legend = new TLegend(0.5, 0.6, 0.9, 0.9);
    legend->SetTextSize(0.04);
    legend->SetHeader("Dynamic Gas Mixing", "C");
    legend->AddEntry((TObject*)nullptr, "40% Ar 55% CO2 5% CF4", "");
    legend->AddEntry(mfSrc, "Fresh CF4", "f");
    legend->AddEntry(mrSrc, "Recuperated CF4", "f");
    legend->SetMargin(0.2);
    legend->Draw();

    //Draw the canvas and save the plot
    prevCanv->Draw();
    prevCanv->SaveAs("./RawResultsComparison.pdf");

    // Single Comparisons
    TH1F *prevBkg, *prevSrc;
    getData("./root_files/03_dyn_recupCF4/dark/raw1/output.root", prevBkg);
    getData("./root_files/03_dyn_recupCF4/wSrc/hole2/raw1/output.root", prevSrc);

    prevSrc->SetTitle("Previous 109Cd Spectrum HV3600 L1 Hole2");
    prevBkg->SetLineColor(2);

    TCanvas *prevCanv2 = new TCanvas("prevCanvas", "prevCanvas", 800, 600);
    prevSrc->Draw();
    prevBkg->Draw("Same");

    TLegend *legend2 = new TLegend(0.6, 0.8, 0.9, 0.9);
    legend2->AddEntry(prevSrc, "Data (Uncorrected)", "l");
    legend2->AddEntry(prevBkg, "Background");
    legend2->Draw();

    prevCanv2->Draw();
    prevCanv2->SaveAs("./RawPrevResultsComparison.pdf");

    // Background Correction
    /*
        I have no clue what is going on in this section of the code.
        idk what the graph is doing
    */
    double pct = 21.5 / 497.2; // (Dark TMB Rate) * (S+B TMB Rate)
    double scaling = pct * prevSrc->Integral();
    prevBkg->Scale(scaling);
    prevSrc->Add(prevBkg, -1);

    prevSrc->SetTitle("Previous 109Cd Spectrum HV3600 L1 Hole2");

    TCanvas *prevCanv3 = new TCanvas("prevCanvas", "prevCanvas", 800, 600);
    prevSrc->Draw("HIST");

    TLegend *legend3 = new TLegend(0.6, 0.8, 0.9, 0.9);
    legend3->AddEntry(prevSrc, "Data (Corrected)", "l");
    legend3->Draw();

    prevCanv3->Draw();
    prevCanv3->SaveAs("./PrevResultsCorrected.pdf");

    /*
        This section is also messed up
    */
    // Using Fresh CF4
    TH1F *mBkg, *mSrc;
    getData("./root_files/02_dyn_freshCF4/dark/raw1/output.root", mBkg);
    getData("./root_files/02_dyn_freshCF4/wSrc/hole2/raw1/output.root", mSrc);

    double mpct = 139.3 / 597.9; // (Dark TMB Rate) * (S+B TMB Rate)
    double mScaling = mpct * mSrc->Integral();
    std::cout << mScaling << std::endl;
    mBkg->Scale(mScaling);
    mSrc->Add(mBkg, -1);

    mSrc->SetTitle("109Cd Spectrum HV3600 L1 Hole2");

    TCanvas *cCanv = new TCanvas("cCanvas", "cCanvas", 800, 600);
    mSrc->SetTitle("109Cd Charge Spectra HV3600 L1 Hole2");
    mSrc->Draw("HIST");

    TLegend *legend4 = new TLegend(0.6, 0.8, 0.9, 0.9);
    legend4->AddEntry(mSrc, "Data (Corrected)", "l");
    legend4->Draw();

    cCanv->Draw();
    cCanv->SaveAs("./mBackgroundCorrected.pdf");


    mSrc->Add(mBkg, -1);
    getData("./root_files/02_dyn_freshCF4/dark/raw1/output.root", mBkg);
    getData("./root_files/02_dyn_freshCF4/wSrc/hole2/raw1/output.root", mSrc);

    mSrc->SetTitle("109Cd Charge Spectra HV3600 L1 Hole2");
    mBkg->SetLineColor(2);
    
    TCanvas *cCanv2 = new TCanvas("cCanvas", "cCanvas", 800, 600);
    mSrc->Draw("HIST");
    mBkg->Draw("Same");

    TLegend *legend5 = new TLegend(0.6, 0.8, 0.9, 0.9);
    legend5->AddEntry(mSrc, "Data (Uncorrected)", "l");
    legend5->AddEntry(mBkg, "Background", "l");
    legend5->Draw();

    cCanv2->Draw();
    cCanv2->SaveAs("./RawSBandSComparison.pdf");

}

int Comparing() {
    runAnalysis();
    return 0;
}
