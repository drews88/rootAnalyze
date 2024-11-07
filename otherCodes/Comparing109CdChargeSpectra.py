#!/usr/bin/env python
# coding: utf-8

# In[1]:


import numpy as np
import matplotlib.pyplot as plt
import uproot
import ROOT
import mplhep as hep
plt.style.use(hep.style.CMS)
ROOT.gStyle.SetOptStat(0)


# In[ ]:


mfl['Cathode/strip/stripL3']


# In[2]:


def getData(flnm,norm=False):
    mTFile = ROOT.TFile.Open(flnm)
    mCS = mTFile.Get('Cathode/charge/chargeL3')
    
    mCS.Rebin(64)
    mCS.GetXaxis().SetRangeUser(100.5,4000.5)
    
    # Normalize?
    if norm:
        mCS.Scale(1./mCS.Integral())
    
    return mCS


# In[3]:


def makeText(mfit):
    x = 0.5
    
    mTitle = ROOT.TText(x,0.7,'Gaussian Fit')
    mTitle.SetNDC()
    mMean = ROOT.TText(x,0.65,f'Mean:   {mfit.GetParameter(1):.3f}')
    mMean.SetNDC()
    mSigma = ROOT.TText(0.5,0.6,f'Sig:   {mfit.GetParameter(2):.3f}')
    mSigma.SetNDC()
    
    return [mTitle,mMean,mSigma]


# # All three

# In[4]:


# prevSrc = getData('../data/Recreating/prevCdSrc.root')
mfSrc = getData('../data/Recreating/mfSingle_output.root')
mrSrc = getData('../data/Recreating/mrSingle_output.root')


# In[23]:


# prevSrc.SetTitle('miniCSC4 109Cd Spectrum HV3600 L1 Hole2')
# prevSrc.SetLineColor(921)
# prevSrc.SetFillColorAlpha(921,0.8)
# prevSrc.SetFillStyle(4001)

mfSrc.SetTitle('miniCSC4 109Cd Spectrum HV3600 L1 Hole2')
mfSrc.SetLineColor(600)
mfSrc.SetFillColorAlpha(600,0.5)
mfSrc.SetFillStyle(4001)

mrSrc.SetLineColor(632)
mrSrc.SetFillColorAlpha(632,0.4)
mrSrc.SetFillStyle(4001)

prevCanv = ROOT.TCanvas('prevCanvas','prevCanvas",800,600')
# prevSrc.Draw()
# mfSrc.Draw("Same")
mfSrc.Draw()
mrSrc.Draw("Same")

legend = ROOT.TLegend(0.5,0.6,0.9,0.9)
legend.SetTextSize(0.04)
legend.SetHeader("Dynamic Gas Mixing", "C")
legend.AddEntry(0,"40% Ar, 55% CO2, 5% CF4","")
# legend.AddEntry(0,"40% Ar: 55% CO2: 5% CF4","")
# legend.AddEntry(prevSrc,"Premixed 40% Ar : 60% CO2","l")
legend.AddEntry(mfSrc,"Fresh CF4")
legend.AddEntry(mrSrc,"Recuperated CF4")
legend.SetMargin(0.2)

legend.Draw()

prevCanv.Draw()
prevCanv.SaveAs('./RawResultsComparison.pdf')


# In[ ]:





# In[ ]:





# In[ ]:





# # Single Comparisons

# In[5]:


prevBkg = getData('../data/Recreating/prevNoSrc.root')
prevSrc = getData('../data/Recreating/prevCdSrc.root')


# In[63]:


prevSrc.SetTitle('Previous 109Cd Spectrum HV3600 L1 Hole2')
prevBkg.SetLineColor(2)

prevCanv = ROOT.TCanvas('prevCanvas','prevCanvas",800,600')
prevSrc.Draw()
prevBkg.Draw("Same")

legend = ROOT.TLegend(0.6,0.8,0.9,0.9)
legend.AddEntry(prevSrc,"Data (Unorrected)","l")
legend.AddEntry(prevBkg,"Background")
legend.Draw()

prevCanv.Draw()
prevCanv.SaveAs('./RawPrevResultsComparison.pdf')


# In[49]:


