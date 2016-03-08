EQTIDEe calculates the evolution of 2 bodies experiencing tidal
evolution according to the "equilibrium tide" (ET) framework. These two models are based on the model by Ferraz-Mello, S. et al. (2008), CeMDA, 101, 171-201. If you use this software, please cite Barnes, R. (2016), CeMDA, submitted.

To compile:

> make

which creates an executable "eqtide". EQTIDE is written in C, and and
the default is to compile with optimization.

Two examples are presented in the Example directory. EarthMoon.in
performs a backward integration of the Earth-Moon system, reproducing
the classic "acceleration problem" of the lunar
expansion. Kepler22b.in performs a forward integration of the
Kepler-22 b system. To run either:

> eqtide file

The input file contains a list of options that can be set, as well as
output parameters that printed to a file during an integration. The
example input files provide a guide for the syntax and grammar of
EQTIDE. The results of these examples should be compared to results in
Barnes (2016).

To see the full list of input options and output parameters:

> eqtide -h 

There are still a few outstanding issues related to input and output,
but for the most part the software works and is user-friendly. The
integration is robust.

This code was developed under the National Science Foundation (USA)
grant AST-1110882, as well as NASA Cooperative Agreement NNA13AA93A.

