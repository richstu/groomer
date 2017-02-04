# groomer

Utility package for post-processing ntuples produced by babymaker.

### Base classes

Two classes are auto-generated on compilation:
   * `baby_plus` - if provided both input and output files, each instance contains two trees: 1) an input tree containing the branches listed in `variables/full`, used to read the standard babies; 2) an output that has all the branches of the input tree + any new specified in `variables/new_full`, used to write out the e.g. renormalized baby. To use just for reading a baby, omit the output name.
   * `baby_corr` - can be used to either read or write a tree containing the weight correction factors; variables for which to create branches are given by `variables/corr` + any new variables that may be desired in the "corection tree" as specified by `variables/new_corr`

### Renormalizing weights

   1. Send batch jobs to calculate reweighting factors using `send_calc_corr.py`.  The script contains the option `quick` for running on a limited set of variables to be renormalized, see `calc_corr.cxx` for a full list. The script would send one job per MC sample and write out a correction tree for each MC sample, which then serves as input in step 2.
   2. Send batch jobs to apply reweighting factors using `send_apply_corr.py`.  Choose option `quick` if it was used in step 1. Also, choose number of jobs over which to split the load.

### Applying SFs

(To be implemented) 
   1. Calculate renormalization correction factors, taking into account the new SF weights; do not write out unnormalized ntuples with SFs applied (too expensive)
   2. Apply the above correction factors and the SFs, his time writing out the results to a new final baby
