#include <TFile.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <vector>
#include <string>
#include "HistUtils.h"

/*
    Main file to create a plot comparing multiple runs, currently (kinda) works for spectra and strip plots.
    at the bottom of the file is RunComparison(), which is run upon execution.
*/

void multiPlot(std::vector<std::string> dirPaths, std::vector<std::string> runTitles, std::string title, std::string type, bool save = false, std::string savePath = "./newPlot.pdf") {

     /*
        * Create a canvas and plot multiple histograms on the same canvas
        * This Function is built to require as little modification as possible. ideally only the legend coordinates may need to be adjusted.
        * @param dirPaths: The paths to the directories containing the root files
        * @param runTitles: The titles of the runs
        * @param title: The title of the plot
        * @param type: The type of histogram to retrieve
        * @param save: Save the plot to a file
        * @param savePath: The path to save the plot
    */
    std::string fileName = "output.root";
    gStyle->SetOptStat(0);

    // Canvas setup
    TCanvas *c = new TCanvas("c", "Multiple Histograms", 800, 600);
    c->SetGrid();

    // Legend setup
    TLegend *legend = new TLegend(0.1, 0.7, 0.3, 0.9);  
    legend->SetTextSize(0.03);
    legend->SetBorderSize(0);
    legend->SetFillStyle(0);

    // Define specific colors
    int colors[] = {kBlue - 9, kRed - 7};  // Light blue and light red
    int colorIndex = 0;

    int runIndex = 0;
    double globalMax = 0;
    std::vector<TH1F*> histograms;

    // First pass to find global maximum
    for (const auto &dirPath : dirPaths) {
        std::string fullPath = dirPath + fileName;
        TH1F *hist = nullptr;
        getData(fullPath, hist, type);

        if (hist) {
            double maxVal = hist->GetMaximum();
            if (maxVal > globalMax) globalMax = maxVal;
            histograms.push_back(hist);
        } else {
            std::cerr << "Failed to retrieve histogram from " << fullPath << std::endl;
        }
    }

    // Second pass to draw histograms with consistent scaling
    for (const auto &dirPath : dirPaths) {
        std::string fullPath = dirPath + fileName;
        TH1F *hist = nullptr;
        getData(fullPath, hist, type);

        if (!hist) continue;

        // Apply custom styling
        hist->SetLineColor(colors[colorIndex]);
        hist->SetFillColorAlpha(colors[colorIndex], 0.5); // Semi-transparent fill
        hist->SetLineWidth(2);

        hist->SetTitle(title.c_str());
        legend->AddEntry(hist, runTitles[runIndex].c_str(), "f");

        // Set consistent y-axis scaling
        hist->SetMaximum(globalMax * 1.2);
        hist->SetMinimum(0);

        if (colorIndex == 0) {
            hist->Draw("HIST");  // "HIST" for histogram style
        } else {
            hist->Draw("HIST SAME");
        }

        runIndex++;
        colorIndex = (colorIndex + 1) % 2;  // Alternate between colors
    }

    legend->Draw();
    c->Update();
    if (save) c->SaveAs(savePath.c_str());
}

void RunComparison() {
    /*
         "./root_files/00_pmx_ArCO2/"               "Premixed ArCO2"                03-19            
         "./root_files/01_dyn_ArCO2/"               "Dynamic ArCO2"                 04-09
         "./root_files/02_dyn_freshCF4/"            "Dynamic Fresh CF4"             08-26
         "./root_files/03_dyn_recupCF4/"            "Dynamic Recuperated CF4"       08-27
         "./root_files/03.01_dyn_recupCF4/"         "Dynamic Recuperated CF4"       09-23 
         "./root_files/04_irradTest/"               "Irradiation Test"              10-25
         "./root_files/05_centralGas_Cd_Scans/"     "Central Gas Cd Scans"          10-31
                    
    */

    //paths to runs
    std::vector<std::string> dirPaths = {//"../root_files/00_pmx_ArCO2/dark/",
                                         //"../root_files/01_dyn_ArCO2/dark/",
                                         //"../root_files/02_dyn_freshCF4/dark/raw1/",
                                         "../root_files/03_dyn_recupCF4/dark/raw1/",
                                         "../root_files/03.01_dyn_recupCF4/dark/raw1/",
                                         // "../root_files/04_irradTest/dark/raw1/",
                                         //"../root_files/05_centralGas_Cd_Scans/dark/raw3600/"
                                         };

    //titles of the runs for the legend
    std::vector<std::string> runTitles = {//"Premixed ArCO2",
                                          //"Dynamic ArCO2",
                                          //"Dynamic Fresh CF4",
                                          "Dynamic Recuperated CF4",
                                          "Dynamic Recuperated CF4",
                                          //"Irradiation Test",
                                          //"Central Gas Cd Scans"
                                          };

    //plot title
    std::string title = "Dark Run Strip Comparison";

    //type of histogram to plot(path to the histogram within the root file)
    std::string type = "Cathode/strip/stripL3";

    //where to save the plot(if applicable)
    std::string savePath = "../plots/runComp/strip/03comparisons.pdf";

    multiPlot(dirPaths, runTitles, title, type, true, savePath);
    cout << "Done" << endl;
}
