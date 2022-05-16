Version 2.54

This program attempts to model the compound benefit of making financially literate decisions
over the course of a lifetime. It takes in a set of identical starting values for both a financially
literate and non-financially literate person, then simulating the growth of their wealth over 40 years.

In its current state, the program may be unable to handle an individual who cannot cover a payment with  
either their checking or savings account, essentially rendering them bankrupt. A patch is in progress and
the fix will be implemented soon.

**NOTE**
This program is most compatible with ATOM TEXT EDITOR. Viewing output files via any other medium may result
in skewed output tables that are illegible. For best results please use Atom.

To run the program:
gcc -o main FinancialSimulation.c
./main output_fl.txt output_nfl.txt
