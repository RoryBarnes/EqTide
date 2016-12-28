/**************************************************************************
 * util.c                                                                 *
 *                                                                        *
 * Written by Rory Barnes                                                 * 
 *                                                                        *
 * This file contains subroutines for I/O, physical and mathematical      *
 * relationships, and the help message.                                   *
 *                                                                        *
 **************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include "eqtide.h"
#include "options.h"

char *InitializeString(int iLen) {
  int i;
  char *str,cTmp[iLen];
  
  str=malloc(iLen*sizeof(char));
  
  for (i=0;i<iLen;i++)
    str[i]=0;

  return str;
}

char *lower(char str[]) {
  int i;
  for (i=0;str[i];i++)
  str[i] = tolower(str[i]);
    
  return str;
}

void fprintd(FILE *fp,double x,int iExp,int iDig) {
  /* Should modify to allow output precision */
  double min,max;
  
  max=pow(10,iExp);
  min=pow(10,-iExp);
  if ((fabs(x) > max || fabs(x) < min) && x != 0) {
    if (iDig == 0) 
      fprintf(fp,"%.0e",x);
    if (iDig == 1) 
      fprintf(fp,"%.1e",x);
    if (iDig == 2) 
      fprintf(fp,"%.2e",x);
    if (iDig == 3) 
      fprintf(fp,"%.3e",x);
    if (iDig == 4) 
      fprintf(fp,"%.4e",x);
    if (iDig == 5) 
      fprintf(fp,"%.5e",x);
    if (iDig == 6) 
      fprintf(fp,"%.6e",x);
    if (iDig == 7) 
      fprintf(fp,"%.7e",x);
    if (iDig == 8) 
      fprintf(fp,"%.8e",x);
    if (iDig == 9) 
      fprintf(fp,"%.9e",x);
    if (iDig == 10) 
      fprintf(fp,"%.10e",x);
    if (iDig == 11) 
      fprintf(fp,"%.11e",x);
    if (iDig == 12) 
      fprintf(fp,"%.12e",x);
    if (iDig == 13) 
      fprintf(fp,"%.13e",x);
    if (iDig == 14) 
      fprintf(fp,"%.14e",x);
    if (iDig == 15) 
      fprintf(fp,"%.15e",x);
    if (iDig == 16) 
      fprintf(fp,"%.16e",x);
  } else {	
    if (iDig == 0) 
      fprintf(fp,"%.0lf",x);
    if (iDig == 1) 
      fprintf(fp,"%.1lf",x);
    if (iDig == 2) 
      fprintf(fp,"%.2lf",x);
    if (iDig == 3) 
      fprintf(fp,"%.3lf",x);
    if (iDig == 4) 
      fprintf(fp,"%.4lf",x);
    if (iDig == 5) 
      fprintf(fp,"%.5lf",x);
    if (iDig == 6) 
      fprintf(fp,"%.6lf",x);
    if (iDig == 7) 
      fprintf(fp,"%.7lf",x);
    if (iDig == 8) 
      fprintf(fp,"%.8lf",x);
    if (iDig == 9) 
      fprintf(fp,"%.9lf",x);
    if (iDig == 10) 
      fprintf(fp,"%.10lf",x);
    if (iDig == 11) 
      fprintf(fp,"%.11lf",x);
    if (iDig == 12) 
      fprintf(fp,"%.12lf",x);
    if (iDig == 13) 
      fprintf(fp,"%.13lf",x);
    if (iDig == 14) 
      fprintf(fp,"%.14lf",x);
    if (iDig == 15) 
      fprintf(fp,"%.15lf",x);
    if (iDig == 16) 
      fprintf(fp,"%.16lf",x);
  }
}

void InitializeIO(IO *io) {
  io->exit_param = EXIT_PARAM;
  io->exit_units = EXIT_UNITS;
  io->exit_exe = EXIT_EXE;
  io->exit_output = EXIT_OUTPUT;
}

void LineExit(char infile[],int nline, int iExit, int iVerbose) {
  fprintf(stderr,"\t%s: Line %d\n",infile,nline+1);
  exit(iExit);
}

/* Return proper length conversion */
double dLengthUnit(int iType,int iExit) {
  if (iType == 0) {
    return 1;
  } else if (iType == 1) {
    return 100;
  } else if (iType == 2) {
    return 1e5;
  } else if (iType == 3) {
    return RSUN;
  } else if (iType == 4) {
    return REARTH;
  } else if (iType == 5) {
    return RJUP;
  } else if (iType == 6) {
    return AUCM;
  } else {
    fprintf(stderr,"ERROR: Unknown iUnitLength %d\n.",iType);
    exit(iExit);      
  }
}

