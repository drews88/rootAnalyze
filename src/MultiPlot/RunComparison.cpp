#include <TFile.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <vector>
#include <string>
#include "../Utils/HistUtils.h"

/*
    Main file to create a plot comparing multiple runs, currently (kinda) works for spectra and strip plots.
    at the bottom of the file is RunComparison(), which is run upon execution.
    
    Note: for std::strings passed into ROOT functions, use .c_str() to convert to a const char*. root only accepts those.

    Author: Drew Swinson
*/

void multiPlot(std::vector<std::string> dirPaths, std::string filePath, std::vector<std::string> runTitles, std::string title, std::string type, bool save = false, std::string savePath = "./newPlot.pdf") {

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
    gStyle->SetOptStat(0);

    // Canvas setup
    TCanvas *c = new TCanvas("c", "Multiple Histograms", 800, 600);
    c->SetGrid();

    // Legend setup
    TLegend *legend = new TLegend(0.1, 0.7, 0.3, 0.9);
    legend->SetTextSize(0.03);
    legend->SetBorderSize(0);
    legend->SetFillStyle(0);

    // Define a larger set of colors for more histograms(may need to be expanded)
    int colors[] = {kBlue - 9, kRed - 7, kGreen - 7, kOrange - 3, kMagenta - 5, kCyan - 6}; 
    int colorCount = sizeof(colors) / sizeof(colors[0]);

    int runIndex = 0;
    double globalMax = 0;
    std::vector<TH1D*> histograms;

    // for loop to find global max for proper y axis range
    for (const auto &dirPath : dirPaths) {
        std::string fullPath = dirPath + filePath;
        TH1D *hist = nullptr;
        getData(fullPath, hist, type);

        if (type == "Cathode/charge/chargeL3") {
            prepareCathodeChargeHist(hist);
        }

        if (hist) {
            double maxVal = hist->GetMaximum();
            if (maxVal > globalMax)
                globalMax = maxVal;
            histograms.push_back(hist);
        } else {
            std::cerr << "Failed to retrieve histogram from " << fullPath << std::endl;
        }
    }

    // Second loop, to draw histograms
    int colorIndex = 0;
    for (const auto &dirPath : dirPaths) {
        std::string fullPath = dirPath + filePath;
        TH1D *hist = nullptr;
        getData(fullPath, hist, type);

        if (!hist) continue;

        //set spectra-specific requirements
        if (type == "Cathode/charge/chargeL3") {
            prepareCathodeChargeHist(hist);
        }
        else if (type == "Cathode/strip/stripL3") {
            prepareCathodeStripHist(hist);
        }

        // Apply custom styling
        hist->SetLineColor(colors[colorIndex % colorCount]);
        hist->SetFillColorAlpha(colors[colorIndex % colorCount], 0); // Semi-transparent fill
        hist->SetLineWidth(2);

        hist->SetTitle(title.c_str());
        legend->AddEntry(hist, runTitles[runIndex].c_str(), "f");

        // Set consistent y-axis scaling
        hist->SetMaximum(globalMax * 1.4);
        hist->SetMinimum(0);
        if (colorIndex == 0) {
            hist->Draw("HIST");  // "HIST" for histogram style
        } else {
            hist->Draw("HIST SAME");
        }

        runIndex++;
        colorIndex++;  // Increment colorIndex for the next histogram
    }

    legend->Draw();
    c->Update();
    if (save) c->SaveAs(savePath.c_str());
}

void RunComparison() {
    /*
         "../root_files/00_pmx_ArCO2/"                  "Premixed ArCO2"                03-19            
         "../root_files/01_dyn_ArCO2/"                  "Dynamic ArCO2"                 04-09
         "../root_files/02_dyn_freshCF4/"               "Dynamic Fresh CF4"             08-26
         "../root_files/03_dyn_recupCF4/"               "Dynamic Recuperated CF4"       08-27
         "../root_files/03.01_dyn_recupCF4/"            "Dynamic Recuperated CF4"       09-23       ***3500V 
         "../root_files/04_irradTest/"                  "Irradiation Test"              10-25
         "../root_files/05_centralGas_Cd_Scans/"        "Central Gas Cd Scans"          10-31       ***Varying

         "../root_files/new_connector_241113/raw1/"     "Run 0"     
         "../root_files/new_connector_241113/raw2/"     "Run 1"
         "../root_files/new_connector_241113/raw3/"     "Run 2"
         "../root_files/new_connector_241113/raw4/"     "Run 3"         
         "../root_files/new_connector_241113/raw5/"     "Run 4"
         "../root_files/new_connector_241113/raw6/"     "Run 5"
         "../root_files/new_connector_241113/raw7/"     "Run 6"

         "../root_files/old_connector_241112/raw1/"     "Run 0"
         "../root_files/old_connector_241112/raw4/"     "Run 3"
         "../root_files/old_connector_241112/raw5/"     "Run 4"                **ALL 3500V, Dark
         "../root_files/old_connector_241112/raw6/"     "Run 5"
         "../root_files/old_connector_241112/shift1/"   "Shifted Run 0"
         "../root_files/old_connector_241112/shift2/"   "Shifted Run 1" 

         "../root_files/src_new_connect/L0raw1/"        "Left Justified Run 0"
         "../root_files/src_new_connect/L0raw2/"        "Left Justified Run 1"
         "../root_files/src_new_connect/L0raw3/"        "Left Justified Run 2"
         "../root_files/src_new_connect/L1raw1/"        "Left Justified +1 Run 0"   
         "../root_files/src_new_connect/L1raw2/"        "Left Justified +1 Run 1"
         "../root_files/src_new_connect/L1raw3/"        "Left Justified +1 Run 2"            **ALL 3600V, wSrc
         "../root_files/src_new_connect/R0raw1/"        "Right Justified Run 0"
         "../root_files/src_new_connect/R0raw2/"        "Right Justified Run 1"
         "../root_files/src_new_connect/R0raw3/"        "Right Justified Run 2"
         "../root_files/src_new_connect/R1raw1/"        "Right Justified +1 Run 0"
         "../root_files/src_new_connect/R1raw2/"        "Right Justified +1 Run 1"
         "../root_files/src_new_connect/R1raw3/"        "Right Justified +1 Run 2"
                    
                        root -l -e ".L ../Utils/HistUtils.cpp" -e ".x RunComparison.cpp"
    */
    //NOTE: REDO THIS FOR THE OLD CONNECTOR RUNS AGAIN, REMOVE RUN 5, RAW6
    //paths to runs
    std::vector<std::string> dirPaths = {
                                            "../../root_files/pins_rotated/L0raw1/",
                                        };  

    //titles of the runs for the legend
    std::vector<std::string> runTitles = {
                                            "Left Justified Run 0",
                                        };

    //plot title
    std::string title = "Rotated Pins, 109Cd Comparison, New Connector";

    //type of histogram to plot(path to the histogram within the root file)
    //stripL3/L4
    //chargeL3/L4
    //wireL5/L6
    std::string type = "Cathode/strip/stripL3";

    //where to save the plot(if applicable)
    std::string savePath = "../../plots/MultiPlot/newConnector/1rotatedPins/Testdeleteme2.pdf";

    std::string filePath = "output.root";

    multiPlot(dirPaths, filePath, runTitles, title, type, true, savePath);
    cout << "Done" << endl;
}
