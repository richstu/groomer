#!/usr/bin/env python
import os, sys, subprocess
from shutil import copyfile
from glob import glob
from ROOT import TChain
import string
from pprint import pprint

infolder  = '/net/cms29/cms29r0/babymaker/babies/2017_01_27/mc/unprocessed_skim_met100nj4/'
outfolder = '/net/cms29/cms29r0/babymaker/babies/2017_01_27/mc/skim_met100nj4/'
corrfolder = '/net/cms29/cms29r0/babymaker/babies/2017_01_27/mc/corrections/'

# infolder  = '/net/cms29/cms29r0/babymaker/babies/2017_01_27/mc/unprocessed/'
# outfolder = '/net/cms29/cms29r0/babymaker/babies/2017_01_27/mc/unskimmed/'
# corrfolder = '/net/cms29/cms29r0/babymaker/babies/2017_01_27/mc/corrections/'
quick = True
# leave as empty string to run over all input files in the infolder
one_sample = 'TTJets_Tune'

njobs = 10


def getTag(file):
  tag = file.split("/")[-1]
  tag = tag.split("RunIISpring16MiniAODv2")[0]
  tag = tag.split("RunIISummer16MiniAODv2")[0]
  tag = tag.replace("fullbaby_","")
  tag = tag.rstrip("_")
  return tag

# Setting folders
if not os.path.exists(outfolder):
  os.system("mkdir -p "+outfolder)
runfolder = outfolder+"run/" 
if not os.path.exists(runfolder):
  os.system("mkdir -p "+runfolder)

infiles = []
for x in glob(infolder+"*.root"):
  outfile = x.split('/')[-1]
  outfile = outfolder+outfile.replace(".root","_renorm.root")
  if (quick): outfile = outfile.replace("_renorm.root", "_requick.root")
  if os.path.exists(outfile): continue
  if (one_sample!='') and (one_sample not in x): continue
  if ('unskimmed' not in outfolder):
    c = TChain("tree") 
    c.Add(x)
    if c.GetEntries()==0: 
      copyfile(x, outfile)
      continue
  corrfile = corrfolder + "corr_" + getTag(x) +".root"
  if quick: corrfile = corrfolder + "corrquick_" + getTag(x) +".root"
  if not os.path.exists(corrfile):
    print "Corr. file not found. Skipping:", corrfile 
    continue
  infiles.append(x)
infiles = sorted(infiles)

splitting = len(infiles)/njobs 
if (len(infiles)%njobs!=0): splitting += 1

ijobfiles = [[]]
for i,file in enumerate(infiles):
  iset = i/njobs 


os.system("JobSetup.csh")
for ijob in range(njobs):
  exename = runfolder+"/apply_corr_"+str(ijob)+".sh"
  fexe = open(exename,"w")
  os.system("chmod u+x "+exename)
  fexe.write("#!/bin/bash\n\n")
  for ifile in range(ijob*splitting, (ijob+1)*splitting):
    if ifile>=len(infiles): break 
    infile = infiles[ifile]
    outfile = infile.split('/')[-1]
    outfile = outfolder+outfile.replace(".root","_renorm.root")
    corrfile = corrfolder + "corr" + getTag(infile) +".root"
    if quick: corrfile = corrfolder + "corrquick_" + getTag(infile) +".root"
    if (quick):
      execmd = "\n./run/apply_corr.exe --quick -i "+infile+" -c "+corrfile+" -o "+outfile.replace("_renorm.root","_requick.root")+'\n'
    else:
      execmd = "\n./run/apply_corr.exe -i "+infile+" -c "+corrfile+" -o "+outfile+'\n'

    fexe.write(execmd)
  fexe.write("echo Job finished.")
  fexe.close()
  cmd = "JobSubmit.csh ./run/wrapper.sh "+exename
  print cmd
  os.system(cmd)
  # sys.exit(0)

print "\nSubmitted "+str(ijob)+" jobs. Output goes to "+outfolder+"\n"

