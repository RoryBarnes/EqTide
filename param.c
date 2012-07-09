#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include "eqtide.h"

void LineExit(char infile[],int nline, int iExit, int iVerbose) {
  fprintf(stderr,"\t%s: Line %d\n",infile,nline);
  exit(iExit);
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

double dTempUnit(double dTemp,int iOldType,int iNewType,int iExit) {
  if (iOldType == 0) {
    if (iNewType == 1) {
      /* Kelvin -> Celsius */
      return dTemp - 273;
    } else if (iNewType == 2) {
      /* Kelvin to Farenheit */
      return (dTemp - 273)*1.8 + 32;
    } else if (iNewType == 0) {
      return dTemp;
    } else {
      fprintf(stderr,"ERROR: Unknown Temperature type %d.\n",iNewType);
      exit(iExit);
    }
  } else if (iOldType == 1) {
    if (iNewType == 0) {
      /* Celsius -> Kelvin */
      return dTemp + 273;
    } else if (iNewType == 2) {
      /* Celsius -> Farenheit */
      return (1.8*dTemp) + 32;
    } else if (iNewType == 1) {
      return dTemp;
    } else {
      fprintf(stderr,"ERROR: Unknown Temperature type %d.\n",iNewType);
      exit(iExit);
    }
  } else if (iOldType == 2) {
    if (iNewType == 0) {
      /* Farenheit -> Kelvin */
      return 5/9*(dTemp - 32) + 273;
    } else if (iNewType == 1) {
      /* Farenheit -> Celsius */
      return 5/9*(dTemp - 32);
    } else if (iNewType == 2) {
      return dTemp;
    } else {
      fprintf(stderr,"ERROR: Unknown Temperature type %d.\n",iNewType);
      exit(iExit);
    }
  } else {
    fprintf(stderr,"ERROR: Unknown Temperature type %d.\n",iOldType);
    exit(iExit);
  }
}


double dAngleUnit(int iType, int iExit) {
  if (iType == 0) {
     return 1;
  } else if (iType == 1) {
     return DEGRAD;
  }
}


