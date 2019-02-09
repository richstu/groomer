#!/usr/bin/env python
import os, sys, subprocess
from shutil import copyfile
from glob import glob
from ROOT import TChain
import string
from pprint import pprint

parser = argparse.ArgumentParser(description="Submits batch jobs to apply new SFs and compute sum-of-weights",
                                 formatter_class=argparse.ArgumentDefaultsHelpFormatter)
parser.add_argument("-i","--in_dir", default="/net/cms2/cms2r0/babymaker/babies/2019_01_11/mc/reweighted/",
                    help="Directory containing babies with old SFs and non-renormalized weights")
parser.add_argument("-o","--out_dir", default="/net/cms2/cms2r0/babymaker/babies/2019_01_11/mc/unskimmed/",
                    help="Directory in which to store reweighted/modified babies")
parser.add_argument("-w","--corr_dir", default="/net/cms2/cms2r0/babymaker/babies/2019_01_11/mc/corrections/",
                    help="Directory in which to store sum-of-weights files for renormalization step.")
parser.add_argument("-f","--fastsim", action="store_true",
                    help="Use when running on FastSim samples.")
parser.add_argument("-y","--year", type=int, default=2016, help="Sample year.")
parser.add_argument("-n","--njobs", type=int, default=50, help="Number of jobs to submit")
args = parser.parse_args()


def getTag(file):
  tag = file.split("/")[-1]
  tag = tag.split("RunIISpring16MiniAODv2")[0]
  tag = tag.split("RunIISummer16MiniAODv2")[0]
  tag = tag.split("RunIIFall17MiniAODv2")[0]
  tag = tag.split("RunIISummer16MiniAODv3")[0]
  tag = tag.replace("fullbaby_","")
  tag = tag.rstrip("_")
  return tag

# Setting folders
if not os.path.exists(args.out_dir):
  os.system("mkdir -p "+args.out_dir)
runfolder = args.out_dir+"run/" 
if not os.path.exists(runfolder):
  os.system("mkdir -p "+runfolder)

infiles = []
for x in glob(args.in_dir+"*.root"):

  # check if output file already exists
  outfile = x.split('/')[-1]
  outfile = args.out_dir+outfile.replace(".root","_renorm.root")
  if os.path.exists(outfile): continue
  # check that corrections file exists
  corrfile = args.corr_dir + "corr_" + getTag(x) +".root"
  if not os.path.exists(corrfile):
    print "Corr. file not found. Skipping:", corrfile 
    continue
  # check if there are 0 entry files if running on a skim
  if ('unskimmed' not in args.out_dir):
    c = TChain("tree") 
    c.Add(x)
    if c.GetEntries()==0: 
      copyfile(x, outfile)
      # print "Input file has 0 entries, copying to output:", x
      continue
  infiles.append(x)
infiles = sorted(infiles)

splitting = len(infiles)/njobs 
if (len(infiles)%njobs!=0): splitting += 1

os.system("JobSetup.csh")
done = False
for ijob in range(njobs):
  exename = runfolder+"/apply_corr_"+str(ijob)+".sh"
  fexe = open(exename,"w")
  os.system("chmod u+x "+exename)
  fexe.write("#!/bin/bash\n\n")
  for ifile in range(ijob*splitting, (ijob+1)*splitting):
    if ifile>=len(infiles): 
      done = True
      break 
    fexe.write("echo Doing file "+str(ifile-ijob*splitting)+" out of "+str(splitting))

    infile = infiles[ifile]
    outfile = infile.split('/')[-1]
    outfile = args.out_dir+outfile.replace(".root","_renorm.root")
    corrfile = args.corr_dir + "corr_" + getTag(infile) +".root"
    execmd = "\n./run/apply_corr.exe -i "+infile+" -c "+corrfile+" -o "+outfile+" -y "+args.year
    if (args.fastsim):
      execmd += " --fastsim"
    execmd += '\n'
    fexe.write(execmd)

  fexe.write("echo Job finished.")
  fexe.close()
  cmd = "JobSubmit.csh ./run/wrapper.sh "+exename
  print cmd
  os.system(cmd)
  if (ijob==0): os.system('cat '+exename)
  if done: break

print "\nSubmitted "+str(ijob)+" jobs. Output goes to "+args.out_dir+"\n"

