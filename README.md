EqTide calculates the evolution of 2 bodies experiencing tidal
evolution according to the "equilibrium tide" framework including the "constant-phase-lag" and "constant-time-lag models." This algorithm is based on models by Ferraz-Mello, S. et al. (2008), CeMDA, 101, 171-201. If you use this software, please cite Barnes, R. (2016), CeMDA, submitted.

To compile:

> make

which creates an executable "eqtide". The default is to compile with 
optimization (-O3).

Two examples are presented in the Examples directory. EarthMoon.in
performs a backward integration of the Earth-Moon system, reproducing
the classic "lunar age" problem. Kepler22b.in performs a forward integration of the
Kepler-22 b system. To run either:

> eqtide [OPTION = -h, -v, -q] filename

The input file contains a list of options that can be set, as well as
output parameters that print to a file during an integration. The
example input files provide a guide for the syntax and grammar of
EqTide. The results of these examples should be compared to results in
Barnes (2016).

To see the full list of input options and output parameters:

> eqtide -h 

Run this before editing the example input files; it will help you immensely
to understand how EqTide operates.

EqTide is written in C, and the code has been swept with valgrind's memcheck 
facility and is free of unitialized parameters and buffer overflows. There are 
still a few memory leaks, but none is likely to cause any problems. This software 
has been tested on the following OS platforms: Mac OS 10.11.

This code was developed under the National Science Foundation (USA)
grant AST-1110882 and NASA Cooperative Agreement NNA13AA93A.