pct = 21.5/497.2 # (Dark TMB Rate) * (S+B TMB Rate)
scaling = pct * prevSrc.Integral()
prevBkg.Scale(scaling)

prevSrc.Add(prevBkg,-1)


# In[51]:


prevSrc.SetTitle('Previous 109Cd Spectrum HV3600 L1 Hole2')

prevCanv = ROOT.TCanvas('prevCanvas','prevCanvas",800,600')
prevSrc.Draw("HIST")

legend = ROOT.TLegend(0.6,0.8,0.9,0.9)
legend.AddEntry(prevSrc,"Data (Corrected)","l")
legend.Draw()

prevCanv.Draw()
prevCanv.SaveAs('./PrevResultsCorrected.pdf')


# In[6]:


# Using Fresh CF4
mBkg = getData('../data/Recreating/mfSingleDark_output.root',norm=True)
mSrc = getData('../data/Recreating/mfSingle_output.root')


# # Subtracting Background from Uncorrected Data
# 
# Dark TMB Rate: 139.3 Hz
# Data TMB Rate: 597.9 Hz
# 
# $ScaleFactor = (139.3 Hz) / (597.9 Hz)$
# 
# Total number of Data events = 2994 Events
# Expected number of Background events is: \
#     $nBkgEvents = ScaleFactor * nDataEvents = 0.233 * 2994 = 698 Events$
# 
# Scale Normalized Background spectrum by nBkgEvents 
# 
# Subtract this expected Background distribution from the Data spectrum

# In[53]:


mpct = 139.3/597.9 # (Dark TMB Rate) * (S+B TMB Rate)
mscaling = mpct * mSrc.Integral()
print(mscaling)
mBkg.Scale(mscaling)

mSrc.Add(mBkg,-1)


# In[54]:


mSrc.SetTitle('109Cd Spectrum HV3600 L1 Hole2')

cCanv = ROOT.TCanvas('cCanvas','cCanvas",800,600')
mSrc.SetTitle("109Cd Charge Spectra HV3600 L1 Hole2")
mSrc.Draw("HIST")

legend = ROOT.TLegend(0.6,0.8,0.9,0.9)
legend.AddEntry(mSrc,"Data (Corrected)","l")
legend.Draw()

cCanv.Draw()
cCanv.SaveAs('./mBackgroundCorrected.pdf')


# In[22]:


mSrc.Add(mBkg,-1)


# In[55]:


# Using Fresh CF4
mBkg = getData('../data/Recreating/mfSingleDark_output.root')
mSrc = getData('../data/Recreating/mfSingle_output.root')


# In[56]:


mSrc.SetTitle('109Cd Spectrum HV3600 L1 Hole2')
mBkg.SetLineColor(2)

mCanv = ROOT.TCanvas('mCanvas','mCanvas",800,600')
mSrc.Draw("HIST")
mBkg.Draw("Same")

legend = ROOT.TLegend(0.6,0.8,0.9,0.9)
legend.AddEntry(mSrc,"Data (Uncorrected)","l")
legend.AddEntry(mBkg,"Background","l")
legend.Draw()

mCanv.Draw()
mCanv.SaveAs('./RawSBandSComparison.pdf')


# In[ ]:





# In[ ]:





# In[ ]:





# In[ ]:





# In[ ]:





# In[23]:


freshgas_flnm = '../data/GasGain/freshCF4/allHoles.root'
freshgas_nsrc_flnm = '../data/GasGain/freshCF4/noSrc.root'
recupgas_flnm = '../data/GasGain/recupCF4/allHoles.root'
recupgas_nsrc_flnm = '../data/GasGain/recupCF4/noSrc.root'


# In[24]:


fcsr = ROOT.TFile.Open(freshgas_flnm)
nsfcsr = ROOT.TFile.Open(freshgas_nsrc_flnm)
rcsr = ROOT.TFile.Open(recupgas_flnm)
nsrcsr = ROOT.TFile.Open(recupgas_nsrc_flnm)


# In[25]:


fl3 = fcsr.Get('Cathode/charge/chargeL3')
nfl3 = nsfcsr.Get('Cathode/charge/chargeL3')
rl3 = rcsr.Get('Cathode/charge/chargeL3')
nrl3 = nsrcsr.Get('Cathode/charge/chargeL3')

