# rootAnalyze
Code files to analyze some root file contents and plot them for the MiniCSC Experiment.

README Sections:
  - [MiniCSC Experiment](#minicsc-experiment) 
  - [Coding Requirements](#coding-requirements)
  - [ROOT Installation](#suggested-installation-and-setup-for-root)
  - [Structure of Project](#structure-of-project)
  - [Folders in src](#folders-in-src)
  - [Executing Files](#executing-files)
  - [Structure of root file](#structure-of-root-file)
___
## Wait! Before Use:

###  MiniCSC Experiment 
familiarize yourself with the miniCSC project. The MiniCSC is a small cross-section of a full CSC used for longevity studies relating to gas mixtures. I wont go into detail, but i will link some useful resources to read up on it.
- [Greater CSC Experiment](https://cms.cern/detector/detecting-muons/cathode-strip-chambers)
- [TWiki](https://twiki.cern.ch/twiki/bin/view/CMS/CSCprotoAt904) (Requires a CERN login)
- See the final presentations from previous students
___
### Coding Requirements
- **Bash**
  - Bash is required to interact with the PC via terminal and use the code. You can easily pick it up along the way, but i recommend at least getting an idea of how it works.
  - Bash is not the default shell for windows machines; however, it will still be used for this project on windows machines due to the recommended setup(WSL); See the "Suggested Installation and Setup For Root" section. Additionally, any work in LXPLUS is bash.
    
- **C++**
  - Most of the code in this project is written in C++. It is the most compatible with root. The code is not super advanced c++ work though, so having some coding knowledge in any language is a good start.
 
- **Python**
  - Python is also compatible with root via PYROOT, although most of the code currently in this repo does not use it.
  - jupyter notebooks in python can also be setup with root, which may be useful for some conceptual work; not required by any means.
  - Note that if you download a windows binary of root instead of following the suggested setup, python will not work.
___
### Suggested Installation and Setup For Root
  - **Note: This Installation assumes you are on a windows machine. If not:**
      - Linux Installation will be almost the same. Ignore the WSL Setup.
      - Mac Installation will also be very similar. Ignore the WSL Setup, and be sure to download the correct mac distro of anaconda.
        
  - **STEP ONE: Setup WSL**
      - WSL (Windows Subystem for Linux) allows you to have a linux setup on a windows PC. This is required because ROOT is optimized for linux(and mac) much more so than windows, to the point it barely works.
      - Notes:
        - Since WSL is within the greater Windows OS, you will still have access to your windows files and programs within linux and vise-versa.
          - Files can be moved between each other, code editors can be used within WSL, etc.
        - Link im following for WSL Setup: [WSL](https://learn.microsoft.com/en-us/windows/wsl/install)
      - To Install:
        - Open the command line(use search bar, or press Win+R, type "cmd", and hit enter)
        - type `wsl --install`
        - This will download ubuntu. follow the instructions to setup.
    
  - **STEP TWO: Download Anaconda**
     - Anaconda is a data analysis environment. it comes with python, and ROOT can be download within it.
     - Link im following for anaconda setup: [Anaconda Installation](https://www.hostinger.com/tutorials/how-to-install-anaconda-on-ubuntu/)
     - To Install:
        - Open WSL
        - `sudo apt-get update`
        - `cd /tmp`
        - `apt-get install wget`
        - `wget https://repo.anaconda.com/archive/Anaconda3-2024.10-1-Linux-x86_64.sh` (Copy the link from their website for the newest version)
        - `bash /Anaconda3-2024.10-1-Linux-x86_64.sh` (Change for version you downloaded)
         
  - **STEP THREE: Download ROOT within Anaconda**
    - Link im following: [ROOT Conda](https://anaconda.org/conda-forge/root/)
    - To Install:
        - `conda install conda-forge::root`
            
  - **FINAL**
     - If followed correctly, your system should now be set up to use root within WSL.
     - if interested in using jupyter notebooks, it can be setup within conda.
     - Some extra notes:
        - Remember the password you give at setup for WSL.
        - Create a file structure within WSL (Presumably a Cern folder, etc)
        - `cd /mnt/` or `cd /mnt/c/` will take you to your windows directories within WSL.
        - `cd ~/` will take you to the parent directory within WSL.
___
## Structure of project:
![image](https://github.com/user-attachments/assets/6c8b6a6b-f62f-4391-8892-fdabd23ba0a2)
- plots/
  - Holds the generated plots from each file. I recommend keeping it well organized with descriptive subfolders.
- root_files/
  - Holds the root files generated from the DAQ measurement unpacking and processing. check the [twiki](https://twiki.cern.ch/twiki/bin/view/CMS/DAQminiCSC) starting at "Accessing LXPLUS" for more.
  - The files currently assume every root file is named "output.root". I recommend keeping an organized folder structure(consistent with plots/ organizational structure) to keep track of seperate data.
- src/
  - holds the main files for this repo. check the "Folders in src" section of this readme for more.
- otherCodes/
  - holds quick drafts and outdated code files. These files are generally not set up for systematic use, but may have useful logic to adapt into future code.
___  
## Folders in src
- GeneratePlot
  - Holds files for generic processing of the root files, just giving the standard histogram charts.
  - NOTE: These files only worked for me on lxplus. If you can get them to work locally, i recommend it, as lxplus is slow, and the header file in this folder is easily reusable for other files
    
- Multiplot
  - This folder contains a file for plotting more than one histogram on the same plot. Useful for seeing differences in runs. Currently setup for strip occupancy and charge spectra plots.
    - Can easily be setup for wire occupancy plots as well. I recommend putting any wire plot specific code in utils and calling them, similar to the prepareCathodeChargeHist() and prepareCathodeStripHist() functions.

- Specialized
  - Holds a file for plotting background corrected charge spectra plots. I recommend putting any future unique plotting code in the future in this folder.
 
- Utils
  - Holds the utility functions currently used by MultiPlot and Specialized. if MiniCSCData.h from GeneratePlot works locally, it may be worth combining these code files in some way for consistenncy.
___
## Executing Files
- If root is set up properly:
  - cd into src
  - look for the "Execution" comment on each file. use the command below it in the terminal to execute the file.
___
## Structure of root file:
![image](https://github.com/user-attachments/assets/0677a09c-e902-450e-8ecf-d20144743931)
- This is the inner structure within each root file currently.
___
## Final Notes
- To learn ROOT, check the [ROOT guide](https://root.cern.ch/root/htmldoc/guides/primer/ROOTPrimer.html) and [ROOT docs](https://root.cern/doc/v632/).
- Additional links:
  - [Previous miniCSC Repo](https://github.com/shanepack/miniCSC)
  - [Strip Currents Repo](https://github.com/CollinArbour/StripCurrents)
  - [TMB Dumps Repo](https://github.com/Parker-Tech/CSC_TMB_Dumps)
  - [CodiMD](https://codimd.web.cern.ch/k99vsKCuSymkyYOzjapTpg) (CERN login required)



