#!/usr/bin/env python
import os, sys, subprocess, glob
import string
from pprint import pprint

def getTags(folder):
  infiles = set()
  for file in glob.glob(folder+'/*.root'):
    tag = file.split("/")[-1]
    tag = tag.split("RunIISpring16MiniAODv2")[0]
    tag = tag.split("RunIISummer16MiniAODv2")[0]
    tag = tag.replace("fullbaby_","")
    tag = tag.rstrip("_")
    infiles.add(tag)
  return sorted(infiles)

infolder  = '/net/cms29/cms29r0/babymaker/babies/2017_01_27/mc/unprocessed/'
outfolder = '/net/cms29/cms29r0/babymaker/babies/2017_01_27/mc/corrections/'
quick = True
# leave as empty list to run over all input files in the infolder
# wanted_samples = ['TTJets_HT','TTJets_SingleLeptFromT_genMET-150']
wanted_samples = ['']


## Finding tags for each dataset
tags = getTags(infolder)

# Setting folders
if not os.path.exists(outfolder):
  os.system("mkdir -p "+outfolder)

os.system("JobSetup.csh")
ijob = 0

for tag in tags:
  wanted = False
  for sample in wanted_samples:
    if sample in tag: 
      wanted = True

  if len(wanted_samples)>0 and (not wanted): continue
  
  if quick:
    cmd = "JobSubmit.csh ./run/wrapper.sh ./run/calc_corr.exe --quick -i "+infolder+" -t "+tag+" -o "+outfolder
  else:
    cmd = "JobSubmit.csh ./run/wrapper.sh ./run/calc_corr.exe -i "+infolder+" -t "+tag+" -o "+outfolder
  os.system(cmd)
  print cmd
  ijob += 1

print "\nSubmitted "+str(ijob)+" jobs. Output goes to "+outfolder+"\n"