# Rebin ---------------------------------------------
fl3r = fl3.Rebin(32)
nfl3r = nfl3.Rebin(32)
rl3r = rl3.Rebin(32)
nrl3r = nrl3.Rebin(32)

# Restrict Axis -------------------------------------
fl3r.GetXaxis().SetRangeUser(100.5,4000.5)
nfl3r.GetXaxis().SetRangeUser(100.5,4000.5)
rl3r.GetXaxis().SetRangeUser(100.5,4000.5)
nrl3r.GetXaxis().SetRangeUser(100.5,4000.5)

# ---------------------------------------------------
# Normalize the background Distribution
nfl3r.Scale(1./nfl3r.Integral())

# Scale the normalized background distribution by expected number of events:
tot_evts = fl3r.Integral()
expected_nb_evts = (0.326) * tot_evts

print(f'Total Events Fresh CF4: {tot_evts}')
print(f'Expected BEvts in Fresh CF4: {expected_nb_evts}')

nfl3r.Scale(expected_nb_evts)


# In[294]:


# fl3r.Scale(1./fl3r.Integral())
# nfl3r.Scale(1./nfl3r.Integral())
# rl3r.Scale(1./rl3r.Integral())


# # Bkg Corrected Fresh CF4 (Cleaned up)

# In[8]:


mfit = ROOT.TF1("mFit","gaus(0)",1000,2100)
mfit.SetParameters(1000,1500,250)
fl3r.Fit("mFit","R")

mtitle = ROOT.TText(0.5,0.7,'Gaussian Fit')
mtitle.SetNDC()
mMean = ROOT.TText(0.5,0.65,f'Mean:   {mfit.GetParameter(1):.3f}')
mMean.SetNDC()
mSigma = ROOT.TText(0.5,0.6,f'Sig:   {mfit.GetParameter(2):.3f}')
mSigma.SetNDC()

mcanv = ROOT.TCanvas('mCanvas','mCanvas",800,600')
fl3r.Draw()
mfit.Draw("Same")
mtitle.Draw()
mMean.Draw()
mSigma.Draw()

mcanv.Draw()


# # Comparing Mine and previous Run

# In[7]:


mfiles = ['../data/Recreating/mfSingle_output.root',\
         '../data/Recreating/mfSingleDark_output.root',\
         '../data/Recreating/prevCdSrc.root',\
         '../data/Recreating/prevNoSrc.root']

mTFiles = []

for fl in mfiles:
    mTFiles.append(ROOT.TFile.Open(fl))

mCS = []
for tfl in mTFiles:
    mCS.append(tfl.Get('Cathode/charge/chargeL3'))


# Process
for cs in mCS:
    cs.Rebin(64)
    cs.GetXaxis().SetRangeUser(100.5,4000.5)


# In[8]:


mf_fit = ROOT.TF1("mfSingleFit","gaus(0)",1000,2200)
o_fit = ROOT.TF1("oSingleFit","gaus(0)",900,2000)

mf_fit.SetParameters(140,1500,250)
o_fit.SetParameters(190,1500,250)

mf_fit.SetLineColor(1)
o_fit.SetLineColor(2)

mCS[0].Fit("mfSingleFit","R")
mCS[2].Fit("oSingleFit","R")

mf_text = ROOT.TText(0.5,0.7,f'mMu: {mf_fit.GetParameter(1):.2f} mSig: {mf_fit.GetParameter(2):.2f}')
mf_text.SetNDC()

o_text = ROOT.TText(0.5,0.65,f'oldMu: {o_fit.GetParameter(1):.2f} oldSig: {o_fit.GetParameter(2):.2f}')
o_text.SetNDC()


# In[9]:


# My 109Cd sample
mCS[0].SetLineColor(1)

# Old 109Cd sample
mCS[2].SetLineColor(2)
mCS[2].SetTitle('MiniCSC4 109Cd Charge Spectrum L1 H2 HV3600')


comp_canv = ROOT.TCanvas('ComparisonCanvas','ComparisonCanvas",800,600')
mCS[2].Draw()
mCS[0].Draw("Same")

