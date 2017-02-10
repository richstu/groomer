#! /usr/bin/env python

from __future__ import print_function

import argparse
import os
import subprocess
import numpy

def fullPath(path):
    return os.path.realpath(os.path.abspath(os.path.expanduser(path)))

def ensureDir(path):
    try:
        os.makedirs(path)
    except OSError:
        if not os.path.isdir(path):
            raise

def sendCalcCorr(in_dir, out_dir, wgt_dir, quick, num_jobs):
    in_dir = fullPath(in_dir)
    out_dir = fullPath(out_dir)
    wgt_dir = fullPath(wgt_dir)
    run_dir = os.path.join(out_dir, "run")

    ensureDir(out_dir)
    ensureDir(wgt_dir)
    ensureDir(run_dir)

    groomer_dir = os.path.dirname(os.path.dirname(__file__))
    exe_path = fullPath(os.path.join(groomer_dir,"run","calc_corr.exe"))

    cmssw_dir = os.path.join(os.environ["CMSSW_BASE"],"src")

    if num_jobs < 1:
        num_jobs = 1

    in_files = [ os.path.join(in_dir,f) for f in os.listdir(in_dir)
                 if os.path.isfile(os.path.join(in_dir,f))
                 and os.path.splitext(f)[1] == ".root" ]
    in_files = numpy.array_split(numpy.array(in_files), num_jobs)

    num_submitted = 0

    for sublist in in_files:
        job_files = sublist.tolist()
        run_path = os.path.join(run_dir,"calc_corr_{}.sh".format(num_submitted)) 
        with open(run_path, "w") as run_file:
            os.fchmod(run_file.fileno(), 0755)
            print("#! /bin/bash", file=run_file)
            print("", file=run_file)
            print("DIRECTORY=`pwd`", file=run_file)
            print("cd {}".format(cmssw_dir), file=run_file)
            print(". /net/cms2/cms2r0/babymaker/cmsset_default.sh", file=run_file)
            print("eval `scramv1 runtime -sh`", file=run_file)
            print("cd $DIRECTORY", file=run_file)
            print("", file=run_file)
            for f in job_files:
                command = "{} -f {} -c {} -o {}".format(exe_path,f,wgt_dir,out_dir)
                if quick:
                    command += " --quick"
                print(command, file=run_file)
        subprocess.call(["JobSubmit.csh",run_path])
        num_submitted += 1
        
    print("\nSubmitted {} jobs.".format(num_submitted))
    print("Sum-of-weights sent to {}.".format(wgt_dir))
    print("Modified weight babies sent to {}.".format(out_dir))
    print("Shell scripts send to {}.".format(run_dir))

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Submits batch jobs to apply new SFs and compute sum-of-weights",
                                     formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument("-i","--in_dir", default="/net/cms29/cms29r0/babymaker/babies/2017_01_27/mc/unprocessed/",
                        help="Directory containing babies with old SFs and non-renormalized weights")
    parser.add_argument("-o","--out_dir", default="/net/cms29/cms29r0/babymaker/babies/2017_01_27/mc/reweighted/",
                        help="Directory in which to store reweighted/modified babies")
    parser.add_argument("-w","--wgt_dir", default="/net/cms29/cms29r0/babymaker/babies/2017_01_27/mc/sum_of_weights/",
                        help="Directory in which to store sum-of-weights files for renormalization step.")
    parser.add_argument("-q","--quick", action="store_true",
                        help="Run in quick mode, only adjusting some weights")
    parser.add_argument("-n","--njobs", type=int, default=50, help="Number of jobs to submit")
    args = parser.parse_args()

    sendCalcCorr(args.in_dir, args.out_dir, args.wgt_dir, args.quick, args.njobs)
