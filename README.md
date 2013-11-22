compileop
=========

Bash scripts to test the effects of various GCC optimisations on the runtime of C programs.  

*singletest* contains code to will test each optimisation individually.  
*fulltest* contains code to test every combination of optimisations.  

Optimisations are found in *lists.sh*.
Singletest and fulltest will both disable every optimisation that is not currently being tested.
The provided list is believed to include the majority of optimisations included in GCC 4.4.7.

Every \*.c file in the *src* directory will be compiled and tested.
Separate scripts (suffixed *bp*) are included to test the provided branch prediction simulator.
These perform the same actions as the other tests, but provide an argument *lstrace.out* when running the compiled program.
These can be ignored.

To run the scripts, simply execute *./run.sh*.
Progress while running the tests is sent to *stdout*, while results are written to *results.csv*.
