#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "eqtide.h"
#include "output.h"


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

/* Energy Flux to Temperature */
double dFluxToTemperature(double dFlux) {
  return pow(dFlux/SIGMA,0.25);
}

/* Peak wavelength emission from Tidal Heating */
double dFluxToPeakWavelength(double dFlux) {
  double dTemp;

  dTemp = dFluxToTemperature(dFlux);
  return dTempToPeakWavelength(dTemp);
}

/* Peak Blackbody Emission Wavelength */
double dTempToPeakWavelength(double dTeff) {
  return WIEN/dTeff;
}

double dFluxToLuminosity(double dFlux,double dRadius) {
  double dTemp;

  dTemp = dFluxToTemperature(dFlux);
  return 4*PI*dRadius*dRadius*pow(dTemp,4);
}

/* Luminosity of black body */ 
double dBlackbodyLuminosity(double dRadius,double dTeff) {
  return 4*PI*dRadius*dRadius*SIGMA*pow(dTeff,4);
}

/* Longitudinal Temperature if synchronously rotating and no heat transport */
/* From Leger et al (2009) */
double dNakedPlanetTemp(double dAlbedo,double dPriRad,double dSemi,double dPriTemp,double dTheta) {
  return pow((1 - dAlbedo),0.25)*pow(dPriRad/(dSemi),0.25)*dPriTemp*pow(cos(dTheta),0.25);
}

/* Insolation -- or is it inSTELLation? */
double dInsolation(double dLuminosity,double dSemi,double dEcc){
  return dLuminosity/(4*PI*dSemi*dSemi*pow((1-dEcc*dEcc),0.5));
}

double dPierrehumbertGrnhsFlux(double g,double l,double r,double tref,double pref) {
  double a,p0,k0,pstar;

  a = 0.7344;
  p0 = 1e5; /* g/cm s^2 */
  pstar = pref*exp(l/(r*tref));
  k0 = 0.55; /* cm/g */
  
  return a*SIGMA*pow(l/r,4)/pow(log(pstar/pow(2*p0*g/k0,0.5)),4);
}

double dKastingCatlingGrnhsFlux(double g,double l,double r,double tref,double pref) {
  double kappa;

  /* Gray body water vapor absorption coefficient */
  kappa = 0.1; /* cm^2/g */

  return SIGMA*pow((tref/(1-tref*(r/l)*log(g/(1.66*kappa*pref)))),4);
}

double dRnwyGrnhsFlux(double dMass,double dRadius,int iModel) {
  double g,l,r,tref,pref;
    
  g = BIGG*dMass/(dRadius*dRadius);
  l = 2.425e-4; /* ergs/g */
  r = 4.615e-8; /* ergs/g K */
  
  /* Triple Point values for water of 611 Pa and 273.15 K */ 
  tref = 273.15; /* K */
  pref = 6110; /* g/cm s^2 */
  
  if (iModel == 0) { /* Pierrehumbert 2010 */
    return dPierrehumbertGrnhsFlux(g,l,r,tref,pref);
  } else if (iModel == 1) { /* Kasting & Catling 2012 */
    return dKastingCatlingGrnhsFlux(g,l,r,tref,pref);
  } else if (iModel == 2) { /* Abe et al. 2011 */
    return 415000;
  }
}

double dRadHeat() {
  return 0;
}

double dTeq(double dLuminosity,double dSemi,double dEcc) {
  return pow(dLuminosity/(16*PI*SIGMA*dSemi*dSemi*(1-dEcc*dEcc)),0.25);
}

int iSign(double x) {
  double foo;

  if (x != 0) 
    foo = x/fabs(x);
  else 
    foo = 0;
  return foo;
}

