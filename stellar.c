#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "eqtide.h"

double dTeff(double dLum,double dRad) {
  return pow((dLum/(4*PI*SIGMA*dRad*dRad)),0.25);
}

/* XUV flux from a star as a function of its age, from Ribas et al
   (2005). This expression requires time units of Gyr, and distance in
   AU. */
double dFxuv_Ribas(double alpha,double beta,double t,double a) 
{
    t /= (1e9*YEARSEC);
    a /= AUCM;
    
    return alpha*pow(t,-beta)/(a*a);
    }


/* Atmospheric mass loss rate, from Valencia et al. (2009). Note that
   they claim it is the "well-known expression for the extreme case of 
   energy-limited escape". */
double dMassLossRate(double epsilon,double fxuv,double rho,double k)
{

    return (3*epsilon*fxuv)/(BIGG*rho*k);
    }

/* Mass lost over interval t to t+dt. */
double dMassLost(double epsilon,double a,double alpha,double beta,double rho,double k,double t,double dt) {
    /* Not sure if this subroutine is working. */
    t /= (1e9*YEARSEC);
    dt /= (1e9*YEARSEC);
    a /= AUCM;
    
    return (3*epsilon*alpha)/(BIGG*rho*k*a*a*(1-beta))*(pow((t+dt),(1-beta)) - pow(t,(1-beta)));
}


double dKTide(double dPriMass,double dSecMass,double dSemi,double dSecXRad) {
  double chi;

  chi = (pow(dSecMass/(3*dPriMass),(1./3))*dSemi)/dSecXRad;

  return (1 - 3/(2*chi) + 1/(2*pow(chi,3)));
}

double dAtmMassLossErkaev07(SECONDARY sec,double dPriLumX,double dPriMass) {
  double ktide,fxuv;

  /* Mass loss per time */

  ktide = dKTide(dPriMass,sec.dMass,sec.dSemi,sec.dRadX);
  fxuv = dInsolation(dPriLumX,sec.dSemi,sec.dEcc);

  return (PI*sec.dRadius*sec.dRadX*sec.dRadX*sec.dAtmXAbsEff*fxuv)/(BIGG*sec.dMass*ktide);
}

double dDesiccationTimeErkaev07(SECONDARY sec,double dPriLumX,double dPriMass) {

  return sec.dMassH/(4*PI*dAtmMassLossErkaev07(sec,dPriLumX,dPriMass));
}


/* White Dwarf Cooling Function. Data from Bergeron et al. (2001), Table 2, 
 * using all masses in the interval 0.55 \le M \le 0.65. !!! Valid only for 
 * 0.6 M_sun WDs !!! 
 * Fit by Rory Barnes. */
double dBergeronWDLum(double dAge) {
  double l;

  dAge /= 1e9*YEARSEC;
  l = -2.47786 - 0.750465*dAge + 0.119914*dAge*dAge - 6.68551e-3*pow(dAge,3);

  return pow(10,l)*LSUN;
}


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


double dReidHawley_MassLum(double dMass) {
  double x,y;

  x = log10(dMass/MSUN);
  y = 0.2984 + 8.712*x + 11.5615*x*x + 6.2407*x*x*x;

  return pow(10,y)*LSUN;
}

double dReidHawley_LumMass(double dLum) {
  double x,y;

  x = log10(dLum/LSUN);
  y = -0.3077 - 0.51*x - 0.4504*x*x - 0.06852*x*x*x;

  return pow(10,y)*MSUN;
}

/* Mass-Luminosity relationship from Scalo, J. 2007, Astrobiology, 7,
   85-166. */
double dScalo07_MassLum(double dMass) {
  double x,y;
    
  x = log10(dMass/MSUN);
  y = 4.101*x*x*x + 8.162*x*x + 7.108*x + 0.065;

  return pow(10,y)*LSUN;
}

/* Mass-Luminosity relationship from Scalo, J. 2007, Astrobiology, 7,
   85-166. */
double dScalo07_LumMass(double dLum) {
    double x,y;

    x = log10(dLum/LSUN);
    y = -0.3536 - 0.5463*x - 0.4422*x*x - 0.06243*x*x*x;

    return pow(10,y)*MSUN;
}


/*
 * IHZ Models
 */

/* Barnes et al. 2008 */

/* Inner IHZ boundary */
void HZBarnes08Inner(double dLuminosity,double dTeff,double dEcc,double *dInnerIHZ) {
  double a,b,t,lin[3];
  int i;

  lin[0]=0.49;
  lin[1]=0.72;
  lin[2]=0.89;

  a=2.7619e-5;
  b=3.8095e-9;
  t=dTeff-5700;

  for (i=0;i<3;i++)
    dInnerIHZ[i] = (lin[i] - a*t - b*t*t)*sqrt(dLuminosity/LSUN)/pow((1-dEcc*dEcc),1.0/4)*AUCM;
}

/* Outer IHZ boundary */
void HZBarnes08Outer(double dLuminosity,double dTeff,double dEcc,double *dOuterIHZ) {
  double a,b,t,lout[3];
  int i;

  lout[0]=1.67;
  lout[1]=1.95;
  lout[2]=2.4;

  a=2.7619e-5;
  b=3.8095e-9;
  t=dTeff-5700;

  for (i=0;i<3;i++)
    dOuterIHZ[i] = (lout[i]- a*t - b*t*t)*sqrt(dLuminosity/LSUN)/pow((1-dEcc*dEcc),1.0/4)*AUCM; 
}