# mf_fit.Draw()
# o_fit.Draw()

legend = ROOT.TLegend(0.6,0.8,0.9,0.9)
legend.AddEntry(mCS[0],"Dynamic 40Ar/55CO2/05CF4","l")
legend.AddEntry(mCS[2],"PreMixed 40Ar/60CO2","l")
legend.Draw()


# mf_text.Draw()
# o_text.Draw()

comp_canv.Draw()
comp_canv.SaveAs('./new_vs_old_noFit.pdf')


# In[ ]:





# In[ ]:





# In[ ]:





# In[18]:





# In[17]:


mnbcanv = ROOT.TCanvas('mnbCanvas','mnbCanvas",800,600')
nfl3r.Draw()

mnbcanv.Draw()


# In[26]:


fl3r.Add(nfl3r,-1)

bscanv = ROOT.TCanvas('bsCanvas','bsCanvas",800,600')
fl3r.Draw()

bscanv.Draw()


# In[ ]:





# # Just Hole 2

# In[ ]:


fh2_flnm = '../data/GasGain/freshCF4/hole2.root'
fd_flnm = '../data/GasGain/freshCF4/Hole2.root'


# In[ ]:





# In[ ]:





# In[ ]:





# # Fitting two peaks,  fresh CF4

# In[7]:


bfit = ROOT.TF1("background","landau(0)")
bfit.SetParameters(500,350,50)
nfl3r.Fit("background","S")


# In[345]:


ttext_title = ROOT.TText(0.5,0.7,'Background Landau Fit')
ttext_title.SetNDC()

ttext_mpv = ROOT.TText(0.5,0.65,f'MPV:   {bfit.GetParameter(1):.3f}')
ttext_mpv.SetNDC()

ttext_sig = ROOT.TText(0.5,0.6,f'Sig:   {bfit.GetParameter(2):.3f}')
ttext_sig.SetNDC()


# In[347]:


bcanv = ROOT.TCanvas('BackgroundCanvas','Background",800,600')
nfl3r.Draw()
bfit.Draw("Same")
# tbox.Draw("Same") 
ttext_title.Draw()
ttext_mpv.Draw()
ttext_sig.Draw()


bcanv.Draw()
bcanv.SaveAs('./FreshCF4_BackgroundFit.pdf')


# In[308]:


bmpv = bfit.GetParameter(1)
bsigma = bfit.GetParameter(2)
print(f'MPV: {bmpv}\tSigma: {bsigma}')


# In[348]:


nbtf = ROOT.TF1("Total","landau(0)+landau(3)")
nbtf.SetParameters(550,300,100,1000,1600,250)
fl3r.Fit("Total","S")


# In[386]:


nbtf = ROOT.TF1("gausTotal","gaus(0)+landau(3)")
nbtf.SetParameters(550,300,100,1000,1600,250)
fl3r.Fit("gausTotal","S")


# In[387]:


x=0.6

ttext_title = ROOT.TText(x,0.7,'Bkg+Sig Landau Fit')
ttext_title.SetNDC()

ttext_mpv1 = ROOT.TText(x,0.65,f'peak 1, MPV:   {nbtf.GetParameter(1):.3f}')
ttext_mpv1.SetNDC()

ttext_sig1 = ROOT.TText(x,0.6,f'peak 1, Sig:   {nbtf.GetParameter(2):.3f}')
ttext_sig1.SetNDC()

ttext_mpv2 = ROOT.TText(x,0.55,f'peak 2, MPV:   {nbtf.GetParameter(4):.3f}')
ttext_mpv2.SetNDC()

ttext_sig2 = ROOT.TText(x,0.5,f'peak 2, Sig:   {nbtf.GetParameter(5):.3f}')
ttext_sig2.SetNDC()


# In[388]:


mcanv = ROOT.TCanvas('SBCanvas','SBCanvas",800,600')
fl3r.Draw()
nbtf.Draw("Same")

ttext_title.Draw()
ttext_mpv1.Draw()
ttext_sig1.Draw()
ttext_mpv2.Draw()
ttext_sig2.Draw()

mcanv.Draw()
# mcanv.SaveAs("./FreshCF4_109Cd_uncorrected.pdf")


