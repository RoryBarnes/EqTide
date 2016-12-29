/**************************************************************************
 * eqtide.c                                                               *
 *                                                                        *
 * Written by Rory Barnes                                                 * 
 *                                                                        *
 * This file contains subroutines to calcualte the tidal properties and   *
 * integrate the parameters forward or backward. The main subroutine      *
 * is located at the end of this file.                                    *
 *                                                                        *
 **************************************************************************/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "eqtide.h"
#include "output.h"

/*
void AssignChi(PRIMARY *pri,SECONDARY *sec,double *c) {
  c[0]=pri->dRG*pri->dRG*pri->dRadius*pri->dRadius*pri->dSpinRate*sec->dSemi*sec->dMeanMotion/(BIGG*sec->dMass);
  c[1]=sec->dRG*sec->dRG*sec->dRadius*sec->dRadius*sec->dSpinRate*sec->dSemi*sec->dMeanMotion/(BIGG*pri->dMass);
}
*/

/* CCCCCC  PPPPPPP  L
 * C       P     P  L
 * C       PPPPPPP  L
 * C       P        L
 * C       P        L
 * CCCCCC  P        LLLLLL
 */

/* 
 * The following subroutine describes the constant-phase-lag model of order 
 * 2. See Heller, Leconte & Barnes (2011), Eqs. 3-10.
 *
 */

/* EQTIDE splits the evolution of each independent variable into a constant
   and evolving part. Constants need only be calculate at the beginning of 
   the integration, or if a body becomes tidally locked. Thus the functions
   "dDxDt" return only the portion of the term that contains evolving
   variables. */

/* The CPL2 model is governed by equation that can be greatly simplified if
   tidal locking occurs. The proper function is chosen at the start of the 
   integration. During forward integrations, the "free" functions will be 
   replaced by "lock" functions.
*/


// Mean Motion, not locked
double dDnDtCPL2Free(PRIMARY *pri,SECONDARY *sec,int *epsilon,double constant,int iBody) {
   double psi;
   
   if (iBody)
     psi=sec->dSinObl;
   else
     psi=pri->dSinObl;

    return (4*epsilon[0] + sec->dEcc*sec->dEcc*(-20*epsilon[0] + 73.5*epsilon[1] + 0.5*epsilon[2] - 3*epsilon[5]) - 4*psi*psi*(epsilon[0]-epsilon[8]));
}

// Mean Motion, locked
double dDnDtCPL2Lock(PRIMARY *pri,SECONDARY *sec,int *epsilon,double constant,int iBody) {
  double psi;

  if (iBody)
    psi=sec->dSinObl;
  else
    psi=pri->dSinObl;

  return constant*(7*sec->dEcc*sec->dEcc + psi*psi)*epsilon[2];
}

// Eccentricity, not locked
double dDeDtCPL2Free(PRIMARY *pri,SECONDARY *sec,int *epsilon,double constant,int iBody) {
   return constant*(2*epsilon[0] - 24.5*epsilon[1] + 0.5*epsilon[2] + 3*epsilon[5]);
}
   
// Eccentricity, locked
double dDeDtCPL2Lock(PRIMARY *pri,SECONDARY *sec,int *epsilon,double constant,int iBody) {
   return constant*epsilon[2];
}

// Spin rate, not locked
double dDomegaDtCPL2Free(PRIMARY *pri,SECONDARY *sec,int *epsilon,double constant,int iBody) {
  double psi;
  
  if (iBody)
    psi=sec->dSinObl;
  else
    psi=pri->dSinObl;
  
  return 4*epsilon[0] + sec->dEcc*sec->dEcc*(-20*epsilon[0] + 49*epsilon[1] + epsilon[2]) + 2*psi*psi*(-2*epsilon[0]+epsilon[8]+epsilon[9]);
}

// Obliquity, locked
double dDoblDtCPL2Free(PRIMARY *pri,SECONDARY *sec,int *epsilon,double constant,int iBody) {
  return constant*(-epsilon[0] + epsilon[8] + epsilon[9]);
}

// Obliquity, not locked
double dDoblDtCPL2Lock(PRIMARY *pri,SECONDARY *sec,int *epsilon,double constant,int iBody) {
  return constant*epsilon[2];
}

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

double EqSpinRate_CPL2(double n,double e,double foo,int bDiscrete) {
  if (bDiscrete)
    return EqSpinRate_CPL2Discrete(n,e);
  else 
    return EqSpinRate_CPL2Cont(n,e);
}

void AssignEpsilon(double om,double n,int *epsilon) {

  int i;
  epsilon[0]=iSign(om-n);
  epsilon[1]=iSign(2*om-3*n);
  epsilon[2]=iSign(2*om-n);
  epsilon[5]=iSign(n);
  epsilon[8]=iSign(om-2*n);
  epsilon[9]=iSign(om);
}

/*
void AssignZprime(PRIMARY *pri,SECONDARY *sec,double *z) {

  z[0] = 3*BIGG*BIGG*pri->dK2*sec->dMass*sec->dMass*(pri->dMass+sec->dMass)*pow(pri->dRadius,5)/(pow(sec->dSemi,9)*sec->dMeanMotion*pri->dQ);
  z[1] = 3*BIGG*BIGG*sec->dK2*pri->dMass*pri->dMass*(pri->dMass+sec->dMass)*pow(sec->dRadius,5)/(pow(sec->dSemi,9)*sec->dMeanMotion*sec->dQ);
}
*/

void CalculateConstantsCPL2(PRIMARY *pri,SECONDARY *sec,double **consts) {
  /* Mapping for constants:
     MeanMotion   -> 0
     Eccentricity -> 1
     SpinRate     -> 2
     Obliquity    -> 3
  */

  if (pri->bSynchronous) {
    consts[0][0] = 9*pri->dK2*sec->dMass*pow(pri->dRadius,5)/(2*pri->dMass)*pow(BIGG*(pri->dMass+sec->dMass),-5./3);
    consts[0][1] = -21*pri->dK2*sec->dMass*pow(pri->dRadius,5)/(2*pri->dMass)*pow(BIGG*(pri->dMass+sec->dMass),-5./3);
    consts[0][3] = -1.5*pri->dK2*sec->dMass*pow(pri->dRadius,5)/pri->dMass*pow(BIGG*(pri->dMass+sec->dMass),-5./3);
  } else {
    consts[0][0] = -9*pri->dK2*sec->dMass*pow(pri->dRadius,5)/(8*pri->dMass)*pow(BIGG*(pri->dMass+sec->dMass),-5./3);
    consts[0][1] = -3*pri->dK2*sec->dMass*pow(pri->dRadius,5)/(8*pri->dMass)*pow(BIGG*(pri->dMass+sec->dMass),-5./3);
    consts[0][2] = -3*pri->dK2*sec->dMass*sec->dMass*pow(pri->dRadius,3)/(8*pri->dRG*pri->dRG*BIGG*pow((pri->dMass+sec->dMass),2));
    consts[0][3] = 3*pri->dK2*sec->dMass*pow(pri->dRadius,5)/(4*pri->dMass)*pow(BIGG*(pri->dMass+sec->dMass),-5./3);
  }

  if (sec->bSynchronous) {
    consts[1][0] = 9*sec->dK2*pri->dMass*pow(sec->dRadius,5)/(2*sec->dMass)*pow(BIGG*(pri->dMass+sec->dMass),-5./3);
    consts[1][1] = -21*sec->dK2*pri->dMass*pow(sec->dRadius,5)/(2*sec->dMass)*pow(BIGG*(pri->dMass+sec->dMass),-5./3);
    consts[1][3] = -1.5*sec->dK2*pri->dMass*pow(sec->dRadius,5)/sec->dMass*pow(BIGG*(pri->dMass+sec->dMass),-5./3);
  } else {
    consts[1][0] = -9*sec->dK2*pri->dMass*pow(sec->dRadius,5)/(8*sec->dMass)*pow(BIGG*(pri->dMass+sec->dMass),-5./3);
    consts[1][1] = -3*sec->dK2*pri->dMass*pow(sec->dRadius,5)/(8*sec->dMass)*pow(BIGG*(pri->dMass+sec->dMass),-5./3);
    consts[1][2] = -3*sec->dK2*pri->dMass*pri->dMass*pow(sec->dRadius,3)/(8*sec->dRG*sec->dRG*BIGG*pow((pri->dMass+sec->dMass),2));
    consts[1][3] = 3*sec->dK2*pri->dMass*pow(sec->dRadius,5)/(4*sec->dMass)*pow(BIGG*(pri->dMass+sec->dMass),-5./3);
  }
}
 