/* Selsis et al. 2007 */

/* Inner IHZ boundary */
void HZSelsis07Inner(double dLuminosity,double dTeff,double *dInnerIHZ) {
  double a,b,t,lin[3];
  int i;

  lin[0]=0.49;
  lin[1]=0.72;
  lin[2]=0.89;

  a=2.7619e-5;
  b=3.8095e-9;
  t=dTeff-5700;

  for (i=0;i<3;i++)
    dInnerIHZ[i] = (lin[i] - a*t - b*t*t)*sqrt(dLuminosity/LSUN)*AUCM;
}

/* Outer IHZ boundary */
void HZSelsis07Outer(double dLuminosity,double dTeff,double *dOuterIHZ) {
  double a,b,t,lout[3];
  int i;

  lout[0]=1.67;
  lout[1]=1.95;
  lout[2]=2.4;

  a=2.7619e-5;
  b=3.8095e-9;
  t=dTeff-5700;

  for (i=0;i<3;i++)
    dOuterIHZ[i] = (lout[i] - a*t - b*t*t)*sqrt(dLuminosity/LSUN)*AUCM; 
}

/* Ramires et al 2011 */

/* Inner boundary */
void HZRamires11Inner(double dLuminosity,double dTeff,double *dInnerIHZ) {
  double a0,a1,a2,a3,b0,b1,b2,b3;

  /* Venus limit coefficients, computed by Rory 06/13/11 */
  a0 = 0.8393;
  a1 = 6.4601e-6;
  a2 = -6.106e-9;
  a3 = 3.781e-13;

  /* Moist Greenhouse Limit */
  b0 = 1.0461;
  b1 = 7.8913e-6;
  b2 = -7.576e-9;
  b3 = 4.691e-13;

  dInnerIHZ[0] = (a0 + a1*dTeff + a2*dTeff*dTeff + a3*pow(dTeff,3))*pow((dLuminosity/LSUN),0.5);
  dInnerIHZ[1] = (b0 + b1*dTeff + b2*dTeff*dTeff + b3*pow(dTeff,3))*pow((dLuminosity/LSUN),0.5);
}

/* Outer boundary */
void HZRamires11Outer(double dLuminosity,double dTeff,double *dOuterIHZ) {
  double a0,a1,a2,a3,b0,b1,b2,b3;

  /* Maxmimum Greenhouse Limit */
  a0 = 2.359;
  a1 = -1.694e-4;
  a2 = -3.077e-9;
  a3 = 8.947e-13;

  /* Mars limit coefficients, computed by Rory 06/13/11 */
  b0 = 2.795;
  b1 = -2.006e-4;
  b2 = -3.645e-9;
  b3 = 1.06e-12;

  dOuterIHZ[0] = (a0 + a1*dTeff + a2*dTeff*dTeff + a3*pow(dTeff,3))*pow((dLuminosity/LSUN),0.5);
  dOuterIHZ[1] = (b0 + b1*dTeff + b2*dTeff*dTeff + b3*pow(dTeff,3))*pow((dLuminosity/LSUN),0.5);
}

/* Ramires' albedo */

double dAlbedoRamires11(double dTeff) {
  double a0,a1,a2,a3;

  a0 = 8.384e-3;
  a1 = -5.909e-6;
  a2 = 1.225e-8;
  a3 = -8.105e-13;

  return a0 + a1*dTeff + a2*dTeff*dTeff + a3*pow(dTeff,3);
}


/* Radius Contraction Models */

double dDSpinRateContrDt(double dRadius,double dOmega,int iModel,double dAge) {
  double dDRDT;

  if (iModel == 1) {
    dDRDT = dDRadDt_Baraffe98(dAge);
  } else if (iModel == 2) {
    dDRDT = dDRadDt_DM97(dAge);
  }

  return dSpinRateDt_dDRDT(dRadius,dOmega,dDRDT);
}
 
double dDRadDtContr(double dAge,int iModel) {
  if (iModel == 1)
    return dDRadDt_Baraffe98(dAge);
  if (iModel == 2)
    return dDRadDt_DM97(dAge);
}

double dDRadDt_Baraffe98(double age) {
  age /= (1e6*YEARSEC);

  /* Polynomial fit to data provided by Leslie Hebb, see e-mail from Feb 3, 2011 */
  if (age <= 2.6) 
    return (-1.754 + 1.378*age - 0.3444*age*age +0.02758*pow(age,3)) * RSUN/(1e6*YEARSEC);
  else
    return 0;
}

double dDRadDt_DM97(double age) {
  age /= (1e6*YEARSEC);

  /* Polynomial fit to data provided by Leslie Hebb, see e-mail from Feb 3, 2011 */
  if (age < 2.06) {
    return (-2.557 + 2.7085*age - 0.9177*age*age + 0.09971*pow(age,3)) * RSUN/(1e6*YEARSEC);
  } else
    return 0;
}

double dSpinRateDt_dDRDT (double dRadius, double dOmega, double dDRDT) {
  return -2*dOmega/dRadius * dDRDT;
}
