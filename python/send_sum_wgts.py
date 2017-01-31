#!/usr/bin/env python
import os, sys, subprocess
import string
from pprint import pprint
from utilities import *

def findBaseSampleNames(folder):
    infiles = set()
    for file in glob.glob(folder+'/*.root'):
        tag = file.split("RunII")[0]
        tag = tag.split("13TeV")[0]
        tag = tag.split("CUETP")[0]
        tag = tag.split("-PromptReco")[0]
        tag = tag.split("-23Sep2016")[0]
        tag = tag.split("_runs")[0]
        tag = tag.split("pythia")[0]
        tag = tag.split("baby_")[1]
        tag = tag.split("__")[0]
        if tag[0] != '_': tag = "_"+tag
        if tag[-1] != '_' and "Tune" not in tag and "Run2016" not in tag: tag = tag+"_"
        infiles.add(tag)
    sortedfiles = list()
    for file in infiles:
        sortedfiles.append(file)
    sortedfiles = sorted(sortedfiles)

    return sortedfiles

infolder  = '/net/cms29/cms29r0/babymaker/babies/2017_01_27/mc/unprocessed/'
outfolder = '/net/cms29/cms29r0/babymaker/babies/2017_01_27/mc/sum_wgts/'

## Finding tags for each dataset
tags = findBaseSampleNames(infolder)

# Setting folders
if not os.path.exists(outfolder):
  os.system("mkdir -p "+outfolder)

os.system("JobSetup.csh")
ijob = 0

for tag in tags:
  ijob += 1
  cmd = "JobSubmit.csh ./run/wrapper.sh ./run/sum_wgts.exe -i "+infolder+" -t "+tag+" -o "+outfolder
  os.system(cmd)

print "\nSubmitted "+str(ijob)+" jobs. Output goes to "+outfolder+"\n"