void CalculateConstantsCTL2(PRIMARY *pri,SECONDARY *sec,double **consts) {

  consts[0][0] = 0;
  consts[0][1] = 0;
  consts[0][2] = 0;
  consts[0][3] = 0;
  consts[1][0] = 0;
  consts[2][1] = 0;
  consts[3][2] = 0;
  consts[4][3] = 0;
}

void CalculateConstantsCTL8(PRIMARY *pri,SECONDARY *sec,double **consts) {

  consts[0][0] = 0;
  consts[0][1] = 0;
  consts[0][2] = 0;
  consts[0][3] = 0;
  consts[1][0] = 0;
  consts[2][1] = 0;
  consts[3][2] = 0;
  consts[4][3] = 0;
}

void AssignConstants(PARAM *param,PRIMARY *pri,SECONDARY *sec,double **consts) {
  if (param->iTideModel == CPL2)
    CalculateConstantsCPL2(pri,sec,consts);
  if (param->iTideModel == CTL2)
    CalculateConstantsCTL2(pri,sec,consts);
  if (param->iTideModel == CTL8)
    CalculateConstantsCTL8(pri,sec,consts);
}    

/*
double dDaDt_CPL2(SECONDARY *sec) {
  sec->dSemi = dMeanMotionToSemi(sec->dMeanMotion,pri->dMass,sec->dMass); xXXX
  return -2*sec->dSemi/(3*sec->dMeanMotion) * sec->dDnDt;
}
*/

/*
double dDeDt_CPL2Free(PRIMARY *pri,SECONDARY *sec,int **epsilon,double *zprime) {
  double sum;
  int i;

  sum=0;
  for (i=0;i<2;i++)
    sum += consts[i][1]*(2*epsilon[i][0] - 24.5*epsilon[i][1] + 0.5*epsilon[i][2] + 3*epsilon[i][5]);

  return sec.dEcc*pow(sec.dMeanMotion,-xx) * sum;
}  

double dDeDt_CPL2Lock(PRIMARY *pri,SECONDARY *sec,int **epsilon,double *zprime) {
  double sum;
  int i;
}
*/

/*
double dDaDt1_CPL2(double dMass,double dMassPert,double dSemi,double dEcc, double dPsi,int *epsilon,double constsant) {


}

double dDeDt1_CPL2(double dMass,double dMassPert,double dSemi,double dEcc,int *epsilon,double zprime) {
  double foo;
 
    foo = zprime*(2*epsilon[0] - 49./2*epsilon[1] + 0.5*epsilon[2] + 3*epsilon[5]);

    return -dSemi*dEcc/(8*BIGG*dMass*dMassPert) * foo;
}  

*/

/*
double dDomegaDt_CPL2(double dMass,double dRadius,double dN,double dEcc,double dC,double dPsi,int *epsilon,double zprime) {
 
  return -zprime/(8*dMass*dC*dC*dRadius*dRadius*dN)*(4*epsilon[0] + dEcc*dEcc*(-20*epsilon[0] + 49*epsilon[1] + epsilon[2]) + 2*sin(dPsi)*sin(dPsi)*(-2*epsilon[0]+epsilon[8]+epsilon[9]));
}
*/

/*
double dDoblDt_CPL2(double dMass,double dRadius,double dOmega,double dN,double dC,int epsilon[9],double dChi,double dPsi,double zprime) {

  return zprime*sin(dPsi)/(4*dMass*dC*dC*dRadius*dRadius*dN*dOmega) * (epsilon[0]*(1-dChi) + (epsilon[8]-epsilon[9])*(1+dChi));
}
*/

double dTideHeat_CPL2(int *epsilon,double zprime,double dEcc,double dN,double dOmega,double dPsi) {

  /* Heller, Leconte & Barnes (2011), Eqn 9-11. */

  double dOrbE,dRotE;

  dOrbE = -zprime/8 * (4*epsilon[0] + dEcc*dEcc*(-20*epsilon[0] + 73.5*epsilon[1] + 0.5*epsilon[2] - 3*epsilon[5]) - 4*sin(dPsi)*sin(dPsi)*(epsilon[0]-epsilon[8]));

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

  epsilon = malloc(10*sizeof(int));

  /* Must reset spin rate in order to get the phase lags correctly */
  om = EqSpinRate_CPL2(dN,dEcc,dPsi,bDiscrete);

  AssignEpsilon(om,dN,epsilon);
  
  grot = dGammaRot(dEcc,dPsi,epsilon);
  gorb = dGammaOrb(dEcc,dPsi,epsilon);

  free(epsilon);

  /* return z/8 * ((om/dN)*grot - gorb); */
  return z/8 * ((1+9.5*dEcc*dEcc)*grot - gorb);
}

/*
void DerivsCPL2Lock(PARAM *param,PRIMARY *pri,SECONDARY *sec,IO *io,double **consts,double *f,double dBeta,int **epsilon,double dTime,int bDiscreteRot) {

  AssignEpsilon(pri->dSpinRate,sec->dMeanMotion,epsilon[0]);
  AssignEpsilon(sec->dSpinRate,sec->dMeanMotion,epsilon[1]);

  // Get the derivatives 
  sec->dDnDt = param->Derivs.PriMeanM(pri,sec,epsilon,consts) + param->Derivs.SecMeanM(pri,sec,epsilon,consts); //XXX Times primary variables out here?
  sec->dDeDt = param->Derivs.PriEcc(pri,sec,epsilon,consts) + param->Derivs.SecEcc(pri,sec,epsilon,consts);

  pri->dDobliquityDt = param->Derivs.PriObl(pri->dMass,pri->dRadius,pri->dSpinRate,sec->dMeanMotion,pri->dRG,epsilon[0],pri->dObliquity,consts[0]);
  sec->dDobliquityDt = param->Derivs.SecObl(sec->dMass,sec->dRadius,sec->dSpinRate,sec->dMeanMotion,sec->dRG,epsilon[1],sec->dObliquity,consts[1]);

}
*/