/* Return proper time conversion */
double dTimeUnit(int iType, int iExit) {
  if (iType == 0) {
    return 1;
  } else if (iType == 1) {
    return DAYSEC;
  } else if (iType == 2) {
    return YEARSEC;
  } else if (iType == 3) {
    return 1e6*YEARSEC;
  } else if (iType == 4) {
    return 1e9*YEARSEC;
  } else {
    fprintf(stderr,"ERROR: Unknown iTimeUnit %d\n.",iType);
    exit(iExit);      
  }
}

double dMassUnit(int iType, int iExit) {
  if (iType == 0) {
    return 1;
  } else if (iType == 1) {
    return 100;
  } else if (iType == 2) {
    return MSUN;
  } else if (iType == 3) {
    return MEARTH;
  } else if (iType == 4) {
    return MJUP;
  } else if (iType == 5) {
    return MSAT;
  } else {
    fprintf(stderr,"ERROR: Unknown iMassUnit %d\n.",iType);
    exit(iExit);      
  }
}

double dAngleUnit(int iType, int iExit) {
  if (iType == 0)
     return 1;
  else if (iType == 1)
     return DEGRAD;
  else {
    fprintf(stderr,"ERROR: Unknown iAngleUnit %d\n.",iType);
    exit(iExit);      
  }
}

/* Help message */

void WriteHelp(OUTPUT output,OPTIONS options,char exe[]) {
  int i;
  char ESC=27;
  
  printf("\n\t\t\tHelp Message for %s\n",exe);
  printf("\nWritten by Rory Barnes\n");
  printf("\n%s is a general purpose tidal integrator. It takes ",exe);
  printf("an input file consisting of options and initial conditions ");
  printf("and simulates tidal evolution, along with other secondary ");
  printf("parameters, forward and/or backward in time. This help ");
  printf("describes the basics of the input file.\n\n");
  
  printf("----- Command Line Options -----\n\n");
  printf("%c[1m-v, -verbose%c[0m -- Maximum verbosity, i.e. display all warnings and updates.\n",ESC,ESC);
  printf("%c[1m-q, -quiet%c[0m   -- No verbosity, i.e. nothing printed to device.\n",ESC,ESC);
  printf("%c[1m-h, -help%c[0m    -- Display this message.\n\n",ESC,ESC);
  
  printf("----- Input File -----\n\n");
  printf("Comments/White Space: Any characters to the right of a # sign ");
  printf("are treated as a comment and are ignored. All white space is ");
  printf("ignored.\n");
  printf("Options must be the first string on any line, and must be ");
  printf("written exactly as listed below.\n");
  printf("Arguments may have any format, and need only be unambiguous.\n\n");
  
  printf("----- Input Options -----\n\n");
  
  for (i=0;i<NUMOPT;i++) {
    if (memcmp(options.cParam[i],"null",4)) {
      printf("%c[1m%s%c[0m (",ESC,options.cParam[i],ESC);
      if (options.iType[i] == 0)
	printf("Bool");
      else if (options.iType[i] == 1) 
	printf("Int");
      else if (options.iType[i] == 2)
	printf("Double");
      else if (options.iType[i] == 3)
	printf("String");
      else if (options.iType[i] == 4)
	printf("Array");
      printf(") -- %s (Default = %s).\n",options.cDescr[i],options.cDefault[i]);
    }
  }
  
  printf("\n----- Output Options -----\n\n");
  printf("These options follow the argument %s.\n",options.cParam[OPT_OUTPUTORDER]);
  
  for (i=0;i<NUMOUT;i++) {
    if (memcmp(output.cParam[i],"null",4)) {
      if (output.iNeg[i] == 0)
	printf("%c[1m[-]%c[0m",ESC,ESC);
      printf("%c[1m%s%c[0m -- %s.",ESC,output.cParam[i],ESC,output.cDescr[i]);
      if (output.iNeg[i] == 0)
	printf(" [%s]",output.cNeg[i]);
      printf("\n");
    }
  }
}


/*
 * Stellar Mass-Radius Relationships
 */

/* Stellar mass-radius relationship from New Light on Dark Stars, Table 4.1. 
 * Fit by Rory Barnes.  */
double dReidHawley_RadMass(double dRadius) {
    double x,y;
    
    x = log10(dRadius/RSUN);
    y = 0.1277 + 2.185*x + 3.135*x*x + 1.9031*x*x*x;
    
    return pow(10,y)*MSUN;
}

/* Stellar mass-radius relationship from New Light on Dark Stars, Table 4.1. 
 * Fit by Rory Barnes.  */
double dReidHawley_MassRad(double dMass) {
  double x,y;

  x = log10(dMass/MSUN);
  y = 0.1424 + 1.568*x - 0.2342*x*x - 0.5581*x*x*x;
  
  return pow(10,y)*RSUN;
}

/* Stellar mass-radius relationship from From Gorda, S. Yu. &
   Svechnikov, M. A. 1999, Astronomy Reports, 43, 521-525 */ 
