#! /usr/bin/env python

import argparse
import os
import subprocess

def fullPath(path):
    return os.path.realpath(os.path.abspath(os.path.expanduser(path)))

def ensureDir(path):
    try:
        os.makedirs(path)
    except OSError:
        if not os.path.isdir(path):
            raise

def sendCalcCorr(in_dir, out_dir, wgt_dir, quick):
    in_dir = fullPath(in_dir)
    out_dir = fullPath(out_dir)
    wgt_dir = fullPath(wgt_dir)

    ensureDir(out_dir)
    ensureDir(wgt_dir)

    

    num_submitted = 0
    for in_file in os.listdir(in_dir):
        if not os.path.isfile(in_file): continue
        if not os.path.splitext(in_file)[1] == ".root": continue

        command = [ "JobSubmit.csh", "run/calc_corr.exe",
                    "-f", in_file, "-c", wgt_dir, "-o", out_dir ]
        if quick:
            command.append("--quick")

        print(" ".join(command[1:]))
        subprocess.call(command)
        num_submitted += 1
        
    print("\nSubmitted {} jobs.\nSum-of-weights sent to {}.\nModified weight babies sent to {}.".format(
        num_submitted, wgt_dir, out_dir))

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Submits batch jobs to apply new SFs and compute sum-of-weights",
                                     formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument("in_dir", default="/net/cms29/cms29r0/babymaker/babies/2017_01_27/mc/unprocessed/",
                        help="Directory containing babies with old SFs and non-renormalized weights")
    parser.add_argument("out_dir", default="/net/cms29/cms29r0/babymaker/babies/2017_01_27/mc/reweighted/",
                        help="Directory in which to store reweighted/modified babies")
    parser.add_argument("wgt_dir", default="/net/cms29/cms29r0/babymaker/babies/2017_01_27/mc/sum_of_weights/",
                        help="Directory in which to store sum-of-weights files for renormalization step.")
    parser.add_argument("-q","--quick", action="store_true",
                        help="Run in quick mode, only adjusting some weights")
    args = parser.parse_args()

    sendCalcCorr(args.in_dir, args.out_dir, args.wgt_dir, args.quick)