void DerivsCPL2(DERIVS *derivs,PRIMARY *pri,SECONDARY *sec,IO *io,double **consts,double *f,double dBeta,int **epsilon,double dTime,int bDiscreteRot) {

  AssignEpsilon(pri->dSpinRate,sec->dMeanMotion,epsilon[0]);
  AssignEpsilon(sec->dSpinRate,sec->dMeanMotion,epsilon[1]);
  pri->dSinObl = sin(pri->dObliquity);
  sec->dSinObl = sin(sec->dObliquity);
  
  /* Get the derivatives */
  sec->dDnDt = pow(sec->dMeanMotion,16./3)*(derivs->PriMeanM(pri,sec,epsilon[0],consts[0][0],0) + derivs->SecMeanM(pri,sec,epsilon[1],consts[1][0],1));
  sec->dDeDt = sec->dEcc*pow(sec->dMeanMotion,13./3)*(derivs->PriEcc(pri,sec,epsilon[0],consts[0][1],0) + derivs->SecEcc(pri,sec,epsilon[1],consts[1][1],1));

  pri->dDobliquityDt = pri->dSinObl*pow(sec->dMeanMotion,13./3)*derivs->PriObl(pri,sec,epsilon[0],consts[0][3],0);
  sec->dDobliquityDt = sec->dSinObl*pow(sec->dMeanMotion,13./3)*derivs->SecObl(pri,sec,epsilon[1],consts[1][3],1);

  if (!pri->bForceEqSpin)
    pri->dDomegaDt = pow(sec->dMeanMotion,4)*dDomegaDtCPL2Free(pri,sec,epsilon[0],consts[0][2],0);
  if (!sec->bForceEqSpin)
    sec->dDomegaDt = pow(sec->dMeanMotion,4)*dDomegaDtCPL2Free(pri,sec,epsilon[1],consts[1][2],1);
}

/*
void DerivsCPL2Lock(PRIMARY *pri,SECONDARY *sec,IO *io,double **consts,,double *f,double dBeta,int **epsilon,double dTime,int bDiscreteRot) {

  AssignEpsilon(pri->dSpinRate,sec->dMeanMotion,epsilon[0]);
  AssignEpsilon(sec->dSpinRate,sec->dMeanMotion,epsilon[1]);

  // Get the derivatives 
  sec->dDnDt = dDnDt_CPL2Free(pri,sec,epsilon,consts);
  sec->dDeDt = dDeDt_CPL2Free(pri,sec,epsilon,consts);

  pri->dDobliquityDt = dDoblDt_CPL2Free(pri->dMass,pri->dRadius,pri->dSpinRate,sec->dMeanMotion,pri->dRG,epsilon[0],pri->dObliquity,consts[0]);
  sec->dDobliquityDt = dDoblDt_CPL2Free(sec->dMass,sec->dRadius,sec->dSpinRate,sec->dMeanMotion,sec->dRG,epsilon[1],sec->dObliquity,consts[1]);

  // None of this should be called
  //pri->dDomegaDt = TINY;
  
}
  
void DerivsCPL2Free(PRIMARY *pri,SECONDARY *sec,IO *io,double **consts,,double *f,double dBeta,int **epsilon,double dTime,int bDiscreteRot) {

  // Get auxiliary parameters for derivative calculations
  //sec->dMeanMotion = dSemiToMeanMotion(sec->dSemi,(pri->dMass+sec->dMass));
  AssignEpsilon(pri->dSpinRate,sec->dMeanMotion,epsilon[0]);
  AssignEpsilon(sec->dSpinRate,sec->dMeanMotion,epsilon[1]);
  //AssignChi(pri,sec,chi);
  //AssignZprime(pri,sec,zprime);

  // Get the derivatives 
  sec->dDnDt = dDaDt_CPL2Free(pri,sec,epsilon,consts);
  sec->dDeDt = dDeDt_CPL2Free(pri,sec,epsilon,consts);

  pri->dDobliquityDt = dDoblDt_CPL2Free(pri->dMass,pri->dRadius,pri->dSpinRate,sec->dMeanMotion,pri->dRG,epsilon[0],pri->dObliquity,consts[0]);
  sec->dDobliquityDt = dDoblDt_CPL2Free(sec->dMass,sec->dRadius,sec->dSpinRate,sec->dMeanMotion,sec->dRG,epsilon[1],sec->dObliquity,consts[1]);

  pri->dDomegaDt = dDomegaDt_CPL2Free(pri->dMass,pri->dRadius,sec->dMeanMotion,sec->dEcc,pri->dRG,pri->dObliquity,epsilon[0],consts[0]);
  sec->dDomegaDt = dDomegaDt_CPL2Free(sec->dMass,sec->dRadius,sec->dMeanMotion,sec->dEcc,sec->dRG,sec->dObliquity,epsilon[1],consts[1]);
}
*/

void AssignDerivsCPL2FreePri(DERIVS *derivs) {
  derivs->PriMeanM = &dDnDtCPL2Free;
  derivs->PriEcc = &dDeDtCPL2Free;
  derivs->PriObl = &dDoblDtCPL2Free;
}

void AssignDerivsCPL2LockPri(DERIVS *derivs) {
  derivs->PriMeanM = &dDnDtCPL2Lock;
  derivs->PriEcc = &dDeDtCPL2Lock;
  derivs->PriObl = &dDoblDtCPL2Lock;
}

void AssignDerivsCPL2LockSec(DERIVS *derivs) {
  derivs->SecMeanM = &dDnDtCPL2Lock;
  derivs->SecEcc = &dDeDtCPL2Lock;
  derivs->SecObl = &dDoblDtCPL2Lock;
}

void AssignDerivsCPL2FreeSec(DERIVS *derivs) {
  derivs->SecMeanM = &dDnDtCPL2Free;
  derivs->SecEcc = &dDeDtCPL2Free;
  derivs->SecObl = &dDoblDtCPL2Free;
}
  
/* CCCCCC  TTTTTTT  L
 * C          T     L
 * C          T     L
 * C          T     L
 * C          T     L
 * CCCCCC     T     LLLLLL
 */


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

double EqSpinRate_CTL8(double n,double e,double psi,int foo) {
    double dBeta,f2,f5;

    dBeta=AssignBeta(e);
    f2=AssignF2(e);
    f5=AssignF5(e);

    return f2/(pow(dBeta,3)*f5) * 2*cos(psi)/(1+cos(psi)*cos(psi)) * n;
}