double dGordaSvech99_MassRad(double dMass) {
  dMass = log10(dMass/MSUN);

  if (dMass > 0.14)
    return pow(10,(0.096 + 0.652*log10(dMass)))*RSUN;
  else
    return pow(10,(0.1 + 1.03*log10(dMass)))*RSUN;
}

/* Stellar mass-radius relationship from From Gorda, S. Yu. & 
 *   Svechnikov, M. A. 1999, Astronomy Reports, 43, 521-525 
 * Fit by Rory Barnes */

/* Only valid for M dwarfs */ 
double dGordaSvech99_RadMass(double dRadius) {
    double x,y;
    
    x = log10(dRadius/REARTH);
    y = -0.09709 + 0.9709*x - 2.502e-5*x*x - 1.34e-5*x*x*x;
    
    return pow(10,y);
}

/* Stellar mass-radius relationship from Bayless, A.J. & Orosz,
   J.A. 2006, ApJ, 651, 1155-1165 */
double dBaylessOrosz06_MassRad(double dMass) {
    double foo;
    
    dMass = dMass/MSUN;
    foo = 0.0324 + 0.9343*dMass + 0.0374*dMass*dMass;
    
    return foo*RSUN;
}

/* Stellar mass-radius relationship from Bayless, A.J. & Orosz,
   J.A. 2006, ApJ, 651, 1155-1165 */
double dBaylessOrosz06_RadMass(double dRadius) {
    double foo;

    dRadius = dRadius/RSUN;
    foo = -0.03477 + 1.07146*dRadius - 8.171*dRadius*dRadius -0.0412*dRadius*dRadius*dRadius;

    return foo*MSUN;
}

/* Stellar mass-radius relationship from Baraffe et al. (2015), assuming 3 Gyr age, A&A 577, A42. Fit by Rory Barnes, see Barnes (2016) */
double dBaraffe15_MassRad(double dMass) {
  dMass /= MSUN;

  return (0.003269 + 1.304*dMass - 1.312*dMass*dMass + 1.055*dMass*dMass*dMass)*RSUN;
}

/* Terrestrial planet mass-radius relationship from Sotin et al 2007
   Icarus, 191, 337-351. */
double dSotin07_MassRad(double dMass) {
    return pow(dMass/MEARTH,0.272)*REARTH;
}

/* Terrestrial planet mass-radius relationship from Sotin et al 2007
   Icarus, 191, 337-351. */
double dSotin07_RadMass(double dRadius) {
    return pow(dRadius/REARTH,3.6765)*MEARTH;
}

/* Useful Relationships */

/* Convert period to Semi-major axis */
double p2a(double p,double m) {
  return pow(BIGG*m/(4*PI*PI)*p*p,(1.0/3));
}

double a2p(double a,double ms) {
  return pow(4*PI*PI*a*a*a/(BIGG*ms),0.5);
}

/* Convert an angular frequency to a period */
double dFreqToPer(double omega) {
  return 2*PI/omega;
}

/* Convert a period to an angular frequency */
double dPerToFreq(double period) {
  return 2*PI/period;
}

/* Convert semi-major axis to mean motion */
double dSemiToMeanMotion(double dSemi,double dMass) {
  return pow(BIGG*dMass/pow(dSemi,3),0.5);
}

double dMeanMotionToSemi(double dMM,double dMass) {
  return pow(BIGG*dMass/(dMM*dMM),(1./3));
}

double dOrbAngMom(PRIMARY *pri,SECONDARY *sec) {
  return pow( BIGG*(pri->dMass+sec->dMass)*sec->dSemi*(1-sec->dEcc*sec->dEcc),0.5);
}

double dSpinAngMom(double dRG,double dMass,double dRad,double dOmega) {
  return dRG*dRG*dMass*dRad*dRad*dOmega;
}

double dRotEn(double dMass,double dRadius,double dRG,double dOmega) {
  return 0.5*dRG*dRG*dMass*dRadius*dRadius*dOmega*dOmega;
}

double dOrbEn(double dMass1, double dMass2,double dSemi) {
  return BIGG*dMass1*dMass2/dSemi;
}

double dRotVel(double dRadius,double dOmega) {
  return 2*PI*dRadius*dOmega;
}

double dDPerDt(double dOmega, double dDOmegaDt) {
    return -2*PI*dDOmegaDt/(dOmega*dOmega);
}

double DOrbPerDt(double dMass,double dSemi,double dDaDt) {
  return 3*PI*sqrt(dSemi/(BIGG*dMass))*dDaDt;
}

int iSign(double dArg) {
  int iVal;

  if (fabs(dArg) > EPS) 
    iVal = (int)(dArg/fabs(dArg));
  else 
    iVal = 0;
  return iVal;
}
