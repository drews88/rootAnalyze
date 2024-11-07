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
        * @param dirPaths: The paths to the directories containing the root files
        * @param runTitles: The titles of the runs
        * @param title: The title of the plot
        * @param type: The type of histogram to retrieve
        * @param save: Save the plot to a file
        * @param savePath: The path to save the plot
    */

    //name of all the files(in my setup)
    std::string fileName = "output.root";

    // Set the style of the plot
    gStyle->SetOptStat(0);

    // Create a canvas to draw the histograms
    TCanvas *c = new TCanvas("c", "Multiple Histograms", 800, 600);
    c->SetGrid();

    // Create a legend
    TLegend *legend = new TLegend(0.6, 0.6, 0.9, 0.9);
    legend->SetTextSize(0.025);
    legend->SetMargin(0.1);

    // Color counter
    int colorIndex = 1;

    //used to loop over the run titles for the legend
    int runIndex = 0;

    // find the global maximum for consistent scaling
    double globalMax = 0;

    // First pass to find the global maximum
    for (const auto &dirPath : dirPaths) {
        std::string fullPath = dirPath + fileName;
        TH1F *hist = nullptr;
        getData(fullPath, hist, type);

        if (hist) {
            // Update globalMax if this histogram has a larger maximum
            double maxVal = hist->GetMaximum();
            if (maxVal > globalMax) {
                globalMax = maxVal;
            }
        } else {
            std::cerr << "Failed to retrieve histogram from " << fullPath << std::endl;
        }
    }

    // Second pass to draw histograms with consistent scaling
    for (const auto &dirPath : dirPaths) {

        //define the full path to the file
        std::string fullPath = dirPath + fileName;

        //initialize and define the histogram
        TH1F *hist = nullptr;
        getData(fullPath, hist, type);

        // Check if the histogram was successfully retrieved
        if (!hist) continue;

        if (type == "Cathode/charge/chargeL3") {
            prepareCathodeChargeHist(hist);  
        }

        // Set the histogram style
        hist->SetLineColor(colorIndex);
        hist->SetFillColor(colorIndex);
        hist->SetLineWidth(2);
        hist->SetMarkerStyle(0);

        // Set the histogram title
        hist->SetTitle(title.c_str());

        // Add the legend entry
        legend->AddEntry(hist, runTitles[runIndex].c_str(), "l");

        // Set y-axis range to the global maximum (with some padding)
        hist->SetMaximum(globalMax * 1.1);
        hist->SetMinimum(0);

        // Draw the histogram
        if (colorIndex == 1) {
            hist->Draw();
        } else {
            hist->Draw("SAME");
        }

        //increment run index
        runIndex++;
        colorIndex++;
    }

    // Draw the legend
    legend->Draw();

    // Update and save the canvas
    c->Update();
    if (save) {
        c->SaveAs(savePath.c_str());
    }
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
    std::string title = "Dark Spectra Comparison";

    //type of histogram to plot(path to the histogram within the root file)
    std::string type = "Cathode/strip/stripL3";

    //where to save the plot(if applicable)
    std::string savePath = "../plots/runComp/Spectra/Dark/cleanedComparison.pdf";

    multiPlot(dirPaths, runTitles, title, type);
}