/*
void AssignZ(PRIMARY *pri,SECONDARY *sec,double *z) {
  z[0] = 3*BIGG*BIGG*pri->dK2*sec->dMass*sec->dMass*(pri->dMass+sec->dMass)*pow(pri->dRadius,5)*pri->dTau/pow(sec->dSemi,9);
  z[1] = 3*BIGG*BIGG*sec->dK2*pri->dMass*pri->dMass*(pri->dMass+sec->dMass)*pow(sec->dRadius,5)*sec->dTau/pow(sec->dSemi,9);
}
*/

double dDaDt_CTL8(PRIMARY *pri,SECONDARY *sec,double *z,double beta,double *f) {
  int i;
  double psi[2],omega[2],sum;

  psi[0]=pri->dObliquity;
  psi[1]=sec->dObliquity;
  omega[0]=pri->dSpinRate;
  omega[1]=sec->dSpinRate;

  sum=0;
  for (i=0;i<2;i++) 
      sum += z[i]*(cos(psi[i])*f[1]*omega[i]/(pow(beta,12)*sec->dMeanMotion) - f[0]/pow(beta,15));
  return 0;//XXX
  //  return 2*sec->dSemi*sec->dSemi/(BIGG*pri->dMass*sec->dMass)*sum;
}

double dDeDt_CTL8(PRIMARY *pri,SECONDARY *sec,double *z,double beta,double *f) {
  int i;
  double *psi,*omega,sum;

  psi = malloc(2*sizeof(double));
  omega = malloc(2*sizeof(double));
  
  psi[0]=pri->dObliquity;
  psi[1]=sec->dObliquity;
  omega[0]=pri->dSpinRate;
  omega[1]=sec->dSpinRate;

  sum=0;
  for (i=0;i<2;i++) 
    sum += z[i]*(cos(psi[i])*f[3]*omega[i]/(pow(beta,10)*sec->dMeanMotion) - 18*f[2]/(11*pow(beta,13)));

  free(psi);
  free(omega);
  return 0;//XXX
  // return 11*sec->dSemi*sec->dEcc/(2*BIGG*pri->dMass*sec->dMass)*sum;
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

void DerivsCTL8(DERIVS *derivs,PRIMARY *pri,SECONDARY *sec,IO *io,double **constns,double *f,double dBeta,int **epsilon,double dTime,int bDiscreteRot) {

  double *z,*chi;
  
  //XXX sec->dMeanMotion = dSemiToMeanMotion(sec->dSemi,(pri->dMass+sec->dMass));
  //AssignChi(pri,sec,chi);
  //AssignZ(pri,sec,z);
  dBeta = AssignBeta(sec->dEcc);
  f[0] = AssignF1(sec->dEcc);
  f[1] = AssignF2(sec->dEcc);
  f[2] = AssignF3(sec->dEcc);
  f[3] = AssignF4(sec->dEcc);
  f[4] = AssignF5(sec->dEcc);
  
  /* Calculate tidal properties */
  // XXX!
  sec->dDnDt = dDaDt_CTL8(pri,sec,z,dBeta,f);
  sec->dDeDt = dDeDt_CTL8(pri,sec,z,dBeta,f);
  
  /* Obliquities */
  
  pri->dDobliquityDt = dDoblDt_CTL8(pri->dMass,pri->dRadius,sec->dMeanMotion,sec->dEcc,pri->dRG,pri->dSpinRate,pri->dObliquity,z[0],dBeta,f,chi[0]);
  sec->dDobliquityDt = dDoblDt_CTL8(sec->dMass,sec->dRadius,sec->dMeanMotion,sec->dEcc,sec->dRG,sec->dSpinRate,sec->dObliquity,z[1],dBeta,f,chi[1]);

  if (pri->bForceEqSpin)
    pri->dDomegaDt = TINY;
  else
    pri->dDomegaDt = dDomegaDt_CTL8(pri->dMass,pri->dRadius,sec->dMeanMotion,sec->dEcc,pri->dRG,pri->dSpinRate,pri->dObliquity,z[0],dBeta,f);

  if (sec->bForceEqSpin)
    sec->dDomegaDt = TINY;
  else
    sec->dDomegaDt = dDomegaDt_CTL8(sec->dMass,sec->dRadius,sec->dMeanMotion,sec->dEcc,sec->dRG,sec->dSpinRate,sec->dObliquity,z[1],dBeta,f);
}

/*
 *
 *     INTEGRATION FUNCTIONS
 *
 *
 */

double AssignDt(PRIMARY *pri,SECONDARY *sec,double dNextOutput,double dTimestepCoeff) {
  double min;

  if (sec->dDnDt != 0)
    min=fabs(sec->dMeanMotion/sec->dDnDt);
  if (sec->dDeDt != 0 && sec->dEcc != 0) {
    if (fabs(sec->dEcc/sec->dDeDt) < min) 
      min=fabs(sec->dEcc/sec->dDeDt);
  }
  if (pri->dDobliquityDt != 0 && pri->dObliquity != 0) {
    if (fabs(pri->dObliquity/pri->dDobliquityDt) < min)
      min=fabsl(pri->dObliquity/pri->dDobliquityDt);
  }
  if (sec->dDobliquityDt != 0 && sec->dObliquity != 0) {
    if (fabs(sec->dObliquity/sec->dDobliquityDt) < min)
      min=fabs(sec->dObliquity/sec->dDobliquityDt);
  }
  if (pri->dDomegaDt != 0) {
    if (fabs(pri->dSpinRate/pri->dDomegaDt) < min)
      min=fabs(pri->dSpinRate/pri->dDomegaDt);
  }
  if (sec->dDomegaDt != 0) {
    if (fabs(sec->dSpinRate/sec->dDomegaDt) < min)
      min=fabs(sec->dSpinRate/sec->dDomegaDt);
  }
  
  min = dTimestepCoeff * min;

  if (dNextOutput < min)
    min = dNextOutput;

  return min;
}

double NextOutput(double dTime,double dOutputInterval) {
  int nSteps;
  
  /* Number of output so far */
  nSteps = (int)(dTime/dOutputInterval);
  /* Next output is one more */
  return (nSteps+1)*dOutputInterval;
}

void CheckTideLock(PARAM *param,PRIMARY *pri,SECONDARY *sec,IO *io,double **consts,double dTime) {
  /* Maintain spins at equilibrium rate? */
  /* dDomegaDt is set to TINY to not throw variable timestepping */
  
  if (pri->bForceEqSpin) {
    pri->dSpinRate = param->fEqSpin(sec->dMeanMotion,sec->dEcc,pri->dObliquity,param->bDiscreteRot);
    if (!sec->bSynchronous && sec->dEcc < sqrt(1./19)) {
      /* The primary was synchronously rotating at 3:2, but now eccentricity
	 has damped enough for it to become a synchronous rotator. The 
	 bSynchronous flag is now flipped to 1, and the derivatives
	 are changed. */
      pri->bSynchronous = 1;
      if (param->iTideModel == CPL2) {
	CalculateConstantsCPL2(pri,sec,consts);
	AssignDerivsCPL2LockPri(&param->Derivs);
      } else if (param->iTideModel == CTL2) {
	//CalculateConstantsCTL2(pri,sec,consts);
	//AssignDerivsCTL2LockPri(&param->Derivs);
      } else if (param->iTideModel == CTL8) {
	//AssignDerivsCTL8LockPri(&param->Derivs);
      }
    }
  }
  else {
    if (fabs((pri->dSpinRate - param->fEqSpin(sec->dMeanMotion,sec->dEcc,pri->dObliquity,param->bDiscreteRot))/param->fEqSpin(sec->dMeanMotion,sec->dEcc,pri->dObliquity,param->bDiscreteRot)) < pri->dMaxLockDiff) { /* yes */
      pri->bForceEqSpin = 1; /* This forces it to be locked from now on */
      pri->dDomegaDt = TINY;
      if (sec->dEcc < sqrt(1./19)) {
	/* The eccentricity is low enough that the primary will now rotate
	   synchronously. The bSynchronous flag is now flipped to 1, and 
	   the derivatives are changed. */
	pri->bSynchronous = 1;
	if (param->iTideModel == CPL2) {
	  CalculateConstantsCPL2(pri,sec,consts);
	  AssignDerivsCPL2LockPri(&param->Derivs);
	} else if (param->iTideModel == CTL2) {
	  //CalculateConstantsCTL2(param->consts);
	  //AssignDerivsCTL2LockPri(&param->Derivs);
	  //params->dDerivs = &DerivsCTL2Lock;
	} else if (param->iTideModel == CTL8) {
	  //CalculateConstantsCTL8(param->consts);
	  //params->dDerivs = &DerivsCTL8Lock;
	  //AssignDerivsCTL8LockPri(&param->Derivs);
	}
      }
      if (io->iVerbose >= VERBPROG) {
	printf("Primary spin locked at ");
	fprintd(stdout,dTime/YEARSEC,io->iSciNot,io->iDigits);
	printf(" years.\n");
      }
    }
  }
  
  if (sec->bForceEqSpin) {
    sec->dSpinRate = param->fEqSpin(sec->dMeanMotion,sec->dEcc,sec->dObliquity,param->bDiscreteRot);
    if (!sec->bSynchronous && sec->dEcc < sqrt(1./19)) {
      /* The secondary was synchronously rotating at 3:2, but now eccentricity
	 has damped enough for it to become a synchronous rotator. The 
	 bSynchronous flag is now flipped to 1, and the derivatives
	 are changed. */
      sec->bSynchronous = 1;
      if (param->iTideModel == CPL2) {
	CalculateConstantsCPL2(pri,sec,consts);
	AssignDerivsCPL2LockSec(&param->Derivs);
      } else if (param->iTideModel == CTL2) {
	//AssignDerivsCTL2LockSec(&param->Derivs);
      } else if (param->iTideModel == CTL8) {
	//AssignDerivsCTL8LockSec(&param->Derivs);
      }
    }
  }
  else {
    if (fabs((sec->dSpinRate - param->fEqSpin(sec->dMeanMotion,sec->dEcc,sec->dObliquity,param->bDiscreteRot))/param->fEqSpin(sec->dMeanMotion,sec->dEcc,sec->dObliquity,param->bDiscreteRot)) < sec->dMaxLockDiff) { /* yes */
      sec->bForceEqSpin = 1; /* This forces it to be locked from now on */
      sec->dDomegaDt = TINY;
      if (sec->dEcc < sqrt(1./19)) {
	/* The eccentricity is low enough that the secondary will now rotate
	   synchronously. The bSynchronous flag is now flipped to 1, and 
	   the derivatives are changed. */
	sec->bSynchronous = 1;
	if (param->iTideModel == CPL2) {
	  CalculateConstantsCPL2(pri,sec,consts);
	  AssignDerivsCPL2LockSec(&param->Derivs);
	} else if (param->iTideModel == CTL2) {
	  // AssignDerivsCTL2LockSec(&param->Derivs);
	} else if (param->iTideModel == CTL8) {
	  //AssignDerivsCTL8LockSec(&param->Derivs);
	}
      }
      if (io->iVerbose >= VERBPROG) {
	printf("Secondary spin locked at ");
	fprintd(stdout,dTime/YEARSEC,io->iSciNot,io->iDigits);
	printf(" years.\n");
      }
    }
  }
}

int bCheckHaltSemi(PARAM *param,PRIMARY *pri,SECONDARY *sec,IO *io,double dTime) {
  /* The semi-major axis does not need to be calculated during the integration,
     but it is calculated here to check for halts. Note that bMerge=1 by 
     default. If calculations will not result in a merge, set bMerge=0 to 
     maximize performance. */

  double dSemi;

  // Calculate Semi-major axis 
  dSemi = dMeanMotionToSemi(sec->dMeanMotion,(pri->dMass+sec->dMass));
  
  /* Minimum Semi-major Axis? */
  if (dSemi <= param->halt.dMinSemi) {
    if (io->iVerbose >= VERBPROG) {
      printf("HALT: a = ");
      fprintd(stdout,dSemi,io->iSciNot,io->iDigits);
      printf(", < min a = ");
      fprintd(stdout,param->halt.dMinSemi,io->iSciNot,io->iDigits);
      printf(" at %.2e years\n",dTime/YEARSEC);
    }
    return 1;
  }

  /* Merge? */

  if (dSemi*(1-sec->dEcc) <= (pri->dRadius + sec->dRadius) && param->halt.bMerge) { /* Merge! */
    if (io->iVerbose > VERBPROG) 
      printf("HALT: Merge at %.2e years!\n",dTime/YEARSEC);

    return 1;
  }

  return 0;
}


int bCheckHalt(PARAM *param,PRIMARY *pri,SECONDARY *sec,IO *io,double dTime) {

  /* Double Synchronous? */

    if (pri->bForceEqSpin && (pri->dObliquity <= param->dMinValue) && sec->bForceEqSpin && (sec->dObliquity <= param->dMinValue) && param->halt.bDblSync) {
    if (io->iVerbose >= VERBPROG) {
      printf("HALT: Double Synchronous at ");
      fprintd(stdout,dTime/YEARSEC,io->iSciNot,io->iDigits);
      printf(" years.\n");
    }
    return 1;
  }

  /* Maximum Eccentricity? */

  if (sec->dEcc > param->halt.dMaxEcc) {
    if (io->iVerbose >= VERBPROG) {
      printf("HALT: e = ");
      fprintd(stdout,sec->dEcc,io->iSciNot,io->iDigits);
      printf(", > max e = ");
      fprintd(stdout,param->halt.dMaxEcc,io->iSciNot,io->iDigits);
      printf(" at %.2e years\n",dTime/YEARSEC);
    }
    return 1;
  }

  /* Minimum Eccentricity? */

  if (sec->dEcc <= param->halt.dMinEcc) {
    if (io->iVerbose >= VERBPROG) {
      printf("HALT: e = ");
      fprintd(stdout,sec->dEcc,io->iSciNot,io->iDigits);
      printf(", < min e = ");
      fprintd(stdout,param->halt.dMinEcc,io->iSciNot,io->iDigits);
      printf(" at %.2e years\n",dTime/YEARSEC);
    }
    return 1;
  }

  /* Positive de/dt? */

  if (sec->dDeDt > 0 && param->halt.bPosDeDt) {
    if (io->iVerbose >= VERBPROG) {
      printf("HALT: de/dt = ");
      fprintd(stdout,sec->dDeDt,io->iSciNot,io->iDigits);
      printf(" at %.2e years\n",dTime/YEARSEC);
    }
    return 1;
  }

  /* Primary reached minimum obliquity? */

  if (pri->dObliquity < param->halt.dMinPriObl) {
    if (io->iVerbose >= VERBPROG) {
      printf("HALT: Primary Obliquity = ");
      fprintd(stdout,pri->dObliquity,io->iSciNot,io->iDigits);
      printf(", < minimum obliquity = ");
      fprintd(stdout,param->halt.dMinPriObl,io->iSciNot,io->iDigits);
      printf(" at %.2e years.\n",dTime/YEARSEC);
    }
    return 1;
  }

  /* Secondary reached minimum obliquity? */

  if (sec->dObliquity < param->halt.dMinSecObl) {
    if (io->iVerbose >= VERBPROG) {
      printf("HALT: Secondary Obliquity = ");
      fprintd(stdout,sec->dObliquity,io->iSciNot,io->iDigits);
      printf(", < minimum obliquity = ");
      fprintd(stdout,param->halt.dMinSecObl,io->iSciNot,io->iDigits);
      printf(" at %.2e years.\n",dTime/YEARSEC);
    }
    return 1;
  }

  /* Primary Tide-locked? */

  if (pri->bForceEqSpin && param->halt.bPriLock) {
    if (io->iVerbose >= VERBPROG) {
      printf("HALT: Primary tide-locked at ");
      fprintd(stdout,dTime/YEARSEC,io->iSciNot,io->iDigits);
      printf(" years.\n");
    }
    return 1;
  }

  /* Secondary Tide-locked? */

  if (sec->bForceEqSpin && param->halt.bSecLock) {
    if (io->iVerbose >= VERBPROG) {
      printf("HALT: Secondary tide-locked at ");
      fprintd(stdout,dTime/YEARSEC,io->iSciNot,io->iDigits);
      printf(" years.\n");
    }
    return 1;
  }
  
  if (param->halt.bSecSync && sec->dEcc <= sec->dSyncEcc && sec->bForceEqSpin) {
    if (io->iVerbose >= VERBPROG) {
      printf("HALT: Secondary's rotation is synchronous (e = ");
      fprintd(stdout,sec->dEcc,io->iSciNot,io->iDigits);
      printf(") at ");
      fprintd(stdout,dTime/YEARSEC,io->iSciNot,io->iDigits);
      printf(" years.\n");
    }
    return 1;
  }

  // These are separate as require semimajor axis calculation
  if (param->halt.bMerge || param->halt.dMinSemi >0) {
    if (bCheckHaltSemi(param,pri,sec,io,dTime))
	return 1;
  }
  
  return 0;
}

/* If small enough, set some quantities to zero */
void SetMinValue(PARAM *param,PRIMARY *pri,SECONDARY *sec) {
  if (param->dMinValue > 0) {
    if (sec->dEcc < param->dMinValue)
      sec->dEcc = 0;
    if (sec->dObliquity < param->dMinValue)
      sec->dObliquity = 0;
    if (pri->dObliquity < param->dMinValue)
      pri->dObliquity = 0;
  }
}

void PrimaryCopy(PRIMARY *tmpPri,PRIMARY pri) {
  tmpPri->dMass = pri.dMass;
  tmpPri->dRadius = pri.dRadius;
  tmpPri->dTau = pri.dTau;
  tmpPri->dQ = pri.dQ;
  tmpPri->dK2 = pri.dK2;
  tmpPri->dObliquity = pri.dObliquity;
  tmpPri->dSpinRate = pri.dSpinRate;
  tmpPri->dRG = pri.dRG;
  tmpPri->bForceEqSpin = pri.bForceEqSpin;
  tmpPri->dMaxLockDiff = pri.dMaxLockDiff;

  tmpPri->dDomegaDt = pri.dDomegaDt;
  tmpPri->dDobliquityDt = pri.dDobliquityDt;
}

void SecondaryCopy(SECONDARY *tmpSec,SECONDARY sec) {
  tmpSec->dMass = sec.dMass;
  tmpSec->dRadius = sec.dRadius;
  tmpSec->dTau = sec.dTau;
  tmpSec->dQ = sec.dQ;
  tmpSec->dK2 = sec.dK2;
  tmpSec->dObliquity = sec.dObliquity;
  tmpSec->dSpinRate = sec.dSpinRate;
  tmpSec->dRG = sec.dRG;
  tmpSec->bForceEqSpin = sec.bForceEqSpin;
  tmpSec->dMaxLockDiff = sec.dMaxLockDiff;

  tmpSec->dDomegaDt = sec.dDomegaDt;
  tmpSec->dDobliquityDt = sec.dDobliquityDt;

  tmpSec->dEcc = sec.dEcc;
  tmpSec->dMeanMotion = sec.dMeanMotion;
  tmpSec->dDnDt = sec.dDnDt;
  tmpSec->dDeDt = sec.dDeDt;
}

void UpdateRotation(PARAM *param,PRIMARY *pri,SECONDARY *sec,double dDt,int iDir) {
  if (pri->bForceEqSpin)
    pri->dSpinRate = param->fEqSpin(sec->dMeanMotion,sec->dEcc,pri->dObliquity,param->bDiscreteRot);
  else
    pri->dSpinRate = pri->dSpinRate + dDt*iDir*pri->dDomegaDt;

  if (sec->bForceEqSpin)
    sec->dSpinRate = param->fEqSpin(sec->dMeanMotion,sec->dEcc,sec->dObliquity,param->bDiscreteRot);
  else
    sec->dSpinRate = sec->dSpinRate + dDt*iDir*sec->dDomegaDt;
}

void Update(PARAM *param,PRIMARY *tmpPri,SECONDARY *tmpSec,PRIMARY pri,SECONDARY sec,double dDt,int iDir) {

  /* The tmp parameters come initialized as the originals at the beginning
     of the timestep. */
  tmpSec->dEcc = tmpSec->dEcc + dDt*iDir*sec.dDeDt;
  tmpSec->dMeanMotion = tmpSec->dMeanMotion + dDt*iDir*sec.dDnDt;
  tmpPri->dObliquity = tmpPri->dObliquity + dDt*pri.dDobliquityDt;
  tmpSec->dSpinRate = tmpSec->dSpinRate + dDt*iDir*sec.dDomegaDt;
  tmpSec->dObliquity = tmpSec->dObliquity + dDt*iDir*sec.dDobliquityDt;

  UpdateRotation(param,tmpPri,tmpSec,dDt,iDir);
}
 
void FinishStep(PARAM *param,PRIMARY *pri,SECONDARY *sec,double *dTime,double dDt,int iDir) {
  /* Update parameters */
  sec->dMeanMotion += iDir*sec->dDnDt*dDt;
  sec->dEcc += iDir*sec->dDeDt*dDt;
  pri->dObliquity += iDir*pri->dDobliquityDt*dDt;
  sec->dObliquity += iDir*sec->dDobliquityDt*dDt;

  UpdateRotation(param,pri,sec,dDt,iDir);

  *dTime += dDt;
  sec->dAge += iDir*dDt;
}

double RK4Step(PARAM *param,PRIMARY *pri,SECONDARY *sec,IO *io,double **consts,double *f,double dBeta,int **epsilon,double *dTime,double dTimeOut,int iDir) {
  int i;
  double dDt;
  PRIMARY tmpPri[4];
  SECONDARY tmpSec[4];

  for (i=0;i<4;i++) {
    PrimaryCopy(&tmpPri[i],*pri);
    SecondaryCopy(&tmpSec[i],*sec);
  }

  // Derivatives at start
  param->fDerivs(&param->Derivs,&tmpPri[0],&tmpSec[0],io,consts,f,dBeta,epsilon,*dTime,param->bDiscreteRot);
  
  /* Adjust dt? */
  if (param->bVarDt) 
    dDt = AssignDt(&tmpPri[0],&tmpSec[0],(dTimeOut-*dTime),param->dTimestepCoeff);
  
  Update(param,&tmpPri[1],&tmpSec[1],tmpPri[0],tmpSec[0],0.5*dDt,iDir);

  // Derivatives at first midpoint
  param->fDerivs(&param->Derivs,&tmpPri[1],&tmpSec[1],io,consts,f,dBeta,epsilon,*dTime,param->bDiscreteRot);
  
  Update(param,&tmpPri[2],&tmpSec[2],tmpPri[1],tmpSec[1],0.5*dDt,iDir);

  // Derivatives at second midpoint
  param->fDerivs(&param->Derivs,&tmpPri[2],&tmpSec[2],io,consts,f,dBeta,epsilon,*dTime,param->bDiscreteRot);
  
  Update(param,&tmpPri[3],&tmpSec[3],tmpPri[2],tmpSec[2],dDt,iDir);

  // Derivatives at end
  param->fDerivs(&param->Derivs,&tmpPri[3],&tmpSec[3],io,consts,f,dBeta,epsilon,*dTime,param->bDiscreteRot);
  
  pri->dDomegaDt = 1./6*(tmpPri[0].dDomegaDt + 2*tmpPri[1].dDomegaDt + 2*tmpPri[2].dDomegaDt + tmpPri[3].dDomegaDt);
  pri->dDobliquityDt = 1./6*(tmpPri[0].dDobliquityDt + 2*tmpPri[1].dDobliquityDt + 2*tmpPri[2].dDobliquityDt + tmpPri[3].dDobliquityDt);

  sec->dDomegaDt = 1./6*(tmpSec[0].dDomegaDt + 2*tmpSec[1].dDomegaDt + 2*tmpSec[2].dDomegaDt + tmpSec[3].dDomegaDt);
  sec->dDobliquityDt = 1./6*(tmpSec[0].dDobliquityDt + 2*tmpSec[1].dDobliquityDt + 2*tmpSec[2].dDobliquityDt + tmpSec[3].dDobliquityDt);

  sec->dDnDt = 1./6*(tmpSec[0].dDnDt + 2*tmpSec[1].dDnDt + 2*tmpSec[2].dDnDt + tmpSec[3].dDnDt);
  sec->dDeDt = 1./6*(tmpSec[0].dDeDt + 2*tmpSec[1].dDeDt + 2*tmpSec[2].dDeDt + tmpSec[3].dDeDt);

  FinishStep(param,pri,sec,dTime,dDt,iDir);

  return dDt;
}

double EulerStep(PARAM *param,PRIMARY *pri,SECONDARY *sec,IO *io,double **consts,double *f,double dBeta,int **epsilon,double *dTime,double dTimeOut,int iDir) {
  double dDt;
  
  param->fDerivs(&param->Derivs,pri,sec,io,consts,f,dBeta,epsilon,*dTime,param->bDiscreteRot);

  /* Adjust dt? */
  if (param->bVarDt) 
    dDt = AssignDt(pri,sec,(dTimeOut-*dTime),param->dTimestepCoeff);
  
  FinishStep(param,pri,sec,dTime,dDt,iDir);

  return dDt;
}

void AssignDerivs(PARAM *param,PRIMARY *pri,SECONDARY *sec) {
  if (param->iTideModel == CPL2) {
    if (pri->bSynchronous) 
      AssignDerivsCPL2LockPri(&param->Derivs);
    else
      AssignDerivsCPL2FreePri(&param->Derivs);
    
    if (sec->bSynchronous)
      AssignDerivsCPL2LockSec(&param->Derivs);
    else
      AssignDerivsCPL2FreeSec(&param->Derivs);
  } else if (param->iTideModel == CTL2) {
  } else if (param->iTideModel == CTL8) {
  }
}


void Backward(PARAM *param,PRIMARY *pri,SECONDARY *sec,OUTPUT *output,FILES *files,IO *io,fdStep fdOneStep) {
  int **epsilon,nsteps=0;
  double *z,*chi;
  double dTime,dTimeOut;
  double dDt;
  FILE *fp;
  double *f,dBeta;
  double **consts;

  // 2nd Order Theories use a 2x4 matrix of coefficients
  consts = malloc(2*sizeof(double*));
  consts[0] = malloc(4*sizeof(double));
  consts[1] = malloc(4*sizeof(double));

  /* CPL Auxiliary variables */
  epsilon=malloc(2*sizeof(int*));
  epsilon[0]=malloc(10*sizeof(int));
  epsilon[1]=malloc(10*sizeof(int));

  /* CTL Auxiliary variables */
  f = malloc(5*sizeof(double));

  /* Common Auxiliary variables */
  z=malloc(2*sizeof(double));
  chi=malloc(2*sizeof(double));

  if (param->iNumCols > 0)
    fp=fopen(files->cBack,"w");
  dTime = 0;
  dDt = param->dBackTimeStep;
  dTimeOut = param->dBackOutputTime;

  /* Calculate initial derivatives */
  param->fDerivs(&param->Derivs,pri,sec,io,consts,f,dBeta,epsilon,dTime,dTimeOut);

  /* Adjust dt? */
  if (param->bVarDt) 
    dDt = AssignDt(pri,sec,(dTimeOut - dTime),param->dTimestepCoeff);

  /* Write out initial conditions */

  Output(param,pri,sec,output,io,-dTime,dDt,fp);
  dTimeOut = NextOutput(dTime,param->dBackOutputTime);

  /* Main loop begins here */

  while (dTime < param->dBackStopTime) {

    /* Calculate derivatives and do the updates */
    dDt = fdOneStep(param,pri,sec,io,consts,f,dBeta,epsilon,&dTime,dTimeOut,-1);

    nsteps++;

    /* If small enough, set some quantities to zero */
    SetMinValue(param,pri,sec);

    /* Halt? */
    if (param->halt.bHalt) {
      if (bCheckHalt(param,pri,sec,io,-dTime)) {
	Output(param,pri,sec,output,io,-dTime,dDt,fp);
	free(f);
	free(epsilon[1]);
	free(epsilon[0]);
	free(epsilon);
	free(chi);
	free(z);
	return;
      }
    }

    /* Time for Output? */
    if (dTime >= dTimeOut) {
      Output(param,pri,sec,output,io,-dTime,param->dBackOutputTime/nsteps,fp);
      dTimeOut = NextOutput(dTime,param->dBackOutputTime);
    }
  }

  free(f);
  free(epsilon[1]);
  free(epsilon[0]);
  free(epsilon);
  free(chi);
  free(z);
}

void Forward(PARAM *param,PRIMARY *pri,SECONDARY *sec,OUTPUT *output,FILES *files,IO *io,fdStep fdOneStep) {
  int **epsilon,nsteps=0,i;
  double *z,*chi;
  double dTime,dTimeOut;
  double dDt;
  double dEqSpinRate;
  FILE *fp;
  double *f,dBeta;
  double **consts;

  // 2nd Order Theories use a 2x4 matrix of coefficients
  consts = malloc(2*sizeof(double*));
  consts[0] = malloc(4*sizeof(double));
  consts[1] = malloc(4*sizeof(double));
  
  // Should a body already be tidally locked?
  CheckTideLock(param,pri,sec,io,consts,dTime);

  /* Check to see if a tidally-locked body requires a halt. */
  if (param->halt.bHalt) {
    if (bCheckHalt(param,pri,sec,io,dTime)) {
      Output(param,pri,sec,output,io,dTime,dDt,fp);
      free(consts);
      return;
    }
  }

  /* Now that we know tidal locking stati, and that an integration will
     occur, initialize arrays and files */
  
  // CPL Auxiliary variables
  epsilon=malloc(2*sizeof(double*));
  epsilon[0]=malloc(10*sizeof(double));
  epsilon[1]=malloc(10*sizeof(double));

  // CTL Auxiliary variables
  f=malloc(5*sizeof(double));

  /* Common Auxiliary variables */
  //z=malloc(2*sizeof(double));
  //chi=malloc(2*sizeof(double));

  if (param->iNumCols > 0)
    fp=fopen(files->cFor,"w");
  dTime = 0;
  dTimeOut = param->dForwOutputTime;
  dDt = param->dForwTimeStep;

  // Initialize funciton pointer derivatives and integration constants
  AssignDerivs(param,pri,sec);
  AssignConstants(param,pri,sec,consts);
  
  /* Calculate initial derivatives */
  param->fDerivs(&param->Derivs,pri,sec,io,consts,f,dBeta,epsilon,dTime,dTimeOut);

  /* Adjust dt? */
  if (param->bVarDt) 
    dDt = AssignDt(pri,sec,(dTimeOut - dTime),param->dTimestepCoeff);

  /* Write out initial conditions */
  Output(param,pri,sec,output,io,dTime,dDt,fp);

  // Calculate next (first) output time
  dTimeOut = NextOutput(dTime,param->dForwOutputTime);

  /* Main loop begins here */

  while (dTime < param->dForwStopTime) {
    /* Calculate derivatives and do the updates */
    dDt = fdOneStep(param,pri,sec,io,consts,f,dBeta,epsilon,&dTime,dTimeOut,1);

    // Is a body tidally locked?
    CheckTideLock(param,pri,sec,io,consts,dTime);

    nsteps++;

    /* Halt? */
    if (param->halt.bHalt) {
      if (bCheckHalt(param,pri,sec,io,dTime)) {
	Output(param,pri,sec,output,io,dTime,dDt,fp);
	free(epsilon[1]);
	free(epsilon[0]);
	free(epsilon);
	//free(chi);
	//free(z);
	return;
      }
    }

    /* If small enough, set some quantities to zero */
    SetMinValue(param,pri,sec);

    /* Time for Output? */
    if (dTime >= dTimeOut) {
      Output(param,pri,sec,output,io,dTime,param->dForwOutputTime/nsteps,fp);
      dTimeOut = NextOutput(dTime,param->dForwOutputTime);
      nsteps=0;
    }
  }

  free(epsilon[1]);
  free(epsilon[0]);
  free(epsilon);
  //free(chi);
  //free(z);
}

int main(int argc,char *argv[]) {
  int i,iv,iq;
  double f;
  PARAM param;
  SECONDARY sec;
  PRIMARY pri;
  IO io;
  FILES files; 
  OUTPUT output;
  OPTIONS options;
  fdStep fdOneStep;

  InitializeIO(&io);
  InitializeOutput(&output);
  InitializeOptions(&options);

  files.cExe=argv[0];

  if (argc == 1) {
    fprintf(stderr,"ERROR: Incorrect number of arguments. Usage: %s [-verbose] [-help] file [file].\n",argv[0]);
    exit(io.exit_exe);
  }

  iv=-1;
  iq=-1;
  io.iVerbose = -1;
  /* Check for flags */
  for (i=1;i<argc;i++) {
    if (memcmp(argv[i],"-v",2) == 0) {
      io.iVerbose = VERBALL;
      iv=i;
    }
    if (memcmp(argv[i],"-q",2) == 0) {
      io.iVerbose = 0;
      iq=i;
    }
    if (memcmp(argv[i],"-h",2) == 0) {
      WriteHelp(output,options,files.cExe);
      exit(0);
    }
  }	  

  if (iq != -1 && iv != -1) {
    fprintf(stderr,"ERROR: -v and -q cannot be set simultaneously.\n");
    exit(io.exit_exe);
  }

  for (i=1;i<argc;i++) {
    if (i == iv || i == iq) {} else {
      files.cIn=argv[i];
      ReadOptions(argv[i],options,&param,&pri,&sec,&files,&output,&io,&fdOneStep);
      
      if (io.iVerbose > 0) 
	printf("Read %s.\n",argv[i]);
      if (param.bLog) {
	WriteLog(param,pri,sec,output,files,io,0);
	if (io.iVerbose > 1)
	  printf("Log file written.\n");
      }
      
      /* Reverse Integration */
      if (param.bDoBackward) {
	Backward(&param,&pri,&sec,&output,&files,&io,fdOneStep);
	if (param.bLog) 
	  WriteLog(param,pri,sec,output,files,io,-1);
	
	if (io.iVerbose > 0) 
	  printf("Completed backward integration.\n");
      }
      
      /* Forward Integration */
      if (param.bDoForward) {
	Forward(&param,&pri,&sec,&output,&files,&io,fdOneStep);
	if (param.bLog) 
	  WriteLog(param,pri,sec,output,files,io,1);
	if (io.iVerbose > 0) 
	  printf("Completed forward integration.\n");
	
      }
    }
  }
  exit(0);
}