# ## Trying to perform simultaneous fit with background and two peaks:

# In[395]:


tfwb = ROOT.TF1("TotalBackground","landau(0)+landau(3)+landau(6)")
tfwb.SetParameters(550,300,100,1000,1600,250,100)
tfwb.FixParameter(7,bmpv)
tfwb.FixParameter(8,bsigma)
tfwb.SetParLimits(1,50,500)
tfwb.SetParLimits(4,900,2400)

fl3r.Fit("TotalBackground","S")


# In[396]:


mcanv = ROOT.TCanvas('newCanvas','New Canvas",800,600')
fl3r.Draw()
tfwb.Draw("Same")

mcanv.Draw()


# In[314]:


pk1evts = tfwb.GetParameter(0)
pk2evts = tfwb.GetParameter(3)
pkbkg = tfwb.GetParameter(6)

pk1pos = tfwb.GetParameter(1)
pk2pos = tfwb.GetParameter(4)
bkgpos = tfwb.GetParameter(7)


# In[315]:


print(f'Expected Peak 1 Events: {pk1evts:.2f} at {pk1pos:.2f}')
print(f'Expected Peak 2 Events: {pk2evts:.2f} at {pk2pos:.2f}')
print(f'Expected Backgr Events: {pkbkg:.2f} at {bkgpos:.2f}')


# ## Fitting just main  peak with Landau

# In[7]:


onepk = ROOT.TF1("mainPeak","landau(0)",1100,3000)
onepk.SetParameters(1000,1600,250)
fl3r.Fit("mainPeak","R")


# In[8]:


x=0.6

ttext_title = ROOT.TText(x,0.7,'Landau Fit, main peak only')
ttext_title.SetNDC()

ttext_mpv1 = ROOT.TText(x,0.65,f'MPV:   {onepk.GetParameter(1):.3f}')
ttext_mpv1.SetNDC()

ttext_sig1 = ROOT.TText(x,0.6,f'Sig:   {onepk.GetParameter(2):.3f}')
ttext_sig1.SetNDC()

mcanv = ROOT.TCanvas('SBoP','SBoPCanvas",800,600')
fl3r.Draw()
onepk.Draw("Same")

ttext_title.Draw()
ttext_mpv1.Draw()
ttext_sig1.Draw()

mcanv.Draw()
# mcanv.SaveAs("./FreshCF4_109Cd_uncorrected_mainPeak.pdf")


# # Recuperated CF4 Analysis

# In[8]:


rbfit = ROOT.TF1("rbackground","landau(0)")
rbfit.SetParameters(230,400,50)
nrl3r.Fit("rbackground","S")


# In[12]:


rttext_title = ROOT.TText(0.5,0.7,'Recup Bkg Landau Fit')
rttext_title.SetNDC()

rttext_mpv = ROOT.TText(0.5,0.65,f'MPV:   {rbfit.GetParameter(1):.3f}')
rttext_mpv.SetNDC()

rttext_sig = ROOT.TText(0.5,0.6,f'Sig:   {rbfit.GetParameter(2):.3f}')
rttext_sig.SetNDC()


# In[13]:


rbcanv = ROOT.TCanvas('recupBackgroundCanvas','recupBackground",800,600')
nrl3r.Draw()
rbfit.Draw("Same")
rttext_title.Draw()
rttext_mpv.Draw()
rttext_sig.Draw()


rbcanv.Draw()
rbcanv.SaveAs('./RecupCF4_BackgroundFit.pdf')


# In[ ]:





# In[ ]:





# In[367]:


rnbtf = ROOT.TF1("rTotal","landau(0)+landau(3)")
rnbtf.SetParameters(300,350,100,1000,1750,250)
rl3r.Fit("rTotal","S")


# In[372]:


x=0.6

rttext_title = ROOT.TText(x,0.7,'Recup. Bkg+Sig Landau Fit')
rttext_title.SetNDC()

rttext_mpv1 = ROOT.TText(x,0.65,f'peak 1, MPV:   {rnbtf.GetParameter(1):.3f}')
rttext_mpv1.SetNDC()

rttext_sig1 = ROOT.TText(x,0.6,f'peak 1, Sig:   {rnbtf.GetParameter(2):.3f}')
rttext_sig1.SetNDC()