double dOrbAngMom(PRIMARY pri,SECONDARY sec) {
  /* XXX Not verified XXX */
  return pow( BIGG*(pri.dMass+sec.dMass)*sec.dSemi*(1-sec.dEcc*sec.dEcc),0.5);
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


void AssignChi(PRIMARY pri,SECONDARY sec,double *c) {
  c[0]=pri.dRG*pri.dRG*pri.dRadius*pri.dRadius*pri.dSpinRate*sec.dSemi*sec.dMeanMotion/(BIGG*sec.dMass);
  c[1]=sec.dRG*sec.dRG*sec.dRadius*sec.dRadius*sec.dSpinRate*sec.dSemi*sec.dMeanMotion/(BIGG*pri.dMass);
}

double AssignDt(PRIMARY pri,SECONDARY sec,double dDaDt,double dDeDt,double *dDomegaDt,double *dDpsiDt,double dNextOutput,double dEta) {
  double min;

  if (dDaDt != 0)
    min=fabs(sec.dSemi/dDaDt);
  if (dDeDt != 0 && sec.dEcc != 0) {
    if (fabs(sec.dEcc/dDeDt) < min) 
      min=fabs(sec.dEcc/dDeDt);
  }
  if (dDpsiDt[0] != 0 && pri.dObliquity != 0) {
    if (fabs(pri.dObliquity/dDpsiDt[0]) < min)
      min=fabsl(pri.dObliquity/dDpsiDt[0]);
  }
  if (dDpsiDt[1] != 0 && sec.dObliquity != 0) {
    if (fabs(sec.dObliquity/dDpsiDt[1]) < min)
      min=fabs(sec.dObliquity/dDpsiDt[1]);
  }
  if (dDomegaDt[0] != 0) {
    if (fabs(pri.dSpinRate/dDomegaDt[0]) < min)
      min=fabs(pri.dSpinRate/dDomegaDt[0]);
  }
  if (dDomegaDt[1] != 0) {
    if (fabs(sec.dSpinRate/dDomegaDt[1]) < min)
      min=fabs(sec.dSpinRate/dDomegaDt[1]);
  }
  
  min = dEta * min;

  if (dNextOutput < min)
    min = dNextOutput;

  return min;
}



/* 
 * The following subroutine describes the constant-phase-lag model of order 
 * 2. See Heller, Leconte & Barnes (2011), Eqs. 3-10.
 *
 */

double EqSpinRate_CPL2Cont(double n,double e) {
  return (1 + 9.5*e*e)*n;
}

double EqSpinRate_CPL2Discrete(double n,double e) {
  if (e <= pow(1./19,0.5)) {
    return n;
  } else {
    return 1.5*n;
  }
}

double EqSpinRate_CPL2(double n,double e,int bDiscrete) {
    if (bDiscrete)
	return EqSpinRate_CPL2Discrete(n,e);
    else 
	return EqSpinRate_CPL2Cont(n,e);
}

void AssignEpsilon(double om,double n,int *epsilon) {

  /* Debugged 12/13/10 */

  int i;
  epsilon[0]=iSign(2*om-2*n);
  epsilon[1]=iSign(2*om-3*n);
  epsilon[2]=iSign(2*om-n);
  epsilon[5]=iSign(n);
  epsilon[8]=iSign(om-2*n);
  epsilon[9]=iSign(om);
  
}

void AssignZprime(PRIMARY pri,SECONDARY sec,double *z) {

    z[0] = 3*BIGG*BIGG*pri.dK2*sec.dMass*sec.dMass*(pri.dMass+sec.dMass)*pow(pri.dRadius,5)/(pow(sec.dSemi,9)*sec.dMeanMotion*pri.dQ);
    z[1] = 3*BIGG*BIGG*sec.dK2*pri.dMass*pri.dMass*(pri.dMass+sec.dMass)*pow(sec.dRadius,5)/(pow(sec.dSemi,9)*sec.dMeanMotion*sec.dQ);
}

/* Want to split this into annual and daily rates

double dDaDtAnn_CPL2(PRIMARY pri,SECONDARY sec,int **epsilon,zprime[2]) {
  double sum,psi[2];
  int i;
   
  psi[0]=pri.dObliquity;
  psi[1]=sec.dObliquity;

  sum=0;
  for (i=0;i<2;i++) 
    sum += zprime[i]*(4*epsilon[i][0] + sec.dEcc*sec.dEcc*(-20*epsilon[i][0] + 147./2*epsilon[i][1] + 0.5*epsilon[i][2] - 3*epsilon[i][5]));

  return sec.dSemi*sec.dSemi/(4*BIGG*pri.dMass*sec.dMass)*sum;

}

*/

/* This subrotuine may be obsolete, as it calculaties the entire da/dt, but for logging purposes, dividing it up is better */

double dDaDt_CPL2(PRIMARY pri,SECONDARY sec,int **epsilon,double zprime[2]) {
  double sum,psi[2];
  int i;
   
  psi[0]=pri.dObliquity;
  psi[1]=sec.dObliquity;

  sum=0;
  for (i=0;i<2;i++) 
    sum += zprime[i]*(4*epsilon[i][0] + sec.dEcc*sec.dEcc*(-20*epsilon[i][0] + 147./2*epsilon[i][1] + 0.5*epsilon[i][2] - 3*epsilon[i][5]) - 4*sin(psi[i])*sin(psi[i])*(epsilon[i][0]-epsilon[i][8]));

  return sec.dSemi*sec.dSemi/(4*BIGG*pri.dMass*sec.dMass)*sum;

}

double dDeDt_CPL2(PRIMARY pri,SECONDARY sec,int **epsilon,double zprime[2]) {
  double sum;
  int i;

  sum=0;
  for (i=0;i<2;i++)
    sum += zprime[i]*(2*epsilon[i][0] - 49./2*epsilon[i][1] + 0.5*epsilon[i][2] + 3*epsilon[i][5]);

    return -sec.dSemi*sec.dEcc/(8*BIGG*pri.dMass*sec.dMass) * sum;
}  


double dDaDt1_CPL2(double dMass,double dMassPert,double dSemi,double dEcc, double dPsi,int *epsilon,double zprime) {
  double foo;

    foo = zprime*(4*epsilon[0] + dEcc*dEcc*(-20*epsilon[0] + 147./2*epsilon[1] + 0.5*epsilon[2] - 3*epsilon[5]) - 4*sin(dPsi)*sin(dPsi)*(epsilon[0]-epsilon[8]));

  return dSemi*dSemi/(4*BIGG*dMass*dMassPert)*foo;

}

double dDeDt1_CPL2(double dMass,double dMassPert,double dSemi,double dEcc,int *epsilon,double zprime) {
  double foo;
 
    foo = zprime*(2*epsilon[0] - 49./2*epsilon[1] + 0.5*epsilon[2] + 3*epsilon[5]);

    return -dSemi*dEcc/(8*BIGG*dMass*dMassPert) * foo;
}  



double dDomegaDt_CPL2(double dMass,double dRadius,double dN,double dEcc,double dC,double dPsi,int epsilon[9],double zprime) {
 
  return -zprime/(8*dMass*dC*dC*dRadius*dRadius*dN)*(4*epsilon[0] + dEcc*dEcc*(-20*epsilon[0] + 49*epsilon[1] + epsilon[2]) + 2*sin(dPsi)*sin(dPsi)*(-2*epsilon[0]+epsilon[8]+epsilon[9]));
}

double dDoblDt_CPL2(double dMass,double dRadius,double dOmega,double dN,double dC,int epsilon[9],double dChi,double dPsi,double zprime) {

  return zprime*sin(dPsi)/(4*dMass*dC*dC*dRadius*dRadius*dN*dOmega) * (epsilon[0]*(1-dChi) + (epsilon[8]-epsilon[9])*(1+dChi));
}

/* Must condense these two into TideHeat */

double dAnnualHeat_CPL2(int epsilon[9],double zprime,double dEcc,double dPsi) {
}

double dDailyHeat_CPL2(int epsilon[9],double zprime,double dEcc,double dN,double dOmega,double dPsi) {
}

double dTideHeat_CPL2(int epsilon[9],double zprime,double dEcc,double dN,double dOmega,double dPsi) {

  /* Heller, Leconte & Barnes (2011), Eqn 9-11. */

  double dOrbE,dRotE;

  dOrbE = -zprime/8 * (4*epsilon[0] + dEcc*dEcc*(-20*epsilon[0] + 147./2*epsilon[1] + 0.5*epsilon[2] - 3*epsilon[5]) - 4*sin(dPsi)*sin(dPsi)*(epsilon[0]-epsilon[8]));

  dRotE = zprime*dOmega/(8*dN) * (4*epsilon[0] + dEcc*dEcc*(-20*epsilon[0] + 49*epsilon[1] + epsilon[2]) + 2*sin(dPsi)*sin(dPsi)*(-2*epsilon[0] + epsilon[8] + epsilon[9]));

  return dOrbE + dRotE;
}

double dGammaRot(double dEcc,double dPsi,int *epsilon) {
  return 4*epsilon[0] + dEcc*dEcc*(-20*epsilon[0] + 49*epsilon[1] + epsilon[2]) + 2*sin(dPsi)*sin(dPsi)*(-2*epsilon[0] + epsilon[8] + epsilon[9]);
}

double dGammaOrb(double dEcc,double dPsi,int *epsilon) {
    return 4*epsilon[0] + dEcc*dEcc*(-20*epsilon[0] + 147./2*epsilon[1] + 0.5*epsilon[2] - 3*epsilon[5]) - 4*sin(dPsi)*sin(dPsi)*(epsilon[0]-epsilon[8]);
}


double dTideHeatEq_CPL2(double z,double dEcc,double dPsi,double dN,int bDiscrete) {
  /* Heller, Leconte  Barnes 2011, Eq. 13 */
  double gorb,grot,om;
  int *epsilon;

  epsilon = malloc(9*sizeof(int));

  /* Must reset spin rate in order to get the phase lags correctly */

  om = EqSpinRate_CPL2(dN,dEcc,bDiscrete);

  AssignEpsilon(om,dN,epsilon);
  
  grot = dGammaRot(dEcc,dPsi,epsilon);
  gorb = dGammaOrb(dEcc,dPsi,epsilon);

  /* return z/8 * ((om/dN)*grot - gorb); */
  return z/8 * ((1+9.5*dEcc*dEcc)*grot - gorb);
}




/* 
 * The following subroutine describes the constant-phase-lag model of order 
 * 2. See Heller, Leconte & Barnes (2011), Eqs. 16-22.
 *
 */

double AssignBeta(double e) {
    return pow(1 - e*e,0.5);
}

double AssignF1(double e) {
  return (1 + 31./2*e*e + 255./8*pow(e,4) + 185./16*pow(e,6) + 25./64*pow(e,8));
}

double AssignF2(double e) {
  return (1 + 15./2*e*e + 45./8*pow(e,4) + 5./16*pow(e,6));
}

double AssignF3(double e) {
  return (1 + 15./4*e*e + 15./8*pow(e,4) + 5./64*pow(e,6));
}

double AssignF4(double e) {
  return (1 + 1.5*e*e + 1./8*pow(e,4));
}

double AssignF5(double e) {
    return (1 + 3*e*e + 3./8*pow(e,4));
}

double EqSpinRate_CTL8(double n,double e,double psi) {
    double dBeta,f2,f5;

    dBeta=AssignBeta(e);
    f2=AssignF2(e);
    f5=AssignF5(e);

    return f2/(pow(dBeta,3)*f5) * 2*cos(psi)/(1+cos(psi)*cos(psi)) * n;
}

void AssignZ(PRIMARY pri,SECONDARY sec,double *z) {
  z[0] = 3*BIGG*BIGG*pri.dK2*sec.dMass*sec.dMass*(pri.dMass+sec.dMass)*pow(pri.dRadius,5)*pri.dTau/pow(sec.dSemi,9);
  z[1] = 3*BIGG*BIGG*sec.dK2*pri.dMass*pri.dMass*(pri.dMass+sec.dMass)*pow(sec.dRadius,5)*sec.dTau/pow(sec.dSemi,9);
}

double dDaDt_CTL8(PRIMARY pri,SECONDARY sec,double *z,double beta,double *f) {
  int i;
  double psi[2],omega[2],sum;

  psi[0]=pri.dObliquity;
  psi[1]=sec.dObliquity;
  omega[0]=pri.dSpinRate;
  omega[1]=sec.dSpinRate;

  sum=0;
  for (i=0;i<2;i++) 
      sum += z[i]*(cos(psi[i])*f[1]*omega[i]/(pow(beta,12)*sec.dMeanMotion) - f[0]/pow(beta,15));

  return 2*sec.dSemi*sec.dSemi/(BIGG*pri.dMass*sec.dMass)*sum;
}

double dDeDt_CTL8(PRIMARY pri,SECONDARY sec,double *z,double beta,double *f) {
  int i;
  double psi[2],omega[2],sum;

  psi[0]=pri.dObliquity;
  psi[1]=sec.dObliquity;
  omega[0]=pri.dSpinRate;
  omega[1]=sec.dSpinRate;

  sum=0;
  for (i=0;i<2;i++) 
    sum += z[i]*(cos(psi[i])*f[3]*omega[i]/(pow(beta,10)*sec.dMeanMotion) - 18*f[2]/(11*pow(beta,13)));
  
  return 11*sec.dSemi*sec.dEcc/(2*BIGG*pri.dMass*sec.dMass)*sum;
}

double dDaDt1_CTL8(double dMass,double dMassPert,double dSemi,double dEcc,double dMeanMotion,double z,double dOmega,double dPsi,double *f,double beta) {
  double foo;

  foo = z*(cos(dPsi)*f[1]*dOmega/(pow(beta,12)*dMeanMotion) - f[0]/pow(beta,15));

  return 2*dSemi*dSemi/(BIGG*dMass*dMassPert)*foo;
}


double dDeDt1_CTL8(double dMass,double dMassPert,double dSemi,double dEcc,double dMeanMotion,double z,double dOmega,double dPsi,double *f,double beta) {
  double foo;

  foo = z*(cos(dPsi)*f[3]*dOmega/(pow(beta,10)*dMeanMotion) - 18*f[2]/(11*pow(beta,13)));
  
  return 11*dSemi*dEcc/(2*BIGG*dMassPert*dMass)*foo;
}

double dDomegaDt_CTL8(double dMass,double dRadius,double dN,double dEcc,double dC,double dOmega,double dPsi,double z,double beta,double *f) {
    return z/(2*dMass*dC*dC*dRadius*dRadius*dN) * (2*cos(dPsi)*f[1]/pow(beta,12) - (1+cos(dPsi)*cos(dPsi))*f[4]*dOmega/(pow(beta,9)*dN));
}

double dDoblDt_CTL8(double dMass,double dRadius,double dN,double dEcc,double dC,double dOmega,double dPsi,double z,double beta,double *f,double chi) {
  return (z*sin(dPsi))/(2*dMass*dC*dC*dRadius*dRadius*dN*dOmega) * ((cos(dPsi) - chi/beta)*f[4]*dOmega/(pow(beta,9)*dN) - 2*f[1]/pow(beta,12));
}

double dTideHeat_CTL8(double z,double *f,double beta,double dOmega,double dPsi,double dN) {

    return z*(f[0]/pow(beta,15) - 2*f[1]*cos(dPsi)*dOmega/(pow(beta,12)*dN) + ((1+cos(dPsi)*cos(dPsi))/2)*f[4]*dOmega*dOmega/(pow(beta,9)*dN*dN));

}

double dTideHeatEq_CTL8(double z,double *f,double beta,double dPsi,double dN) {
    double heat;
    heat = z/pow(beta,15) * (f[0] - f[1]*f[1]/f[4] * (2*cos(dPsi)*cos(dPsi))/(1+cos(dPsi)*cos(dPsi)));
    return heat;
    }


/*
 * Alternative tidal heating models 
 *
 */

double ViscoelasticHeat(double dMass,double dRadius, double dN, double dEcc,double dSemi,double dImk2) {
  /* dMass is perturber mass, dRadius is body radius */

  return dImk2*10.5*BIGG*dMass*dMass*pow(dRadius,5)*dN*dEcc*dEcc/pow(dSemi,6);
}

/* Tidal heat for a Maxwell body as described in Table 1 of Henning et al (2009). Note I have not double-checked this expression!! */

double dImk2_Maxwell(double dViscosity,double dShearModulus,double dOmega,double dBeta) {
  return (57*dViscosity*dOmega)/(4*dBeta*(1+(pow(1+(19*dShearModulus/(2*dBeta)),2)))*dViscosity*dViscosity*dOmega*dOmega/(dShearModulus*dShearModulus));
}

double MaxwellHeat(double dMass,double dRadius,double dN,double dEcc,double dSemi,double dViscosity,double dShearModulus) {
  double beta,rho,g,dImk2;

  rho=4*PI*PI/3*dMass/pow(dRadius,3);
  g=BIGG*dMass/(dRadius*dRadius);
  beta=rho*g*dRadius;

  dImk2=dImk2_Maxwell(dViscosity,dShearModulus,dN,beta);
  
  return ViscoelasticHeat(dMass,dRadius,dN,dEcc,dSemi,dImk2);
}

double dImk2_SAS(double dViscosity,double dJu,double dCreep,double dOmega,double dBeta) {
  return (57*dCreep*dCreep*dBeta*dViscosity*dOmega)/( pow((19+2*dBeta*(dJu+dCreep)),2) + pow(dCreep*dViscosity*dOmega*(19+2*dBeta*dJu),2));
}

double StandardAnelasticSolidHeat(double dMass,double dRadius,double dN,double dEcc,double dSemi,double dViscosity,double dJu,double dCreep) {
  double beta,rho,g,dImk2;

  rho=4*PI*PI/3*dMass/pow(dRadius,3);
  g=BIGG*dMass/(dRadius*dRadius);
  beta=rho*g*dRadius;

  dImk2=dImk2_SAS(dViscosity,dJu,dCreep,dN,beta);
  
  return ViscoelasticHeat(dMass,dRadius,dN,dEcc,dSemi,dImk2);
}


/* Burgers Model */

double BurgersC1(double dCreep,double dEtaA,double dEtaB,double dShearModulus) {
  return dCreep + dCreep*dEtaA/dEtaB + 1./dShearModulus;
}

double BurgersC2(double dCreep,double dEtaA,double dEtaB,double dShearModulus,double dOmega) {
  return 1./dEtaB - dEtaA*dCreep*dOmega*dOmega/dShearModulus;
}

double dImk2_Burgers(double dBeta,double dOmega,double dC1,double dC2,double dCreep,double dEtaA,double dEtaB) {
  return (57*dBeta*dOmega*(dC2+dCreep*dOmega*dOmega*dC1*dEtaA))/(361*dOmega*dOmega+76*dBeta*dOmega*dOmega*dC1+4*dBeta*dBeta*dOmega*dOmega*dC1*dC1+pow((2*dBeta*dC2-19*dCreep*dOmega*dOmega*dEtaA),2));
}

double BurgersHeat(double dMass,double dRadius,double dN,double dOmega,double dEcc,double dSemi,double dShearModulus,double dCreep,double dEtaA,double dEtaB) {
  double beta,rho,g,dImk2,dC1,dC2;

  rho=4*PI*PI/3*dMass/pow(dRadius,3);
  g=BIGG*dMass/(dRadius*dRadius);
  beta=rho*g*dRadius;

  dC1=BurgersC1(dCreep,dEtaA,dEtaB,dShearModulus);
  dC2=BurgersC2(dCreep,dEtaA,dEtaB,dShearModulus,dOmega);

  dImk2=dImk2_Burgers(beta,dOmega,dC1,dC2,dCreep,dEtaA,dEtaB);
  
  return ViscoelasticHeat(dMass,dRadius,dN,dEcc,dSemi,dImk2);
}

double NextOutput(double dTime,double dOutputInterval) {
  int nSteps;
  
  /* Number of output so far */
  nSteps = (int)(dTime/dOutputInterval);
  /* Next output is one more */
  return (nSteps+1)*dOutputInterval;
}

int bCheckHalt(PARAM param,PRIMARY pri,SECONDARY sec,double dDeDt,double dTime) {

  /* Double Synchronous? */

    if (pri.bForceEqSpin && (pri.dObliquity <= param.dMinValue) && sec.bForceEqSpin && (sec.dObliquity <= param.dMinValue) && param.halt.bDblSync) {
    if (param.iVerbose >= VERBPROG) {
      printf("HALT: Double Synchronous at ");
      fprintd(stdout,dTime/YEARSEC,param.iSciNot,param.iDigits);
      printf(" years.\n");
    }
    return 1;
  }

  /* Maximum Eccentricity? */

  if (sec.dEcc > param.halt.dMaxEcc) {
    if (param.iVerbose >= VERBPROG) {
      printf("HALT: e = ");
      fprintd(stdout,sec.dEcc,param.iSciNot,param.iDigits);
      printf(", > max e = ");
      fprintd(stdout,param.halt.dMaxEcc,param.iSciNot,param.iDigits);
      printf(" at %.2e years\n",dTime/YEARSEC);
    }
    return 1;
  }

  /* Minimum Eccentricity? */

  if (sec.dEcc < param.halt.dMinEcc) {
    if (param.iVerbose >= VERBPROG) {
      printf("HALT: e = ");
      fprintd(stdout,sec.dEcc,param.iSciNot,param.iDigits);
      printf(", < min e = ");
      fprintd(stdout,param.halt.dMinEcc,param.iSciNot,param.iDigits);
      printf(" at %.2e years\n",dTime/YEARSEC);
    }
    return 1;
  }

  /* Positive de/dt? */

  if (dDeDt > 0 && param.halt.bPosDeDt) {
    if (param.iVerbose >= VERBPROG) {
      printf("HALT: de/dt = ");
      fprintd(stdout,dDeDt,param.iSciNot,param.iDigits);
      printf(" at %.2e years\n",dTime/YEARSEC);
    }
    return 1;
  }

  /* Merge? */

  if (sec.dSemi*(1-sec.dEcc) <= (pri.dRadius + sec.dRadius) && param.halt.bMerge) { /* Merge! */
    if (param.iVerbose > VERBPROG) 
      printf("HALT: Merge at %.2e years!\n",dTime/YEARSEC);

    return 1;
  }

  /* Primary reached minimum obliquity? */

  if (pri.dObliquity < param.halt.dMinPriObl) {
    if (param.iVerbose >= VERBPROG) {
      printf("HALT: Primary Obliquity = ");
      fprintd(stdout,pri.dObliquity,param.iSciNot,param.iDigits);
      printf(", < minimum obliquity = ");
      fprintd(stdout,param.halt.dMinPriObl,param.iSciNot,param.iDigits);
      printf(" at %.2e years.\n",dTime/YEARSEC);
    }
    return 1;
  }

  /* Secondary reached minimum obliquity? */

  if (sec.dObliquity < param.halt.dMinSecObl) {
    if (param.iVerbose >= VERBPROG) {
      printf("HALT: Secondary Obliquity = ");
      fprintd(stdout,sec.dObliquity,param.iSciNot,param.iDigits);
      printf(", < minimum obliquity = ");
      fprintd(stdout,param.halt.dMinSecObl,param.iSciNot,param.iDigits);
      printf(" at %.2e years.\n",dTime/YEARSEC);
    }
    return 1;
  }

  /* Dry? */
  if (sec.bWet == 0 && param.halt.bDry) {
    if (param.iVerbose >= VERBPROG) {
      printf("HALT: Secondary dessicated at ");
      fprintd(stdout,dTime/YEARSEC,param.iSciNot,param.iDigits);
      printf(" years.\n");
    }
    return 1;
  }

  /* Primary Tide-locked? */

  if (pri.bForceEqSpin && param.halt.bPriLock) {
    if (param.iVerbose >= VERBPROG) {
      printf("HALT: Primary tide-locked at ");
      fprintd(stdout,dTime/YEARSEC,param.iSciNot,param.iDigits);
      printf(" years.\n");
    }
    return 1;
  }

  /* Secondary Tide-locked? */

  if (sec.bForceEqSpin && param.halt.bSecLock) {
    if (param.iVerbose >= VERBPROG) {
      printf("HALT: Secondary tide-locked at ");
      fprintd(stdout,dTime/YEARSEC,param.iSciNot,param.iDigits);
      printf(" years.\n");
    }
    return 1;
  }
  
  if (param.halt.bSecSync && sec.dEcc <= sec.dSyncEcc && sec.bForceEqSpin) {
    if (param.iVerbose >= VERBPROG) {
      printf("HALT: Secondary's rotation is synchronous (e = ");
      fprintd(stdout,sec.dEcc,param.iSciNot,param.iDigits);
      printf(") at ");
      fprintd(stdout,dTime/YEARSEC,param.iSciNot,param.iDigits);
      printf(" years.\n");
    }
    return 1;
  }
  return 0;
}

void TideRewind_CPL2(PARAM param,PRIMARY pri,SECONDARY sec,HZ hz,OUTPUT output,FILES files,IO io) 
{
  int **epsilon;
  double *zprime,*chi;
  double dTime,dTimeOut;
  double dDaDt,dDeDt,dDomegaDt[2],dDobliquityDt[2];
  double dTimeOverGrnhsLim,dSurfFlux;
  double dDt;
  FILE *fp;

  epsilon=malloc(2*sizeof(int*));
  epsilon[0]=malloc(9*sizeof(int));
  epsilon[1]=malloc(9*sizeof(int));

  zprime=malloc(2*sizeof(double));
  chi=malloc(2*sizeof(double));

  fp=fopen(files.cBack,"w");
  dTime = 0;
  dDt = param.dBackTimeStep;
  dTimeOut = param.dBackOutputTime;

  /* Dump initial conditions */
  AssignEpsilon(pri.dSpinRate,sec.dMeanMotion,epsilon[0]);
  AssignEpsilon(sec.dSpinRate,sec.dMeanMotion,epsilon[1]);
  AssignChi(pri,sec,chi);
  AssignZprime(pri,sec,zprime);

  dDaDt = dDaDt_CPL2(pri,sec,epsilon,zprime);
  dDeDt = dDeDt_CPL2(pri,sec,epsilon,zprime);
  dDobliquityDt[0] = dDoblDt_CPL2(pri.dMass,pri.dRadius,pri.dSpinRate,sec.dMeanMotion,pri.dRG,epsilon[0],chi[0],pri.dObliquity,zprime[0]);
  dDobliquityDt[1] = dDoblDt_CPL2(sec.dMass,sec.dRadius,sec.dSpinRate,sec.dMeanMotion,sec.dRG,epsilon[1],chi[1],sec.dObliquity,zprime[1]);

  /* Maintain spins at equilibrium rate? */
  /* dDomegaDt[i] is set to TINY to not throw variable timestepping */
  
  if (pri.bForceEqSpin)
    dDomegaDt[0] = TINY;
  else
    dDomegaDt[0] = dDomegaDt_CPL2(pri.dMass,pri.dRadius,sec.dMeanMotion,sec.dEcc,pri.dRG,pri.dObliquity,epsilon[0],zprime[0]);
  
  if (sec.bForceEqSpin)
    dDomegaDt[1] = TINY;
  else
    dDomegaDt[1] = dDomegaDt_CPL2(sec.dMass,sec.dRadius,sec.dMeanMotion,sec.dEcc,sec.dRG,sec.dObliquity,epsilon[1],zprime[1]);

  /* Adjust dt? */
  if (param.bBackVarDt) 
    dDt = AssignDt(pri,sec,dDaDt,dDeDt,dDomegaDt,dDobliquityDt,(dTimeOut - dTime),param.dBackEta);

  /* Set Greenhouse and Q */
  if (param.bGrnhsWaterLoss && sec.bWet) {
      dTimeOverGrnhsLim = 0;
      sec.dQ = sec.dWetQ;
  }

  /* Write out initial conditions */

  Output(param,pri,sec,hz,output,io,-dTime,dDaDt,dDeDt,dDomegaDt,dDobliquityDt,dDt,fp);
  dTimeOut = NextOutput(dTime,param.dBackOutputTime);

  /* Main loop begins here */

  while (dTime <= param.dBackStopTime) {
    AssignEpsilon(pri.dSpinRate,sec.dMeanMotion,epsilon[0]);
    AssignEpsilon(sec.dSpinRate,sec.dMeanMotion,epsilon[1]);
    AssignChi(pri,sec,chi);
    AssignZprime(pri,sec,zprime);

    /* Calculate tidal properties */

    dDaDt = dDaDt_CPL2(pri,sec,epsilon,zprime);
    dDeDt = dDeDt_CPL2(pri,sec,epsilon,zprime);
    dDobliquityDt[0] = dDoblDt_CPL2(pri.dMass,pri.dRadius,pri.dSpinRate,sec.dMeanMotion,pri.dRG,epsilon[0],chi[0],pri.dObliquity,zprime[0]);
    dDobliquityDt[1] = dDoblDt_CPL2(sec.dMass,sec.dRadius,sec.dSpinRate,sec.dMeanMotion,sec.dRG,epsilon[1],chi[1],sec.dObliquity,zprime[1]);

    /* Maintain spins at equilibrium rate? */
    /* dDomegaDt[i] is set to TINY to not throw variable timestepping */

    if (pri.bForceEqSpin)
      dDomegaDt[0] = TINY;
    else
      dDomegaDt[0] = dDomegaDt_CPL2(pri.dMass,pri.dRadius,sec.dMeanMotion,sec.dEcc,pri.dRG,pri.dObliquity,epsilon[0],zprime[0]);

    if (sec.bForceEqSpin) 
      dDomegaDt[1] = TINY;
    else
      dDomegaDt[1] = dDomegaDt_CPL2(sec.dMass,sec.dRadius,sec.dMeanMotion,sec.dEcc,sec.dRG,sec.dObliquity,epsilon[1],zprime[1]);

    /* Adjust dt? */
    if (param.bBackVarDt) 
      dDt = AssignDt(pri,sec,dDaDt,dDeDt,dDomegaDt,dDobliquityDt,(dTimeOut-dTime),param.dBackEta);

    /* Update parameters */
    sec.dSemi -= dDaDt*dDt;
    sec.dMeanMotion = dSemiToMeanMotion(sec.dSemi,(pri.dMass+sec.dMass));
    sec.dEcc -= dDeDt*dDt;
    pri.dObliquity -= dDobliquityDt[0]*dDt;
    sec.dObliquity -= dDobliquityDt[1]*dDt;
    pri.dSpinRate -= dDomegaDt[0]*dDt;
    sec.dSpinRate -= dDomegaDt[1]*dDt;

    dTime += dDt;
    sec.dAge -= dDt;

    /* Now worry about non-tidal aspects */

    /* Check Greenhouse Effects */

    if (param.bGrnhsWaterLoss && sec.bWet) {
      dSurfFlux = dTideHeat_CPL2(epsilon[1],zprime[1],sec.dEcc,sec.dMeanMotion,sec.dSpinRate,sec.dObliquity)/(4*PI*sec.dRadius*sec.dRadius) + dInsolation(pri.dLuminosity,sec.dSemi,sec.dEcc)*(1-sec.dAlbedo)/4;
      if (dSurfFlux >= hz.dRnwyGrnhsFlux)
	dTimeOverGrnhsLim += dDt;

      if (dTimeOverGrnhsLim >= param.dGrnhsWaterLossTime) {
	sec.bWet = 0;
	sec.dQ = sec.dDryQ;
	if (param.iVerbose >= VERBPROG) {
	  printf("Water lost by runaway greenhouse at ");
	  fprintd(stdout,-dTime/YEARSEC,param.iSciNot,param.iDigits);
	  printf(" years.\n");
	}
      }
    }

    /* Adjust radii, if necessary */

    if (pri.iRadContr) {
      pri.dSpinRate -= dDSpinRateContrDt(pri.dRadius,pri.dSpinRate,pri.iRadContr,sec.dAge)*dDt;
      pri.dRadius -= dDRadDtContr(sec.dAge,pri.iRadContr)*dDt;
    }

    if (sec.iRadContr) {
      sec.dSpinRate -= dDSpinRateContrDt(sec.dRadius,sec.dSpinRate,sec.iRadContr,sec.dAge)*dDt;
      sec.dRadius -= dDRadDtContr(sec.dAge,sec.iRadContr)*dDt;
    }

     /* Halt? */
    if (param.halt.bHalt) {
      if (bCheckHalt(param,pri,sec,dDeDt,-dTime)) {
	Output(param,pri,sec,hz,output,io,dTime,dDaDt,dDeDt,dDomegaDt,dDobliquityDt,dDt,fp);
	return;
      }
    }

    /* Time for Output? */
    if (dTime >= dTimeOut) {
      Output(param,pri,sec,hz,output,io,-dTime,dDaDt,dDeDt,dDomegaDt,dDobliquityDt,dDt,fp);
      dTimeOut = NextOutput(dTime,param.dBackOutputTime);
    }

  }
  free(epsilon);
  free(chi);
  free(zprime);
    
}

/*
 *
 * TideForward CPL2
 *
 */


void TideForward_CPL2(PARAM param,PRIMARY pri,SECONDARY sec,HZ hz,OUTPUT output,FILES files,IO io) {
 
  int **epsilon,nsteps=0;
  double *zprime,*chi;
  double dTime,dTimeOut;
  double dDaDt,dDeDt,dDomegaDt[2],dDobliquityDt[2];
  double dTimeOverGrnhsLim,dSurfFlux;
  double dDt;
  double dEqSpinRate;
  FILE *fp;

  epsilon=malloc(2*sizeof(double*));
  epsilon[0]=malloc(9*sizeof(double));
  epsilon[1]=malloc(9*sizeof(double));

  zprime=malloc(2*sizeof(double));
  chi=malloc(2*sizeof(double));

  /* Set Greenhouse and Q */
  if (param.bGrnhsWaterLoss && sec.bWet) {
      dTimeOverGrnhsLim = 0;
      sec.dQ = sec.dWetQ;
  }

  fp=fopen(files.cFor,"w");
  dTime = 0;
  dTimeOut = param.dForwOutputTime;
  dDt = param.dForwTimeStep;

  /* Dump initial conditions */
  AssignEpsilon(pri.dSpinRate,sec.dMeanMotion,epsilon[0]);
  AssignEpsilon(sec.dSpinRate,sec.dMeanMotion,epsilon[1]);
  AssignChi(pri,sec,chi);
  AssignZprime(pri,sec,zprime);

  dDaDt = dDaDt_CPL2(pri,sec,epsilon,zprime);
  dDeDt = dDeDt_CPL2(pri,sec,epsilon,zprime);

  dDomegaDt[0] = dDomegaDt_CPL2(pri.dMass,pri.dRadius,sec.dMeanMotion,sec.dEcc,pri.dRG,pri.dObliquity,epsilon[0],zprime[0]);
  dDomegaDt[1] = dDomegaDt_CPL2(sec.dMass,sec.dRadius,sec.dMeanMotion,sec.dEcc,sec.dRG,sec.dObliquity,epsilon[1],zprime[1]);
    
  dDobliquityDt[0] = dDoblDt_CPL2(pri.dMass,pri.dRadius,pri.dSpinRate,sec.dMeanMotion,pri.dRG,epsilon[0],chi[0],pri.dObliquity,zprime[0]);
  dDobliquityDt[1] = dDoblDt_CPL2(sec.dMass,sec.dRadius,sec.dSpinRate,sec.dMeanMotion,sec.dRG,epsilon[1],chi[1],sec.dObliquity,zprime[1]);

  /* Adjust dt? */
  if (param.bForwVarDt) 
    dDt = AssignDt(pri,sec,dDaDt,dDeDt,dDomegaDt,dDobliquityDt,(dTimeOut - dTime),param.dForwEta);

  /* Write out initial conditions */

  Output(param,pri,sec,hz,output,io,dTime,dDaDt,dDeDt,dDomegaDt,dDobliquityDt,dDt,fp);
  dTimeOut = NextOutput(dTime,param.dForwOutputTime);

  /* Main loop begins here */

  while (dTime < param.dForwStopTime) {
    AssignEpsilon(pri.dSpinRate,sec.dMeanMotion,epsilon[0]);
    AssignEpsilon(sec.dSpinRate,sec.dMeanMotion,epsilon[1]);
    AssignChi(pri,sec,chi);
    AssignZprime(pri,sec,zprime);

    /* Calculate Time Rates of Change */

    /* a and e */

    dDaDt = dDaDt_CPL2(pri,sec,epsilon,zprime);
    dDeDt = dDeDt_CPL2(pri,sec,epsilon,zprime);

    /* Spin Rates */

    /* Lock the primary? */

    if (pri.bForceEqSpin) /* yes */
	dDomegaDt[0] = TINY;
    else {
	if (fabs((pri.dSpinRate - EqSpinRate_CPL2(sec.dMeanMotion,sec.dEcc,param.bDiscreteRot))/EqSpinRate_CPL2(sec.dMeanMotion,sec.dEcc,param.bDiscreteRot)) < pri.dMaxLockDiff) { /* yes */
	    pri.bForceEqSpin = 1; /* This forces it to be locked from now on */
	    dDomegaDt[0] = TINY;
	    if (param.iVerbose >= VERBPROG) {
		printf("Primary spin locked at ");
		fprintd(stdout,dTime/YEARSEC,param.iSciNot,param.iDigits);
		printf(" years.\n");
	    }
	} else { /* no */
	    dDomegaDt[0] = dDomegaDt_CPL2(pri.dMass,pri.dRadius,sec.dMeanMotion,sec.dEcc,pri.dRG,pri.dObliquity,epsilon[0],zprime[0]);
	}
    }
    /* Lock the Secondary? */

    if (sec.bForceEqSpin) /* yes */
	dDomegaDt[1] = TINY;
    else {
      dEqSpinRate = EqSpinRate_CPL2(sec.dMeanMotion,sec.dEcc,param.bDiscreteRot);
	if (fabs((sec.dSpinRate - EqSpinRate_CPL2(sec.dMeanMotion,sec.dEcc,param.bDiscreteRot))/EqSpinRate_CPL2(sec.dMeanMotion,sec.dEcc,param.bDiscreteRot)) < sec.dMaxLockDiff) { /* yes */
	    sec.bForceEqSpin = 1;
	    dDomegaDt[1] = TINY;
	    if (param.iVerbose >= VERBPROG) {
		printf("Secondary spin locked at ");
		fprintd(stdout,dTime/YEARSEC,param.iSciNot,param.iDigits);
		printf(" years.\n");
	    }
	} else { /* no */
		dDomegaDt[1] = dDomegaDt_CPL2(sec.dMass,sec.dRadius,sec.dMeanMotion,sec.dEcc,sec.dRG,sec.dObliquity,epsilon[1],zprime[1]);
	}
    }	

    /* Obliquities */

    dDobliquityDt[0] = dDoblDt_CPL2(pri.dMass,pri.dRadius,pri.dSpinRate,sec.dMeanMotion,pri.dRG,epsilon[0],chi[0],pri.dObliquity,zprime[0]);
    dDobliquityDt[1] = dDoblDt_CPL2(sec.dMass,sec.dRadius,sec.dSpinRate,sec.dMeanMotion,sec.dRG,epsilon[1],chi[1],sec.dObliquity,zprime[1]);

    /* Adjust dt? */
    if (param.bForwVarDt) 
	dDt = AssignDt(pri,sec,dDaDt,dDeDt,dDomegaDt,dDobliquityDt,(dTimeOut-dTime),param.dForwEta);

    /* Update a, e, and obliquities */
    sec.dSemi += dDaDt*dDt;
    sec.dMeanMotion = dSemiToMeanMotion(sec.dSemi,(pri.dMass+sec.dMass));
    sec.dEcc += dDeDt*dDt;
    pri.dObliquity += dDobliquityDt[0]*dDt;
    sec.dObliquity += dDobliquityDt[1]*dDt;

    if (pri.bForceEqSpin)
	pri.dSpinRate = EqSpinRate_CPL2(sec.dMeanMotion,sec.dEcc,param.bDiscreteRot);
    else 
	pri.dSpinRate += dDomegaDt[0]*dDt;

    if (sec.bForceEqSpin)
	sec.dSpinRate = EqSpinRate_CPL2(sec.dMeanMotion,sec.dEcc,param.bDiscreteRot);
    else	
	sec.dSpinRate += dDomegaDt[1]*dDt;



    dTime += dDt;
    sec.dAge += dDt;

    nsteps++;

    /* Now worry about non-tidal aspects */

    /* Check Greenhouse Effects */

    if (param.bGrnhsWaterLoss && sec.bWet) {
      dSurfFlux = dTideHeat_CPL2(epsilon[1],zprime[1],sec.dEcc,sec.dMeanMotion,sec.dSpinRate,sec.dObliquity)/(4*PI*sec.dRadius*sec.dRadius) + dInsolation(pri.dLuminosity,sec.dSemi,sec.dEcc)*(1-sec.dAlbedo)/4;
      if (dSurfFlux >= hz.dRnwyGrnhsFlux)
	dTimeOverGrnhsLim += dDt;
	
      if (dTimeOverGrnhsLim >= param.dGrnhsWaterLossTime) {
	sec.bWet = 0;
	sec.dQ = sec.dDryQ;
	if (param.iVerbose >= VERBPROG) {
	  printf("Water lost by runaway greenhouse at ");
	  fprintd(stdout,dTime/YEARSEC,param.iSciNot,param.iDigits);
	  printf(" years.\n");
	}
      }
    }

    /* Halt? */
    if (param.halt.bHalt) {
      if (bCheckHalt(param,pri,sec,dDeDt,dTime)) {
	Output(param,pri,sec,hz,output,io,dTime,dDaDt,dDeDt,dDomegaDt,dDobliquityDt,dDt,fp);
	return;
      }
    }

    /* If small enough, set some quantities to zero */
    if (param.dMinValue > 0) {
      if (sec.dEcc < param.dMinValue)
	sec.dEcc = 0;
      if (sec.dObliquity < param.dMinValue)
	sec.dObliquity = 0;
      if (pri.dObliquity < param.dMinValue)
	pri.dObliquity = 0;
    }

    /* Time for Output? */
    if (dTime >= dTimeOut) {
      Output(param,pri,sec,hz,output,io,dTime,dDaDt,dDeDt,dDomegaDt,dDobliquityDt,param.dForwOutputTime/nsteps,fp);
      dTimeOut = NextOutput(dTime,param.dForwOutputTime);
      nsteps=0;
    }

  }


  free(epsilon);
  free(chi);
  free(zprime);
  

}

void TideRewind_CTL8 (PARAM param,PRIMARY pri,SECONDARY sec,HZ hz,OUTPUT output,FILES files,IO io) {
  int nsteps=0;
  double *z,*chi;
  double dTime,dTimeOut;
  double dBeta,f[5];
  double dDaDt,dDeDt,dDomegaDt[2],dDobliquityDt[2];
  double dTimeOverGrnhsLim,dSurfFlux;
  double dDt;
  FILE *fp;

  z=malloc(2*sizeof(double));
  chi=malloc(2*sizeof(double));

  fp=fopen(files.cBack,"w");
  dTime=0;
  dTimeOut=0;

  /* Dump initial conditions */
  AssignChi(pri,sec,chi);
  AssignZ(pri,sec,z);
  dBeta = AssignBeta(sec.dEcc);
  f[0] = AssignF1(sec.dEcc);
  f[1] = AssignF2(sec.dEcc);
  f[2] = AssignF3(sec.dEcc);
  f[3] = AssignF4(sec.dEcc);
  f[4] = AssignF5(sec.dEcc);
  dDaDt = dDaDt_CTL8(pri,sec,z,dBeta,f);
  dDeDt = dDeDt_CTL8(pri,sec,z,dBeta,f);
  dDobliquityDt[0] = dDoblDt_CTL8(pri.dMass,pri.dRadius,sec.dMeanMotion,sec.dEcc,pri.dRG,pri.dSpinRate,pri.dObliquity,z[0],dBeta,f,chi[0]);
  dDobliquityDt[1] = dDoblDt_CTL8(sec.dMass,sec.dRadius,sec.dMeanMotion,sec.dEcc,sec.dRG,sec.dSpinRate,sec.dObliquity,z[1],dBeta,f,chi[1]);

  /* Maintain spins at equilibrium rate? */
  /* dDomegaDt[i] is set to TINY to not throw variable timestepping */
  
  if (pri.bForceEqSpin)
    dDomegaDt[0] = TINY;
  else 
    dDomegaDt[0] = dDomegaDt_CTL8(pri.dMass,pri.dRadius,sec.dMeanMotion,sec.dEcc,pri.dRG,pri.dSpinRate,pri.dObliquity,z[0],dBeta,f);
  
  if (sec.bForceEqSpin)
    dDomegaDt[1] = TINY;
  else
    dDomegaDt[1] = dDomegaDt_CTL8(sec.dMass,sec.dRadius,sec.dMeanMotion,sec.dEcc,sec.dRG,sec.dSpinRate,sec.dObliquity,z[1],dBeta,f);

  /* Adjust dt? */
  if (param.bBackVarDt) 
    dDt = AssignDt(pri,sec,dDaDt,dDeDt,dDomegaDt,dDobliquityDt,(dTimeOut - dTime),param.dBackEta);
  else
    dDt=param.dBackTimeStep;

  /* Set Greenhouse and Tau */
  if (param.bGrnhsWaterLoss && sec.bWet) {
      dTimeOverGrnhsLim = 0;
      sec.dTau = sec.dWetTau;
  }

  Output(param,pri,sec,hz,output,io,-dTime,dDaDt,dDeDt,dDomegaDt,dDobliquityDt,dDt,fp);
  dTimeOut = NextOutput(dTime,param.dBackOutputTime);

  /* 
   * Integration Loop Begins Here 
   */
  
  while (dTime <= param.dBackStopTime) {
    AssignChi(pri,sec,chi);
    AssignZ(pri,sec,z);
    dBeta = AssignBeta(sec.dEcc);
    f[0] = AssignF1(sec.dEcc);
    f[1] = AssignF2(sec.dEcc);
    f[2] = AssignF3(sec.dEcc);
    f[3] = AssignF4(sec.dEcc);
    f[4] = AssignF5(sec.dEcc);

    /* Calculate tidal properties */
    
    dDaDt = dDaDt_CTL8(pri,sec,z,dBeta,f);
    dDeDt = dDeDt_CTL8(pri,sec,z,dBeta,f);
    dDomegaDt[1] = dDomegaDt_CTL8(sec.dMass,sec.dRadius,sec.dMeanMotion,sec.dEcc,sec.dRG,sec.dSpinRate,sec.dObliquity,z[1],dBeta,f);
    dDobliquityDt[0] = dDoblDt_CTL8(pri.dMass,pri.dRadius,sec.dMeanMotion,sec.dEcc,pri.dRG,pri.dSpinRate,pri.dObliquity,z[0],dBeta,f,chi[0]);
    dDobliquityDt[1] = dDoblDt_CTL8(sec.dMass,sec.dRadius,sec.dMeanMotion,sec.dEcc,sec.dRG,sec.dSpinRate,sec.dObliquity,z[1],dBeta,f,chi[1]);

    /* Maintain spins at equilibrium rate? */
    /* dDomegaDt[i] is set to TINY to not throw variable timestepping */
    if (pri.bForceEqSpin)
      dDomegaDt[0] = TINY;
    else 
      dDomegaDt[0] = dDomegaDt_CTL8(pri.dMass,pri.dRadius,sec.dMeanMotion,sec.dEcc,pri.dRG,pri.dSpinRate,pri.dObliquity,z[0],dBeta,f);
    
    if (sec.bForceEqSpin)
      dDomegaDt[1] = TINY;
    else
      dDomegaDt[1] = dDomegaDt_CTL8(sec.dMass,sec.dRadius,sec.dMeanMotion,sec.dEcc,sec.dRG,sec.dSpinRate,sec.dObliquity,z[1],dBeta,f);

    /* Adjust dt? */
    if (param.bBackVarDt) 
      dDt = AssignDt(pri,sec,dDaDt,dDeDt,dDomegaDt,dDobliquityDt,(dTimeOut-dTime),param.dBackEta);

    /* Update parameters */
    sec.dSemi -= dDaDt*dDt;
    sec.dMeanMotion = dSemiToMeanMotion(sec.dSemi,(pri.dMass+sec.dMass));
    sec.dEcc -= dDeDt*dDt;
    pri.dObliquity -= dDobliquityDt[0]*dDt;
    sec.dObliquity -= dDobliquityDt[1]*dDt;
    pri.dSpinRate -= dDomegaDt[0]*dDt;
    sec.dSpinRate -= dDomegaDt[1]*dDt;

    dTime += dDt;
    sec.dAge -= dDt;

    /* Now worry about non-tidal aspects */

    /* Check Greenhouse Effects */

    if (param.bGrnhsWaterLoss && sec.bWet) {
      dSurfFlux = dTideHeat_CTL8(z[1],f,dBeta,sec.dSpinRate,sec.dObliquity,sec.dMeanMotion)/(4*PI*sec.dRadius*sec.dRadius) + dInsolation(pri.dLuminosity,sec.dSemi,sec.dEcc)*(1-sec.dAlbedo)/4;
      if (dSurfFlux >= hz.dRnwyGrnhsFlux)
	dTimeOverGrnhsLim += dDt;

      if (dTimeOverGrnhsLim >= param.dGrnhsWaterLossTime) {
	sec.bWet = 0;
	sec.dTau = sec.dDryTau;
	if (param.iVerbose >= VERBPROG) {
	  printf("Water lost by runaway greenhouse at ");
	  fprintd(stdout,-dTime/YEARSEC,param.iSciNot,param.iDigits);
	  printf(" years.\n");
	}
      }
    }

    /* Include other Primary effects here */ 

    if (pri.iRadContr) {
      pri.dSpinRate -= dDSpinRateContrDt(pri.dRadius,pri.dSpinRate,pri.iRadContr,sec.dAge)*dDt;
      pri.dRadius -= dDRadDtContr(sec.dAge,pri.iRadContr)*dDt;
    }

    /* Include other Secondary effects here */

    if (sec.iRadContr) {
      sec.dSpinRate -= dDSpinRateContrDt(sec.dRadius,sec.dSpinRate,sec.iRadContr,sec.dAge)*dDt;
      sec.dRadius -= dDRadDtContr(sec.dAge,sec.iRadContr)*dDt;
    }

    /* Halt? */
    if (param.halt.bHalt) {
      if (bCheckHalt(param,pri,sec,dDeDt,-dTime)) {
	Output(param,pri,sec,hz,output,io,-dTime,dDaDt,dDeDt,dDomegaDt,dDobliquityDt,dDt,fp);
	return;
      }
    }

    /* Time for Output? */
    if (dTime >= dTimeOut) {
      /* Add f, z and epsilon to argument list */
      Output(param,pri,sec,hz,output,io,-dTime,dDaDt,dDeDt,dDomegaDt,dDobliquityDt,dDt,fp);
	dTimeOut = NextOutput(dTime,param.dBackOutputTime);
    }

    if (sec.dSemi*(1-sec.dEcc) <= (pri.dRadius + sec.dRadius)) { /* Merge! */
      printf("Merge at %.2e years!\n",-dTime/YEARSEC);
      return;
    }
	
  }
  free(z);
  free(chi);
  fclose(fp);
}


void TideForward_CTL8 (PARAM param,PRIMARY pri,SECONDARY sec,HZ hz,OUTPUT output,FILES files,IO io) {
  int bHalt=0,nsteps=0;
  double *z,*chi;
  double dTime,dTimeOut;
  double dBeta,f[5];
  double dDaDt,dDeDt,dDomegaDt[2],dDobliquityDt[2];
  double dTimeOverGrnhsLim,dSurfFlux;
  double dDt,dEqSpinRate;
  FILE *fp;

  z=malloc(2*sizeof(double));
  chi=malloc(2*sizeof(double));

  fp=fopen(files.cFor,"w");
  dTime=0;
  dTimeOut=0;
  dDt=param.dForwTimeStep;

  /* Set Greenhouse and Q */
  if (param.bGrnhsWaterLoss && sec.bWet) {
      dTimeOverGrnhsLim = 0;
      sec.dTau = sec.dWetTau;
  }

  AssignChi(pri,sec,chi);
  AssignZ(pri,sec,z);
  dBeta = AssignBeta(sec.dEcc);
  f[0] = AssignF1(sec.dEcc);
  f[1] = AssignF2(sec.dEcc);
  f[2] = AssignF3(sec.dEcc);
  f[3] = AssignF4(sec.dEcc);
  f[4] = AssignF5(sec.dEcc);
  
  dDaDt = dDaDt_CTL8(pri,sec,z,dBeta,f);
  dDeDt = dDeDt_CTL8(pri,sec,z,dBeta,f);
  
  dDomegaDt[0] = dDomegaDt_CTL8(pri.dMass,pri.dRadius,sec.dMeanMotion,sec.dEcc,pri.dRG,pri.dSpinRate,pri.dObliquity,z[0],dBeta,f);
  dDomegaDt[1] = dDomegaDt_CTL8(sec.dMass,sec.dRadius,sec.dMeanMotion,sec.dEcc,sec.dRG,sec.dSpinRate,sec.dObliquity,z[1],dBeta,f);
  
  dDobliquityDt[0] = dDoblDt_CTL8(pri.dMass,pri.dRadius,sec.dMeanMotion,sec.dEcc,pri.dRG,pri.dSpinRate,pri.dObliquity,z[0],dBeta,f,chi[0]);
  dDobliquityDt[1] = dDoblDt_CTL8(sec.dMass,sec.dRadius,sec.dMeanMotion,sec.dEcc,sec.dRG,sec.dSpinRate,sec.dObliquity,z[1],dBeta,f,chi[1]);


  /* Adjust dt? */
  if (param.bForwVarDt) 
    dDt = AssignDt(pri,sec,dDaDt,dDeDt,dDomegaDt,dDobliquityDt,(dTimeOut - dTime),param.dForwEta);

  /* Write out initial conditions */

  Output(param,pri,sec,hz,output,io,dTime,dDaDt,dDeDt,dDomegaDt,dDobliquityDt,dDt,fp);
  dTimeOut = NextOutput(dTime,param.dForwOutputTime);

  /* Main loop begins here */



  while (dTime < param.dForwStopTime) {
    AssignChi(pri,sec,chi);
    AssignZ(pri,sec,z);
    dBeta = AssignBeta(sec.dEcc);
    f[0] = AssignF1(sec.dEcc);
    f[1] = AssignF2(sec.dEcc);
    f[2] = AssignF3(sec.dEcc);
    f[3] = AssignF4(sec.dEcc);
    f[4] = AssignF5(sec.dEcc);

    /* Calculate tidal properties */
    
    dDaDt = dDaDt_CTL8(pri,sec,z,dBeta,f);
    dDeDt = dDeDt_CTL8(pri,sec,z,dBeta,f);

    if (pri.bForceEqSpin) /* yes */
        dDomegaDt[0] = TINY;
    else {
        if (fabs((pri.dSpinRate - EqSpinRate_CTL8(sec.dMeanMotion,sec.dEcc,pri.dObliquity))/EqSpinRate_CTL8(sec.dMeanMotion,sec.dEcc,pri.dObliquity)) < pri.dMaxLockDiff) { /* yes */
            pri.bForceEqSpin = 1; /* This forces is to be locked from now on */
            dDomegaDt[0] = TINY;
            if (param.iVerbose >= VERBPROG) {
                printf("Primary spin locked at ");
                fprintd(stdout,dTime/YEARSEC,param.iSciNot,param.iDigits);
                printf(" years.\n");
            }
        } else { /* no */
	    dDomegaDt[0] = dDomegaDt_CTL8(pri.dMass,pri.dRadius,sec.dMeanMotion,sec.dEcc,pri.dRG,pri.dSpinRate,pri.dObliquity,z[0],dBeta,f);
	}
    }
 
    /* Lock the Secondary? */

    if (sec.bForceEqSpin) /* yes */
        dDomegaDt[1] = TINY;
    else {
        if (fabs((sec.dSpinRate - EqSpinRate_CTL8(sec.dMeanMotion,sec.dEcc,sec.dObliquity))/EqSpinRate_CTL8(sec.dMeanMotion,sec.dEcc,sec.dObliquity)) < sec.dMaxLockDiff) { /* yes */
            sec.bForceEqSpin = 1;
            dDomegaDt[0] = TINY;
            if (param.iVerbose >= VERBPROG) {
                printf("Secondary spin locked at ");
                fprintd(stdout,dTime/YEARSEC,param.iSciNot,param.iDigits);
                printf(" years.\n");
            }
        } else { /* no */
	    dDomegaDt[1] = dDomegaDt_CTL8(sec.dMass,sec.dRadius,sec.dMeanMotion,sec.dEcc,sec.dRG,sec.dSpinRate,sec.dObliquity,z[1],dBeta,f);
	}
    }

    /* Obliquities */

    dDobliquityDt[0] = dDoblDt_CTL8(pri.dMass,pri.dRadius,sec.dMeanMotion,sec.dEcc,pri.dRG,pri.dSpinRate,pri.dObliquity,z[0],dBeta,f,chi[0]);
    dDobliquityDt[1] = dDoblDt_CTL8(sec.dMass,sec.dRadius,sec.dMeanMotion,sec.dEcc,sec.dRG,sec.dSpinRate,sec.dObliquity,z[1],dBeta,f,chi[1]);

    /* Adjust dt? */
    if (param.bForwVarDt) 
	dDt = AssignDt(pri,sec,dDaDt,dDeDt,dDomegaDt,dDobliquityDt,(dTimeOut-dTime),param.dForwEta);

    /* Update parameters */
    sec.dSemi += dDaDt*dDt;
    sec.dMeanMotion = dSemiToMeanMotion(sec.dSemi,(pri.dMass+sec.dMass));
    sec.dEcc += dDeDt*dDt;
    pri.dObliquity += dDobliquityDt[0]*dDt;
    sec.dObliquity += dDobliquityDt[1]*dDt;
    
    if (pri.bForceEqSpin)
	pri.dSpinRate = EqSpinRate_CTL8(sec.dMeanMotion,sec.dEcc,pri.dObliquity);
    else 
	pri.dSpinRate += dDomegaDt[0]*dDt;

    if (sec.bForceEqSpin)
	sec.dSpinRate = EqSpinRate_CTL8(sec.dMeanMotion,sec.dEcc,sec.dObliquity);
    else	
	sec.dSpinRate += dDomegaDt[1]*dDt;

    dTime += dDt;
    sec.dAge += dDt;

    /* Now worry about non-tidal aspects */

    /* Check Greenhouse Effects */

    if (param.bGrnhsWaterLoss && sec.bWet) {
      dSurfFlux = dTideHeat_CTL8(z[1],f,dBeta,sec.dSpinRate,sec.dObliquity,sec.dMeanMotion)/(4*PI*sec.dRadius*sec.dRadius) + dInsolation(pri.dLuminosity,sec.dSemi,sec.dEcc)*(1-sec.dAlbedo)/4;
      if (dSurfFlux >= hz.dRnwyGrnhsFlux)
	dTimeOverGrnhsLim += dDt;

      if (dTimeOverGrnhsLim >= param.dGrnhsWaterLossTime) {
	sec.bWet = 0;
	sec.dTau = sec.dDryTau;
	if (param.iVerbose >= VERBPROG) {
	  printf("Water lost by runaway greenhouse at ");
	  fprintd(stdout,dTime/YEARSEC,param.iSciNot,param.iDigits);
	  printf(" years.\n");
	}
      }
    }

    /* Adjust White Dwarf Luminosity */

    if (pri.bWD) {
      pri.dLuminosity = dBergeronWDLum(sec.dAge);
      pri.dTeff = dTeff(pri.dLuminosity,pri.dRadius);
    }

    nsteps++;

    /* Halt? */
    if (param.halt.bHalt) {
      if (bCheckHalt(param,pri,sec,dDeDt,dTime)) {
	Output(param,pri,sec,hz,output,io,dTime,dDaDt,dDeDt,dDomegaDt,dDobliquityDt,dDt,fp);
	return;
      }
    }

    /* If small enough, set some quantities to zero */
    if (param.dMinValue > 0) {
      if (sec.dEcc < param.dMinValue)
	sec.dEcc = 0;
      if (sec.dObliquity < param.dMinValue)
	sec.dObliquity = 0;
      if (pri.dObliquity < param.dMinValue)
	pri.dObliquity = 0;
    }

    /* Time for Output? */
    if (dTime >= dTimeOut) {

      Output(param,pri,sec,hz,output,io,dTime,dDaDt,dDeDt,dDomegaDt,dDobliquityDt,dDt,fp);
      dTimeOut = NextOutput(dTime,param.dForwOutputTime);
      nsteps=0;

    }



  }

  free(z);
  free(chi);
}

