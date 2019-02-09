# groomer

Utility package for post-processing ntuples produced by babymaker.

### Pending updates for Full Run II dataset !!!

    * Update SFs for b-tagging for 2016 and 2018 FullSim
    * Update SFs for b-tagging for 2016 and 2018 FastSim
    * Update SFs for leptons for 2018 FullSim 
    * Update SFs for leptons for 2017 and 2018 FastSim
    * ISR weights for 2017 and 2018 FastSim

N.B. When adding new SF files, check that the axis assignment match what is assumed in the code!! They change from POG to POG and year to year...

### Interactive test

Follow these steps to test the code on a single file. Example given for a file from 2017 MC.

Step 1. Make some test directories:

    mkdir -p out/unprocessed out/reweighted out/sum_of_weights out/corrections out/unskimmed

Step 2. Apply any SFs and sum up the weights:

    ./compile.sh && ./run/calc_corr.exe \
    --in_file /net/cms2/cms2r0/babymaker/babies/2018_12_17/mc/unprocessed/fullbaby_TTJets_SingleLeptFromT_TuneCP5_13TeV-madgraphMLM-pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1_110.root \
    --wgt_dir out/sum_of_weights/ \
    --out_dir out/reweighted/ \
    --year 2017

Step 3. Merge weights and calculate normalization corrections:

    ./python/merge_corrections.py --wgt_dir out/sum_of_weights/ --corr_dir out/corrections/

Step 4. Apply the renormalization weights from found in step 3:

    ./compile.sh && ./run/apply_corr.exe \
    --in_file out/reweighted/fullbaby_TTJets_SingleLeptFromT_TuneCP5_13TeV-madgraphMLM-pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1_110.root \
    --corr_file out/corrections/corr_TTJets_SingleLeptFromT_TuneCP5_13TeV-madgraphMLM-pythia8.root \
    --out_file out/unskimmed/fullbaby_TTJets_SingleLeptFromT_TuneCP5_13TeV-madgraphMLM-pythia8_RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1_110_renorm.root \
    --year 2017

Step 5. Check that the output looks ok:
    * Do the weights have a mean of 1? Check by drawing "weight/w_lumi", "w_lep", "w_isr", "w_btag_deep", plus any that were changed in the code development or suspicious.
    * If you changed any SFs, did the corresponding weight distribution change between unprocessed and reweighted file?
        N.B.1. unskimmed will always change because of renormalizing 
        N.B.2. These distributions would change if you modify btag_efficiency_deep.root even if the SFs remain the same!

### Batch system usage

Step 1. If not already done, create a link to folder with unprocessed MC from cms2.

Step 2. Check if there are any bad files in the unprocessed directory by chaining all of the files together and doing GetEntries().

Step 3. Apply any SFs and sum up the weights:
    
    ./python/send_calc_corr.py \
    --in_dir /net/cms2/cms2r0/babymaker/babies/$YYYY_MM_DD/mc/unprocessed/ \
    --wgt_dir /net/cms2/cms2r0/babymaker/babies/$YYYY_MM_DD/mc/sum_of_weights/ \
    --out_dir /net/cms2/cms2r0/babymaker/babies/$YYYY_MM_DD/mc/reweighted/ \
    --year $YYYY \
    --njobs 50

Use the option `--fastsim` if running on FastSim samples!

If there is no need to change the SFs, use options `--keep_lep` and `--keep_b`. Note that `reweighted` files will still be written (this is not needed and should be fixed in the future).

Step 4. Check that correct number of files were written to the `reweighted` and the `sum_of_weights` directories.

Step 5. Calculate the renormalization weights from the sums found in step 1:

    ./python/merge_corrections.py \
    --wgt_dir /net/cms2/cms2r0/babymaker/babies/$YYYY_MM_DD/mc/sum_of_weights/ \
    --corr_dir /net/cms2/cms2r0/babymaker/babies/$YYYY_MM_DD/mc/corrections/

This step runs interactively since it should be about a minute. Check that the number of files in `corrections` folder is equal to the number of samples. If not, then the `getTag` function in `merge_corrections.py` did not parse correctly the file names.

Step 6. Apply the renormalization weights from found in step 3:

    ./python/send_apply_corr.py \
    --in_dir /net/cms2/cms2r0/babymaker/babies/$YYYY_MM_DD/mc/reweighted/ \
    --corr_dir /net/cms2/cms2r0/babymaker/babies/$YYYY_MM_DD/mc/corrections/ \
    --out_dir /net/cms2/cms2r0/babymaker/babies/$YYYY_MM_DD/mc/unskimmed/ \
    --year $YYYY \
    --njobs 50

Step 7. When jobs are finished, check that there is as many files in `unskimmed` as there were in `unprocessed` and that they are all good by chaining all unskimmed files together and doing GetEntries(). To skim the files use babymaker package.