rttext_mpv2 = ROOT.TText(x,0.55,f'peak 2, MPV:   {rnbtf.GetParameter(4):.3f}')
rttext_mpv2.SetNDC()

rttext_sig2 = ROOT.TText(x,0.5,f'peak 2, Sig:   {rnbtf.GetParameter(5):.3f}')
rttext_sig2.SetNDC()


# In[375]:


mrcanv = ROOT.TCanvas('rSBCanvas','rSBCanvas",800,600')
rl3r.Draw()
rnbtf.Draw("Same")

rttext_title.Draw()
rttext_mpv1.Draw()
rttext_sig1.Draw()
rttext_mpv2.Draw()
rttext_sig2.Draw()

mrcanv.Draw()
mrcanv.SaveAs("./RecupCF4_109Cd_uncorrected.pdf")


# ## Just the Main Peak

# In[18]:


ronepk = ROOT.TF1("rmainPeak","landau(0)",1200,3000)
ronepk.SetParameters(1000,1650,200)
rl3r.Fit("rmainPeak","R")


# In[21]:


x=0.6

ttext_title = ROOT.TText(x,0.7,'Landau Fit, main peak only')
ttext_title.SetNDC()

ttext_mpv1 = ROOT.TText(x,0.65,f'MPV:   {ronepk.GetParameter(1):.3f}')
ttext_mpv1.SetNDC()

ttext_sig1 = ROOT.TText(x,0.6,f'Sig:   {ronepk.GetParameter(2):.3f}')
ttext_sig1.SetNDC()

rmcanv = ROOT.TCanvas('rSBoP','rSBoPCanvas',800,600)
rl3r.Draw()
ronepk.Draw("Same")

ttext_title.Draw()
ttext_mpv1.Draw()
ttext_sig1.Draw()

rmcanv.Draw()
rmcanv.SaveAs("./RecupCF4_109Cd_uncorrected_mainPeak.pdf")


# # Plotting normalized, and both at same time:

# In[22]:


fl3r.Scale(1./fl3r.Integral())
rl3r.Scale(1./rl3r.Integral())


# In[30]:


fl3r.SetMarkerStyle(2)

rl3r.SetMarkerStyle(2)
rl3r.SetMarkerColor(2)

fnorm_onepeak = ROOT.TF1("norm_mainPeak","landau(0)",1100,3000)
ronepk.SetParameters(0.02,1650,200)
fl3r.Fit("norm_mainPeak","R")

rnorm_onepeak = ROOT.TF1("rnorm_mainPeak","landau(0)",1200,3000)
rnorm_onepeak.SetParameters(0.02,1650,200)
rl3r.Fit("rnorm_mainPeak","R")

x=0.6
ttext_title = ROOT.TText(x,0.7,'Fresh CF4')
ttext_title.SetNDC()

ttext_mpv1 = ROOT.TText(x,0.65,f'MPV:   {fnorm_onepeak.GetParameter(1):.3f}')
ttext_mpv1.SetNDC()

ttext_sig1 = ROOT.TText(x,0.6,f'Sig:   {fnorm_onepeak.GetParameter(2):.3f}')
ttext_sig1.SetNDC()

ttext_title2 = ROOT.TText(x,0.55,'Recup CF4')
ttext_title2.SetNDC()

ttext_mpv2 = ROOT.TText(x,0.50,f'MPV:   {rnorm_onepeak.GetParameter(1):.3f}')
ttext_mpv2.SetNDC()

ttext_sig2 = ROOT.TText(x,0.45,f'Sig:   {rnorm_onepeak.GetParameter(2):.3f}')
ttext_sig2.SetNDC()

canv = ROOT.TCanvas('bothNorm','bothNorm',800,600)
fl3r.Draw()
fnorm_onepeak.Draw("Same")
rl3r.Draw("Same")
rnorm_onepeak.Draw("Same")

ttext_title.Draw()
ttext_mpv1.Draw()
ttext_sig1.Draw()
ttext_title2.Draw()
ttext_mpv2.Draw()
ttext_sig2.Draw()

canv.Draw()
canv.SaveAs("./BothNormFit_onepeak.pdf")


