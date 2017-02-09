#! /usr/bin/env python

from __future__ import print_function

import argparse
import subprocess
import os

import ROOT

def fullPath(path):
    return os.path.realpath(os.path.abspath(os.path.expanduser(path)))

def ensureDir(path):
    try:
        os.makedirs(path)
    except OSError:
        if not os.path.isdir(path):
            raise

def getTag(path):
    tag = path.split("/")[-1]
    tag = tag.split("RunIISpring16MiniAODv2")[0]
    tag = tag.split("RunIISummer16MiniAODv2")[0]
    tag = tag.replace("fullbaby_","")
    tag = tag.replace("mergedbaby_","")
    tag = tag.strip("_")
    tag = tag.split("__")[0]
    return tag

def mergeCorrections(input_dir, output_dir):
    input_dir = fullPath(input_dir)
    output_dir = fullPath(output_dir)

    ensureDir(output_dir)
    
    input_files = [os.path.join(input_dir,f) for f in os.listdir(input_dir)
                   if os.path.isfile(os.path.join(input_dir, f))
                   and os.path.splitext(f)[1] == ".root"]

    tags = set([getTag(f) for f in input_files])
    print(input_files)
    print(tags)

    for tag in tags:
        output_file = os.path.join(output_dir, "corr_"+tag+".root")
        command = ["run/merge_corrections.exe", output_file]
        for f in input_files:
            if tag in f:
                command.append(f)
        subprocess.call(command)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Merges multiple sum-of-weights files into one corrections file per tag.",
                                     formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument("input_dir", default="/net/cms29/cms29r0/babymaker/babies/2017_01_27/mc/sum_of_weights/",
                        help="Directory from which to read sum-of-weights files")
    parser.add_argument("output_dir", default="/net/cms29/cms29r0/babymaker/babies/2017_01_27/mc/corrections/",
                        help="Directory in which to store corrections files")
    args = parser.parse_args()

    mergeCorrections(args.input_dir, args.output_dir)
