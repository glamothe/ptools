#!/bin/sh
g77 -c lbfgs.f
gcc -Wall -o sdrive sdrive.c lbfgs_wrapper.c lbfgs.o -lg2c && ./sdrive

g77 -c lbfgsb.f
gcc -Wall  -o driver1 driver1.c lbfgsb_wrapper.c lbfgsb.o -lg2c && ./driver1