# In[9]:


nfl3r.Scale(1./fl3r.Integral())
nrl3r.Scale(1./rl3r.Integral())


# In[13]:


nfl3r.SetMarkerStyle(2)

nrl3r.SetMarkerStyle(2)
nrl3r.SetMarkerColor(2)

nfnorm_bkg = ROOT.TF1("norm_bkg","landau(0)")
nfnorm_bkg.SetParameters(0.01,350,50)
nfl3r.Fit("norm_bkg","S")

nrnorm_bkg = ROOT.TF1("rnorm_bkg","landau(0)")
nrnorm_bkg.SetParameters(0.005,400,50)
nrl3r.Fit("rnorm_bkg","S")

x=0.6
ttext_title = ROOT.TText(x,0.7,'Fresh CF4')
ttext_title.SetNDC()

ttext_mpv1 = ROOT.TText(x,0.65,f'MPV:   {nfnorm_bkg.GetParameter(1):.3f}')
ttext_mpv1.SetNDC()

ttext_sig1 = ROOT.TText(x,0.6,f'Sig:   {nfnorm_bkg.GetParameter(2):.3f}')
ttext_sig1.SetNDC()

ttext_title2 = ROOT.TText(x,0.55,'Recup CF4')
ttext_title2.SetNDC()

ttext_mpv2 = ROOT.TText(x,0.50,f'MPV:   {nrnorm_bkg.GetParameter(1):.3f}')
ttext_mpv2.SetNDC()

ttext_sig2 = ROOT.TText(x,0.45,f'Sig:   {nrnorm_bkg.GetParameter(2):.3f}')
ttext_sig2.SetNDC()

bcanv = ROOT.TCanvas('bothNorm_bkg','bothNorm_bkg',800,600)
nfl3r.Draw()
nfnorm_bkg.Draw("Same")
nrl3r.Draw("Same")
nrnorm_bkg.Draw("Same")

ttext_title.Draw()
ttext_mpv1.Draw()
ttext_sig1.Draw()
ttext_title2.Draw()
ttext_mpv2.Draw()
ttext_sig2.Draw()

bcanv.Draw()
bcanv.SaveAs("./BothNormFit_bkg.pdf")


# In[ ]:





# In[ ]:





# In[ ]:





# In[376]:


fcs_th1f=freshgas_tfl['Cathode/charge/chargeL3']
fcs = fcs_th1f.to_numpy()

rcs_th1f=recupgas_tfl['Cathode/charge/chargeL3']
rcs = rcs_th1f.to_numpy()


# In[377]:


def rebin(evts,chrg):
    nevts = []
    nchrg = []

    for i in range(640):
        j = i*32
        nevts.append(sum(evts[j:j+32]))
        nchrg.append(chrg[j])

    nchrg.append(j+32)
    
    return nevts,nchrg


# In[378]:


nfevts, nfchrg = rebin(fcs[0],fcs[1])
nrevts, nrchrg = rebin(rcs[0],rcs[1])


# In[381]:


fig, ax = plt.subplots()
ax.set_xlim(left=100,right=4000)
ax.set_xlabel('Charge in ADC channels')
ax.set_ylabel('Number of Events')
hep.histplot(nfevts,nfchrg,label='Fresh CF4')
hep.histplot(nrevts,nrchrg,label='Recuperated CF4')
plt.legend()
plt.savefig('./RawSpectralComparison.png',format='png',dpi=400)
plt.close()


# In[385]:


fig, ax = plt.subplots()
ax.set_xlim(left=100,right=4000)
ax.set_xlabel('Charge in ADC channels')
ax.set_ylabel('Rel. Num. of Events')
ax.set_ylim(bottom=0,top=0.03)

fscale = 1/sum(nfevts)
rscale = 1/sum(nrevts)

hep.histplot(np.array(nfevts)*fscale,nfchrg,label='Fresh CF4')
hep.histplot(np.array(nrevts)*rscale,nrchrg,label='Recuperated CF4')
plt.legend()
plt.savefig('./RelativeSpectralComparison.png',format='png',dpi=400)


# In[87]:


sum(nevts)


# In[88]:


sum(evts)


# In[ ]:




