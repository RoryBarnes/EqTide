/**************************************************************************
 * output.c                                                               *
 *                                                                        *
 * Written by Rory Barnes                                                 * 
 *                                                                        *
 * This file contains subroutines related to output, both the log and     *
 * forward or backward integration results.                               *
 *                                                                        *
 **************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include "eqtide.h"
#include "output.h"

void InitializeOutput(OUTPUT *output) {
  int i;
  
  for (i=0;i<NUMOUT;i++) {
    output->cParam[i] = InitializeString(OPTLEN);
    sprintf(output->cParam[i],"null");
    output->iNeg[i] = 0;
    output->dConvert[i] = 1;
  }
  
  /*
   * Age
   */
  
  sprintf(output->cParam[OUT_AGE],"Age");
  sprintf(output->cDescr[OUT_AGE],"System Age");
  output->iNeg[OUT_AGE] = 0;
  sprintf(output->cNeg[OUT_AGE],"Gyr");
  output->dConvert[OUT_AGE] = 1./(YEARSEC*1e9);
  
  /*
   * E
   */
  
  sprintf(output->cParam[OUT_EQSPINPERCONT],"EqSpinPerCont");
  sprintf(output->cDescr[OUT_EQSPINPERCONT],"CPL2 Continuous Equilibrium Spin Period");
  output->iNeg[OUT_EQSPINPERCONT] = 0;
  sprintf(output->cNeg[OUT_EQSPINPERCONT],"days");
  output->dConvert[OUT_EQSPINPERCONT] = 1./DAYSEC;
  
  sprintf(output->cParam[OUT_EQSPINRATECONT],"EqSpinRateCont");
  sprintf(output->cDescr[OUT_EQSPINRATECONT],"CPL2 Continuous Equilibrium Spin Rate");
  output->iNeg[OUT_EQSPINRATECONT] = 0;
  sprintf(output->cNeg[OUT_EQSPINRATECONT],"/day");
  output->dConvert[OUT_EQSPINRATECONT] = DAYSEC;
  
  sprintf(output->cParam[OUT_EQSPINPERDISCRETE],"EqSpinPerDiscrete");
  sprintf(output->cDescr[OUT_EQSPINPERDISCRETE],"CPL2 Discrete Equilibrium Spin Period");
  output->iNeg[OUT_EQSPINPERDISCRETE] = 0;
  sprintf(output->cNeg[OUT_EQSPINPERDISCRETE],"days");
  output->dConvert[OUT_EQSPINPERDISCRETE] = 1./DAYSEC;
  
  sprintf(output->cParam[OUT_EQSPINRATEDISCRETE],"EqSpinRateDiscrete");
  sprintf(output->cDescr[OUT_EQSPINRATEDISCRETE],"CPL2 Discrete Equilibrium Spin Rate");
  output->iNeg[OUT_EQSPINRATEDISCRETE] = 0;
  sprintf(output->cNeg[OUT_EQSPINRATEDISCRETE],"/day");
  output->dConvert[OUT_EQSPINRATEDISCRETE] = DAYSEC;
  
  /*
   *   ORBIT
   */
  
  sprintf(output->cParam[OUT_ORBANGMOM],"OrbAngMom");
  sprintf(output->cDescr[OUT_ORBANGMOM],"Orbital Angular Momentum");
  
  sprintf(output->cParam[OUT_ORBDADT],"DaDt");
  sprintf(output->cDescr[OUT_ORBDADT],"Total da/dt");
  output->iNeg[OUT_ORBDADT] = 0;
  sprintf(output->cNeg[OUT_ORBDADT],"AU/Gyr");
  output->dConvert[OUT_ORBDADT] = YEARSEC*1e9/AUCM;
  
  sprintf(output->cParam[OUT_ORBDEDT],"DeDt");
  sprintf(output->cDescr[OUT_ORBDEDT],"Total de/dt");
  output->iNeg[OUT_ORBDEDT] = 0; 
  sprintf(output->cNeg[OUT_ORBDEDT],"/Gyr");
  output->dConvert[OUT_ORBDEDT] = YEARSEC*1e9;
  
  sprintf(output->cParam[OUT_ORBDMMDT],"DMeanMotionDt");
  sprintf(output->cDescr[OUT_ORBDMMDT],"Total dMeanMotion/dt");
  
  sprintf(output->cParam[OUT_ORBDPERDT],"DPerDt");
  sprintf(output->cDescr[OUT_ORBDPERDT],"Total dOrbPer/dt");
  output->iNeg[OUT_ORBDPERDT] = 0;
  sprintf(output->cNeg[OUT_ORBDPERDT],"days/Gyr");
  output->dConvert[OUT_ORBDPERDT] = YEARSEC*1e9/DAYSEC;
  
  sprintf(output->cParam[OUT_ORBECC],"Eccentricity");
  sprintf(output->cDescr[OUT_ORBECC],"Orbital Eccentricity");
  
  sprintf(output->cParam[OUT_ORBEN],"OrbEnergy");
  sprintf(output->cDescr[OUT_ORBEN],"Orbital Energy");
  output->iNeg[OUT_ORBEN] = 0;
  sprintf(output->cNeg[OUT_ORBEN],"ergs");
  
  sprintf(output->cParam[OUT_ORBMEANMOTION],"MeanMotion");
  sprintf(output->cDescr[OUT_ORBMEANMOTION],"Orbital Mean Motion");
  output->iNeg[OUT_ORBMEANMOTION] = 0;
  sprintf(output->cNeg[OUT_ORBMEANMOTION],"/day");
  output->dConvert[OUT_ORBMEANMOTION] = DAYSEC;
  
  sprintf(output->cParam[OUT_ORBPER],"OrbPeriod");
  sprintf(output->cDescr[OUT_ORBPER],"Orbital Period");
  output->iNeg[OUT_ORBPER] = 0;
  sprintf(output->cNeg[OUT_ORBPER],"days");
  output->dConvert[OUT_ORBPER] = 1./DAYSEC;
  
  sprintf(output->cParam[OUT_ORBSEMI],"SemiMajorAxis");
  sprintf(output->cDescr[OUT_ORBSEMI],"Semi-major Axis");
  output->iNeg[OUT_ORBSEMI] = 0; 
  sprintf(output->cNeg[OUT_ORBSEMI],"AU");
  output->dConvert[OUT_ORBSEMI] = 1./AUCM;
  
  /*
   *   PRIMARY
   */
  
  sprintf(output->cParam[OUT_PRIANGMOM],"PriAngMom");
  sprintf(output->cDescr[OUT_PRIANGMOM],"Primary's Rotational Angular Momentum");
  
  sprintf(output->cParam[OUT_PRIDADT],"PriDaDt");
  sprintf(output->cDescr[OUT_PRIDADT],"Primary's Contribution to da/dt");
  output->iNeg[OUT_PRIDADT] = 0;
  sprintf(output->cNeg[OUT_PRIDADT],"AU/Gyr");
  output->dConvert[OUT_PRIDADT] = YEARSEC*1e9/AUCM;
  
  sprintf(output->cParam[OUT_PRIDEDT],"PriDeDt");
  sprintf(output->cDescr[OUT_PRIDEDT],"Primary's Contribution to de/dt");
  output->iNeg[OUT_PRIDEDT] = 0;
  sprintf(output->cNeg[OUT_PRIDEDT],"/Gyr");
  output->dConvert[OUT_PRIDEDT] = YEARSEC*1e9;
  
  sprintf(output->cParam[OUT_PRIDOBLDT],"PriDoblDt");
  sprintf(output->cDescr[OUT_PRIDOBLDT],"Time Rate of Change of Primary's Obliquity");
  output->iNeg[OUT_PRIDOBLDT] = 0;
  sprintf(output->cNeg[OUT_PRIDOBLDT],"deg/Gyr");
  output->dConvert[OUT_PRIDOBLDT] = YEARSEC*1e9/DEGRAD;
  
  sprintf(output->cParam[OUT_PRIDPERDT],"PriDperDt");
  sprintf(output->cDescr[OUT_PRIDPERDT],"Time Rate of Change of Primary's Rotation Period");
  output->iNeg[OUT_PRIDPERDT] = 0;
  sprintf(output->cNeg[OUT_PRIDPERDT],"days/Myr");
  output->dConvert[OUT_PRIDPERDT] = DAYSEC/(YEARSEC*1e6);
  
  sprintf(output->cParam[OUT_PRIDSPINDT],"PriDSpinDt");
  sprintf(output->cDescr[OUT_PRIDSPINDT],"Time Rate of Change of Primary's Rotation Rate");
  
  sprintf(output->cParam[OUT_PRIEQSPINPER],"PriEqSpinPer");
  sprintf(output->cDescr[OUT_PRIEQSPINPER],"Primary's Equilibrium Spin Period");
  output->iNeg[OUT_PRIEQSPINPER] = 0;
  sprintf(output->cNeg[OUT_PRIEQSPINPER],"days");
  output->dConvert[OUT_PRIEQSPINPER] = 1./DAYSEC;
  
  sprintf(output->cParam[OUT_PRIEQSPINRATE],"PriEqSpinRate");
  sprintf(output->cDescr[OUT_PRIEQSPINRATE],"Primary's Equilibrium Rotation Rate");
  output->iNeg[OUT_PRIEQSPINRATE] = 0;
  sprintf(output->cNeg[OUT_PRIEQSPINRATE],"/day");
  output->dConvert[OUT_PRIEQSPINRATE] = DAYSEC;
  
  sprintf(output->cParam[OUT_PRIGAMMAROT],"PriGammaRot");
  sprintf(output->cDescr[OUT_PRIGAMMAROT],"Primary's Gamma_Rotation");
  
  sprintf(output->cParam[OUT_PRIGAMMAORB],"PriGammaOrb");
  sprintf(output->cDescr[OUT_PRIGAMMAORB],"Primary's Gamma_Orbital");
  
  sprintf(output->cParam[OUT_PRIHEAT],"PriHeat");
  sprintf(output->cDescr[OUT_PRIHEAT],"Primary's Total Internal Heat");
  output->iNeg[OUT_PRIHEAT] = 0;
  sprintf(output->cNeg[OUT_PRIHEAT],"TW");
  output->dConvert[OUT_PRIHEAT] = 1e-19;
  
  sprintf(output->cParam[OUT_PRIOBL],"PriObliquity");
  sprintf(output->cDescr[OUT_PRIOBL],"Primary's Obliquity");
  
  sprintf(output->cParam[OUT_PRIROTEN],"PriRotEnergy");
  sprintf(output->cDescr[OUT_PRIROTEN],"Primary's Rotational Energy");
  output->iNeg[OUT_PRIROTEN] = 0;
  sprintf(output->cNeg[OUT_PRIROTEN],"ergs");
  
  sprintf(output->cParam[OUT_PRISPINPER],"PriPer");
  sprintf(output->cDescr[OUT_PRISPINPER],"Primary's Rotational Period");
  output->iNeg[OUT_PRISPINPER] = 0;
  sprintf(output->cNeg[OUT_PRISPINPER],"days");
  output->dConvert[OUT_PRISPINPER] = 1./DAYSEC;
  
  sprintf(output->cParam[OUT_PRISPINRATE],"PriSpinRate");
  sprintf(output->cDescr[OUT_PRISPINRATE],"Primary's Rotational Angular Frequency");
  output->iNeg[OUT_PRISPINRATE] = 0;
  sprintf(output->cNeg[OUT_PRISPINRATE],"/day");
  output->dConvert[OUT_PRISPINRATE] = DAYSEC;
  
  sprintf(output->cParam[OUT_PRISURFFLUX],"PriSurfFlux");
  sprintf(output->cDescr[OUT_PRISURFFLUX],"Primary's Surface Heat Flux");
  output->iNeg[OUT_PRISURFFLUX] = 0;
  sprintf(output->cNeg[OUT_PRISURFFLUX],"W/m^2");
  output->dConvert[OUT_PRISURFFLUX] = 1e-3;
  
  sprintf(output->cParam[OUT_PRITIDEHEAT],"PriTidePower");
  sprintf(output->cDescr[OUT_PRITIDEHEAT],"Primary's Tidal Power");
  output->iNeg[OUT_PRITIDEHEAT] = 0;
  sprintf(output->cNeg[OUT_PRITIDEHEAT],"TW");
  output->dConvert[OUT_PRITIDEHEAT] = 1e-19;

  sprintf(output->cParam[OUT_PRIVROT],"PriRotVel");
  sprintf(output->cDescr[OUT_PRIVROT],"Primary's Rotational Velocity");
  output->iNeg[OUT_PRIVROT] = 0;
  sprintf(output->cNeg[OUT_PRIVROT],"km/s");
  output->dConvert[OUT_PRIVROT] = 1e-5;
  
  /*
   *   SECONDARY
   */
  
  sprintf(output->cParam[OUT_SECANGMOM],"SecAngMom");
  sprintf(output->cDescr[OUT_SECANGMOM],"Secondary's Angular Momentum");
  
  sprintf(output->cParam[OUT_SECDADT],"SecDaDt");
  sprintf(output->cDescr[OUT_SECDADT],"Secondary's Contribution to da/dt");
  output->iNeg[OUT_SECDADT] = 0;
  sprintf(output->cNeg[OUT_SECDADT],"AU/Gyr");
  output->dConvert[OUT_SECDADT] = YEARSEC*1e9/AUCM;
  
  sprintf(output->cParam[OUT_SECDEDT],"SecDeDt");
  sprintf(output->cDescr[OUT_SECDEDT],"Secondary's Contribution to de/dt");
  output->iNeg[OUT_SECDEDT] = 0;
  sprintf(output->cNeg[OUT_SECDEDT],"/Gyr");
  output->dConvert[OUT_SECDEDT] = YEARSEC*1e9;
  
  sprintf(output->cParam[OUT_SECDOBLDT],"SecDoblDt");
  sprintf(output->cDescr[OUT_SECDOBLDT],"Time Rate of Change of Secondary's Obliquity");
  output->iNeg[OUT_SECDOBLDT] = 0;
  sprintf(output->cNeg[OUT_SECDOBLDT],"deg/Gyr");
  output->dConvert[OUT_SECDOBLDT] = YEARSEC*1e9/DEGRAD;
  
  sprintf(output->cParam[OUT_SECDPERDT],"SecDperDt");
  sprintf(output->cDescr[OUT_SECDPERDT],"Time Rate of Change of Secondary's Rotation Period");
  output->iNeg[OUT_SECDPERDT] = 0;
  sprintf(output->cNeg[OUT_SECDPERDT],"days/Gyr");
  output->dConvert[OUT_SECDPERDT] = YEARSEC*1e9/DAYSEC;
  
  sprintf(output->cParam[OUT_SECDSPINDT],"SecDspinDt");
  sprintf(output->cDescr[OUT_SECDSPINDT],"Time Rate of Change of Secondary's Rotation Rate");
  
  sprintf(output->cParam[OUT_SECEQSPINPER],"SecEqSpinPer");
  sprintf(output->cDescr[OUT_SECEQSPINPER],"Secondary's Equilibrium Spin Period");
  output->iNeg[OUT_SECEQSPINPER] = 0;
  sprintf(output->cNeg[OUT_SECEQSPINPER],"days");
  output->dConvert[OUT_SECEQSPINPER] = 1./DAYSEC;
  
  sprintf(output->cParam[OUT_SECEQSPINRATE],"SecEqSpinRate");
  sprintf(output->cDescr[OUT_SECEQSPINRATE],"Secondary's Equilibrium Rotation Rate");
  output->iNeg[OUT_SECEQSPINRATE] = 0;
  sprintf(output->cNeg[OUT_SECEQSPINRATE],"/day");
  output->dConvert[OUT_SECEQSPINRATE] = DAYSEC;
  
  sprintf(output->cParam[OUT_SECEQSURFFLUX],"SecEqSurfFlux");
  sprintf(output->cDescr[OUT_SECEQSURFFLUX],"Secondary's Surface Flux in Equilibrium Rotation");
  output->iNeg[OUT_SECEQSURFFLUX] = 0;
  sprintf(output->cNeg[OUT_SECEQSURFFLUX],"W/m^2");
  output->dConvert[OUT_SECEQSURFFLUX] = 1e-3;
  
  sprintf(output->cParam[OUT_SECGAMMAROT],"SecGammaRot");
  sprintf(output->cDescr[OUT_SECGAMMAROT],"Secondary's Gamma_Rotation");
  
  sprintf(output->cParam[OUT_SECGAMMAORB],"SecGammaOrb");
  sprintf(output->cDescr[OUT_SECGAMMAORB],"Secondary's Gamma_Orbital");
  
  sprintf(output->cParam[OUT_SECHEAT],"SecPower");
  sprintf(output->cDescr[OUT_SECHEAT],"Secondary's Power");
  output->iNeg[OUT_SECHEAT] = 0;
  sprintf(output->cNeg[OUT_SECHEAT],"TW");
  output->dConvert[OUT_SECHEAT] = 1e-19;
  
  sprintf(output->cParam[OUT_SECOBL],"SecObliquity");
  sprintf(output->cDescr[OUT_SECOBL],"Secondary's Obliquity");
  
  sprintf(output->cParam[OUT_SECQ],"SecQ");
  sprintf(output->cDescr[OUT_SECQ],"Secondary's Tidal Q");
  output->iNeg[OUT_SECQ] = 0;
  sprintf(output->cNeg[OUT_SECQ],"none");
  output->dConvert[OUT_SECQ] = 1;
  
  sprintf(output->cParam[OUT_SECROTEN],"SecRotEnergy");
  sprintf(output->cDescr[OUT_SECROTEN],"Secondary's Rotational Energy");
  output->iNeg[OUT_SECROTEN] = 0;
  sprintf(output->cNeg[OUT_SECROTEN],"ergs");
  
  sprintf(output->cParam[OUT_SECSPINPER],"SecPer");
  sprintf(output->cDescr[OUT_SECSPINPER],"Secondary's Rotational Period");
  output->iNeg[OUT_SECSPINPER] = 0;
  sprintf(output->cNeg[OUT_SECSPINPER],"days");
  output->dConvert[OUT_SECSPINPER] = 1./DAYSEC;
  
  sprintf(output->cParam[OUT_SECSPINRATE],"SecSpinRate");
  sprintf(output->cDescr[OUT_SECSPINRATE],"Secondary's Rotational Angular Frequency");
  output->iNeg[OUT_SECSPINRATE] = 0;
  sprintf(output->cNeg[OUT_SECSPINRATE],"/day");
  output->dConvert[OUT_SECSPINRATE] = DAYSEC;
  
  sprintf(output->cParam[OUT_SECSURFFLUX],"SecSurfFlux");
  sprintf(output->cDescr[OUT_SECSURFFLUX],"Secondary's Surface Heat Flux");
  output->iNeg[OUT_SECSURFFLUX] = 0;
  sprintf(output->cNeg[OUT_SECSURFFLUX],"W/m^2");
  output->dConvert[OUT_SECSURFFLUX] = 1e-3;
  
  sprintf(output->cParam[OUT_SECTIDELOCK],"SecTideLock");
  sprintf(output->cDescr[OUT_SECTIDELOCK],"Secondary Tidally Locked?");
  output->iNeg[OUT_SECTIDELOCK] = 0;
  sprintf(output->cNeg[OUT_SECTIDELOCK],"0,1");
  
  sprintf(output->cParam[OUT_SECVROT],"SecRotVel");
  sprintf(output->cDescr[OUT_SECVROT],"Secondary's Rotational Velocity");
  output->iNeg[OUT_SECVROT] = 0;
  sprintf(output->cNeg[OUT_SECVROT],"km/s");
  output->dConvert[OUT_SECVROT] = 1e-5;
  
  /* 
   * INTEGRATION PARAMETERS
   */
  
  sprintf(output->cParam[OUT_TIME],"Time");
  sprintf(output->cDescr[OUT_TIME],"Simulation Time");
  output->iNeg[OUT_TIME] = 0;
  sprintf(output->cNeg[OUT_TIME],"Gyr");
  output->dConvert[OUT_TIME] = 1./(YEARSEC*1e9);
  
  sprintf(output->cParam[OUT_DT],"DeltaTime");
  sprintf(output->cDescr[OUT_DT],"Average Timestep Over Last Output Interval");
  output->iNeg[OUT_DT] = 0;
  sprintf(output->cNeg[OUT_DT],"years");
  output->dConvert[OUT_DT] = 1./YEARSEC;
  
  sprintf(output->cParam[OUT_TOTANGMOM],"TotAngMom");
  sprintf(output->cDescr[OUT_TOTANGMOM],"Total Angular Momentum");
  
  sprintf(output->cParam[OUT_TOTEN],"TotEnergy");
  sprintf(output->cDescr[OUT_TOTEN],"Total Energy");
  output->iNeg[OUT_TOTEN] = 0;
  sprintf(output->cNeg[OUT_TOTEN],"ergs");
  
  /* 
   * TIMESCALES 
   */
  
  sprintf(output->cParam[OUT_TAUSEMI],"TauSemi");
  sprintf(output->cDescr[OUT_TAUSEMI],"Timescale for Semi-major Axis Evolution (a/[da/dt])");
  output->iNeg[OUT_TAUSEMI] = 0;
  sprintf(output->cNeg[OUT_TAUSEMI],"years");
  output->dConvert[OUT_TAUSEMI] = 1./YEARSEC;
  
  sprintf(output->cParam[OUT_TAUECC],"TauEcc");
  sprintf(output->cDescr[OUT_TAUECC],"Timescale for Eccentricity Evolution (e/[de/dt])");
  output->iNeg[OUT_TAUECC] = 0;
  sprintf(output->cNeg[OUT_TAUECC],"years");
  output->dConvert[OUT_TAUECC] = 1./YEARSEC;
  
  sprintf(output->cParam[OUT_TAUPRISPIN],"TauPriSpin");
  sprintf(output->cDescr[OUT_TAUPRISPIN],"Timescale for Primary's Rotational Evolution");
  output->iNeg[OUT_TAUPRISPIN] = 0;
  sprintf(output->cNeg[OUT_TAUPRISPIN],"years");
  output->dConvert[OUT_TAUPRISPIN] = 1./YEARSEC;
  
  sprintf(output->cParam[OUT_TAUPRIOBL],"TauPriObl");
  sprintf(output->cDescr[OUT_TAUPRIOBL],"Timescale for Primary's Obliquity Evolution");
  output->iNeg[OUT_TAUPRIOBL] = 0;
  sprintf(output->cNeg[OUT_TAUPRIOBL],"years");
  output->dConvert[OUT_TAUPRIOBL] = 1./YEARSEC;
  
  sprintf(output->cParam[OUT_TAUSECSPIN],"TauSecSpin");
  sprintf(output->cDescr[OUT_TAUSECSPIN],"Timescale for Secondary's Rotational Evolution");
  output->iNeg[OUT_TAUSECSPIN] = 0;
  sprintf(output->cNeg[OUT_TAUSECSPIN],"years");
  output->dConvert[OUT_TAUSECSPIN] = 1./YEARSEC;
  
  sprintf(output->cParam[OUT_TAUSECOBL],"TauSecObl");
  sprintf(output->cDescr[OUT_TAUSECOBL],"Timescale for Secondary's Obliquity Evolution");
  output->iNeg[OUT_TAUSECOBL] = 0;
  sprintf(output->cNeg[OUT_TAUSECOBL],"years");
  output->dConvert[OUT_TAUSEMI] = 1./YEARSEC;
}

void WriteLog(PARAM param,PRIMARY pri,SECONDARY sec,OUTPUT output,FILES files,IO io,int iEnd) {
  int i,j,k,foo; // foo is a placehold for EqSpinRate_CTL8
  FILE *fp;
  double tmp;
  char cTime[16],cTmp[OPTLEN];
  double *chi,*dTideHeat,*dTideSurfFlux;
  double dEqHeatFlux[2],dEqSpinRate[2];
  double *z,*f,dBeta; // CTL parameters
  double *zprime;
  int **epsilon; // CPL parameters

  epsilon = malloc(2*sizeof(int*));
  epsilon[0] = malloc(10*sizeof(int));
  epsilon[1] = malloc(10*sizeof(int));

  z = malloc(2*sizeof(double));
  zprime = malloc(2*sizeof(double));
  f = malloc(5*sizeof(double));
  chi = malloc(2*sizeof(double));
  dTideHeat = malloc(2*sizeof(double));
  dTideSurfFlux = malloc(2*sizeof(double));

  if (iEnd == 0) {
    sprintf(cTime,"Input");
    fp=fopen(files.cLog,"w");
  } else if (iEnd == 1) {
    sprintf(cTime,"Final");
    fp=fopen(files.cLog,"a");
  } else if (iEnd == -1) {
    sprintf(cTime,"Initial");
    fp=fopen(files.cLog,"a");
  }
  
  /*
   * General Control
   */

  if (!iEnd) {
    /* System Name */
    fprintf(fp,"System Name: %s\n",param.cSystemName);
    
    /* Tidal Model */
    fprintf(fp,"Tidal Model: ");
    if (param.iTideModel == CPL) 
      fprintf(fp,"Constant-Phase-Lag\n");
    else if (param.iTideModel == CTL) 
      fprintf(fp,"Constant-Time-Lag\n");
 
    if (param.iTideModel == CPL) 
      fprintf(fp,"Use Discrete Rotation Rate Model: %d\n",param.bDiscreteRot);

    /* Integration method */
    fprintf(fp,"Integration Method: ");
    if (param.iIntegration == EULER)
      fprintf(fp,"Euler\n");
    else if (param.iIntegration == RK4)
      fprintf(fp,"4th order Runge-Kutta\n");
 
    /* Verbosity */
    
    fprintf(fp,"Verbosity Level: %d\n",io.iVerbose);
    
    fprintf(fp,"\n--------- Input/Output Units ---------\n\n");
    
    /* Mass Units */
    
    fprintf(fp,"Mass Units: ");
    if (param.iUnitMass == 0) {
      fprintf(fp,"Grams\n");
    } else if (param.iUnitMass == 1) {
      fprintf(fp,"Kilograms\n");
    } else if (param.iUnitMass == 2) {
      fprintf(fp,"Solar\n");
    } else if (param.iUnitMass == 3) {
      fprintf(fp,"Earth\n");
    } else if (param.iUnitMass == 4) {
      fprintf(fp,"Jupiter\n");
    } else if (param.iUnitMass == 5) {
      fprintf(fp,"Saturn\n");
    }
    
    /* Length Units */
    
    fprintf(fp,"Length Units: ");
    if (param.iUnitLength == 0) {
      fprintf(fp,"Centimeters\n");
    } else if (param.iUnitLength == 1) {
      fprintf(fp,"Meters\n");
    } else if (param.iUnitLength == 2) {
      fprintf(fp,"Kilometers\n");
    } else if (param.iUnitLength == 3) {
      fprintf(fp,"Solar\n");
    } else if (param.iUnitLength == 4) {
      fprintf(fp,"Earth\n");
    } else if (param.iUnitLength == 5) {
      fprintf(fp,"Jupiter\n");
    } else if (param.iUnitLength == 6) {
      fprintf(fp,"AU\n");
    }
    
    /* Time Units */
    
    fprintf(fp,"Time Units: ");
    if (param.iUnitTime == 0) {
      fprintf(fp,"Seconds\n");
    } else if (param.iUnitTime == 1) {
      fprintf(fp,"Days\n");
    } else if (param.iUnitTime == 2) {
      fprintf(fp,"Years\n");
    } else if (param.iUnitTime == 3) {
      fprintf(fp,"Megayears\n");
    } else if (param.iUnitTime == 4) {
      fprintf(fp,"Gigayears\n");
    }
    
    /* Angle Units */
    
    fprintf(fp,"Angle Units: ");
    if (param.iUnitAngle == 0) {
      fprintf(fp,"Radians\n");
    } else if (param.iUnitAngle == 1) {
      fprintf(fp,"Degrees\n");
    }
    
    fprintf(fp,"Minimum Value of ecc and obl: ");
    fprintd(fp,param.dMinValue,io.iSciNot,io.iDigits);
    fprintf(fp,"\n");

    /* 
     * Files
     */
    
    fprintf(fp,"\n--------- Files ---------\n\n");
    fprintf(fp,"Executable: %s\n",files.cExe);
    fprintf(fp,"Input: %s\n",files.cIn);
    if (param.bDoForward) 
      fprintf(fp,"Forward Output: %s\n",files.cFor);
    if (param.bDoBackward)
      fprintf(fp,"Backward Output: %s\n",files.cBack);
    
    /*
     * Integration Paramters
     */
    
    if (param.bDoForward || param.bDoBackward) {
      fprintf(fp,"---------- Integration(s) ----------\n\n");

      fprintf(fp,"Use Variable Timestep: ");
      if (param.bVarDt == 0) {
	fprintf(fp,"No\n");
      } else {
	fprintf(fp,"Yes\n");
	fprintf(fp,"Timestep Coefficient: ");
	fprintd(fp,param.dTimestepCoeff,io.iSciNot,io.iDigits);
	fprintf(fp,"\n");
      }
    }
    
    if (param.bDoForward) {
      fprintf(fp,"\n--------- Forward Integration ---------\n\n");
      fprintf(fp,"%s Time Step: ",cTime);
      fprintd(fp,param.dForwTimeStep/dTimeUnit(param.iUnitTime,io.exit_units),io.iSciNot,io.iDigits);
      fprintf(fp,"\n");

      fprintf(fp,"Stop Time: ");
      fprintd(fp,param.dForwStopTime/dTimeUnit(param.iUnitTime,io.exit_units),io.iSciNot,io.iDigits);
      fprintf(fp,"\n");

      fprintf(fp,"Output Interval: ");
      fprintd(fp,param.dForwOutputTime/dTimeUnit(param.iUnitTime,io.exit_units),io.iSciNot,io.iDigits);
      fprintf(fp,"\n");

      fprintf(fp,"%s Minimum Semi-major Axis: ",cTime);
      fprintd(fp,param.halt.dMinSemi/dLengthUnit(param.iUnitLength,io.exit_units),io.iSciNot,io.iDigits);
      fprintf(fp,"\n");

    }   
     
    if (param.bDoBackward) {
      fprintf(fp,"\n--------- Backward Integration ---------\n\n");
      fprintf(fp,"%s Time Step: ",cTime);
      fprintd(fp,param.dBackTimeStep/dTimeUnit(param.iUnitTime,io.exit_units),io.iSciNot,io.iDigits);
      fprintf(fp,"\n");

      fprintf(fp,"Stop Time: ");
      fprintd(fp,param.dBackStopTime/dTimeUnit(param.iUnitTime,io.exit_units),io.iSciNot,io.iDigits);
      fprintf(fp,"\n");

      fprintf(fp,"Output Time: ");
      fprintd(fp,param.dBackOutputTime/dTimeUnit(param.iUnitTime,io.exit_units),io.iSciNot,io.iDigits);
      fprintf(fp,"\n");
    }

    /* Output Order */

    if (param.bDoBackward || param.bDoForward) {
      fprintf(fp,"Output Order: ");
      for (i=0;i<param.iNumCols;i++) {
	fprintf(fp,"%s",param.cOutputOrder[i]);
	for (j=0;j<NUMOUT;j++) {
	  if (!strcmp(param.cOutputOrder[i],output.cParam[j])) {
	    if (output.iNeg[j])
	      fprintf(fp,"[%s]",output.cNeg[j]);
	  }
	}
	fprintf(fp," ");
      }
    }
    fprintf(fp,"\n");
  } /* iEnd */

  /* 
   * Properties of Primary Body 
   */

  fprintf(fp,"\n--------- Primary Body ---------\n\n");
  if (!iEnd) {
    fprintf(fp,"Primary Mass: ");
    fprintd(fp,pri.dMass/dMassUnit(param.iUnitMass,io.exit_units),io.iSciNot,io.iDigits);
    fprintf(fp,"\n");
    
    fprintf(fp,"Primary Radius: ");
    fprintd(fp,pri.dRadius/dLengthUnit(param.iUnitLength,io.exit_units),io.iSciNot,io.iDigits);
    fprintf(fp,"\n");

    if (param.iTideModel == CPL) {
      fprintf(fp,"Primary Tidal Q: ");
      fprintd(fp,pri.dQ,io.iSciNot,io.iDigits);
      fprintf(fp,"\n");
    } else if (param.iTideModel == CTL) {
      fprintf(fp,"Primary Time Lag: ");
      fprintd(fp,pri.dTau/dTimeUnit(param.iUnitTime,io.exit_units),io.iSciNot,io.iDigits);
    }
    fprintf(fp,"\n");

    fprintf(fp,"Primary Love Number k2: ");
    fprintd(fp,pri.dK2,io.iSciNot,io.iDigits);
    fprintf(fp,"\n");

  } /* iEnd */

  fprintf(fp,"Primary %s Obliquity: ",cTime);
  fprintd(fp,pri.dObliquity/dAngleUnit(param.iUnitAngle,io.exit_units),io.iSciNot,io.iDigits);
  fprintf(fp,"\n");

  fprintf(fp,"Primary %s Spin Period: ",cTime);
  fprintd(fp,dFreqToPer(pri.dSpinRate)/dTimeUnit(param.iUnitTime,io.exit_units),io.iSciNot,io.iDigits);
  fprintf(fp,"\n");

  fprintf(fp,"Primary %s Spin Frequency: ",cTime);
  fprintd(fp,pri.dSpinRate*dTimeUnit(param.iUnitTime,io.exit_units),io.iSciNot,io.iDigits);
  fprintf(fp,"\n");

  if (!iEnd) {
    fprintf(fp,"Primary Radius of Gyration: ");
    fprintd(fp,pri.dRG,io.iSciNot,io.iDigits);
    fprintf(fp,"\n");
    
    if (pri.iMassRad > 0) {
      fprintf(fp,"Primary Mass-Radius Relationship: ");
      if (pri.iMassRad == 1) {
	fprintf(fp,"New Light On Dark Stars\n");
      } else if (pri.iMassRad == 2) {
	fprintf(fp,"Gorda, S.Y. & Svechnikov, M.A. 1999, Astronomy Reports, 43, 521-525.\n");
      } else if (pri.iMassRad == 3) {
	fprintf(fp,"Bayless, A.J. & Orosz, J.A. 2006, ApJ, 651, 1155-1165.\n");
      } else if (pri.iMassRad == 4) {
	fprintf(fp,"Sotin, C. et al. 2007, Icarus, 191, 337-351.\n");
      } else if (pri.iMassRad == 5) {
	fprintf(fp,"Baraffe, I. et al. 2015, A&A, 577, 42.\n");
      }
    }
  } /* iEnd */

  /* 
   * Properties of Secondary
   */

  fprintf(fp,"\n--------- Secondary Body ---------\n\n");
  if (!iEnd) {
    fprintf(fp,"Secondary Mass: ");
    fprintd(fp,sec.dMass/dMassUnit(param.iUnitMass,io.exit_units),io.iSciNot,io.iDigits);
    fprintf(fp,"\n");
    
    fprintf(fp,"Secondary Radius: ");
    fprintd(fp,sec.dRadius/dLengthUnit(param.iUnitLength,io.exit_units),io.iSciNot,io.iDigits);
    fprintf(fp,"\n");
    
    if (param.iTideModel == CPL) {
      fprintf(fp,"Secondary Tidal Q: ");
      fprintd(fp,sec.dQ,io.iSciNot,io.iDigits);
      fprintf(fp,"\n");
    } else if (param.iTideModel == CTL) {
      fprintf(fp,"Secondary Time Lag: ");
      fprintd(fp,sec.dTau/dTimeUnit(param.iUnitTime,io.exit_units),io.iSciNot,io.iDigits);
      fprintf(fp,"\n");
    }

    fprintf(fp,"Secondary Love Number k2: ");
    fprintd(fp,sec.dK2,io.iSciNot,io.iDigits);
    fprintf(fp,"\n");
    
    fprintf(fp,"Secondary Radius of Gyration: ");
    fprintd(fp,sec.dRG,io.iSciNot,io.iDigits);
    fprintf(fp,"\n");
    
    if (sec.iMassRad > 0) {
      fprintf(fp,"Secondary Mass-Radius Relationship: ");
      if (sec.iMassRad == 1) {
	fprintf(fp,"New Light On Dark Stars\n");
      } else if (sec.iMassRad == 2) {
	fprintf(fp,"Gorda, S.Y. & Svechnikov, M.A. 1999, Astronomy Reports, 43, 521-525.\n");
      } else if (sec.iMassRad == 3) {
	fprintf(fp,"Bayless, A.J. & Orosz, J.A. 2006, ApJ, 651, 1155-1165.\n");
      } else if (sec.iMassRad == 4) {
	fprintf(fp,"Sotin, C. et al. 2007, Icarus, 191, 337-351.\n");
      }
    }
  }

  fprintf(fp,"Secondary %s Obliquity: ",cTime);
  fprintd(fp,sec.dObliquity/dAngleUnit(param.iUnitAngle,io.exit_units),io.iSciNot,io.iDigits);
  fprintf(fp,"\n");
  
  fprintf(fp,"Secondary %s Spin Period: ",cTime);
  fprintd(fp,dFreqToPer(sec.dSpinRate)/dTimeUnit(param.iUnitTime,io.exit_units),io.iSciNot,io.iDigits);
  fprintf(fp,"\n");
  
  fprintf(fp,"Secondary %s Spin Frequency: ",cTime);
  fprintd(fp,sec.dSpinRate*dTimeUnit(param.iUnitTime,io.exit_units),io.iSciNot,io.iDigits);
  fprintf(fp,"\n");
  
  tmp=2*atan(pri.dRadius/sec.dSemi);
  if (param.iUnitAngle == 1)
    tmp /= dAngleUnit(param.iUnitAngle,io.exit_units);
  fprintf(fp,"%s Angular Size of Primary body: ",cTime);
  fprintd(fp,tmp,io.iSciNot,io.iDigits);
  fprintf(fp,"\n");

  fprintf(fp,"\n-------------- Orbit ----------\n\n");
  
  fprintf(fp,"%s Semi-major Axis: ",cTime);
  fprintd(fp,sec.dSemi/dLengthUnit(param.iUnitLength,io.exit_units),io.iSciNot,io.iDigits);
  fprintf(fp,"\n");

  fprintf(fp,"%s Orbital Mean Motion: ",cTime);
  fprintd(fp,sec.dMeanMotion*dTimeUnit(param.iUnitTime,io.exit_units),io.iSciNot,io.iDigits);
  fprintf(fp,"\n");

  fprintf(fp,"%s Orbital Period: ",cTime);
  fprintd(fp,dFreqToPer(sec.dMeanMotion)/dTimeUnit(param.iUnitTime,io.exit_units),io.iSciNot,io.iDigits);
  fprintf(fp,"\n");

  fprintf(fp,"%s Eccentricity: ",cTime);
  fprintd(fp,sec.dEcc,io.iSciNot,io.iDigits);
  fprintf(fp,"\n");
       
  /* 
   * Tidal properties 
   */

  fprintf(fp,"\n--------- Tidal Effects ---------\n\n");

  if (param.iTideModel == CPL) {
    /* Constant-Phase-Lag, order 2 */

    DerivsCPL(&pri,&sec,&io,zprime,chi,f,dBeta,epsilon,0,param.bDiscreteRot);

    dEqSpinRate[0] = EqSpinRate_CPL2(sec.dMeanMotion,sec.dEcc,pri.dObliquity,param.bDiscreteRot);
    dEqSpinRate[1] = EqSpinRate_CPL2(sec.dMeanMotion,sec.dEcc,sec.dObliquity,param.bDiscreteRot);

    /* in Watts */
    dTideHeat[0] = dTideHeat_CPL2(epsilon[0],zprime[0],sec.dEcc,sec.dMeanMotion,pri.dSpinRate,pri.dObliquity)/1e7;
    dTideHeat[1] = dTideHeat_CPL2(epsilon[1],zprime[1],sec.dEcc,sec.dMeanMotion,sec.dSpinRate,sec.dObliquity)/1e7;

    /* in W/m^2 */
    dTideSurfFlux[0] = dTideHeat[0]/(4*PI*pri.dRadius*pri.dRadius)*1e4;
    dTideSurfFlux[1] = dTideHeat[1]/(4*PI*sec.dRadius*sec.dRadius)*1e4;

    /* in W/m^2 */
    dEqHeatFlux[0] = dTideHeatEq_CPL2(zprime[0],sec.dEcc,pri.dObliquity,sec.dMeanMotion,param.bDiscreteRot)/(4*PI*pri.dRadius*pri.dRadius)/1e3;
    dEqHeatFlux[1] = dTideHeatEq_CPL2(zprime[1],sec.dEcc,sec.dObliquity,sec.dMeanMotion,param.bDiscreteRot)/(4*PI*sec.dRadius*sec.dRadius)/1e3;

  } else if (param.iTideModel == CTL) {
    dBeta = AssignBeta(sec.dEcc);
    
    DerivsCTL(&pri,&sec,&io,z,chi,f,dBeta,epsilon,0,param.bDiscreteRot);

    dEqSpinRate[0] = EqSpinRate_CTL8(sec.dMeanMotion,sec.dEcc,pri.dObliquity,param.bDiscreteRot);
    dEqSpinRate[1] = EqSpinRate_CTL8(sec.dMeanMotion,sec.dEcc,sec.dObliquity,param.bDiscreteRot);

    /* in Watts */
    dTideHeat[0]=dTideHeat_CTL8(z[0],f,dBeta,pri.dSpinRate,pri.dObliquity,sec.dMeanMotion)/1e7;
    dTideHeat[1]=dTideHeat_CTL8(z[1],f,dBeta,sec.dSpinRate,sec.dObliquity,sec.dMeanMotion)/1e7;

    /* in W/m^2 */
    dTideSurfFlux[0]=dTideHeat[0]/(4*PI*pri.dRadius*pri.dRadius)*1e4;
    dTideSurfFlux[1]=dTideHeat[1]/(4*PI*sec.dRadius*sec.dRadius)*1e4;

    dEqHeatFlux[0] = dTideHeatEq_CTL8(z[0],f,dBeta,pri.dObliquity,sec.dMeanMotion)/(4*PI*pri.dRadius*pri.dRadius)/1e3;
    dEqHeatFlux[1] = dTideHeatEq_CTL8(z[1],f,dBeta,sec.dObliquity,sec.dMeanMotion)/(4*PI*sec.dRadius*sec.dRadius)/1e3;
  }

  fprintf(fp,"da/dt: ");
  fprintd(fp,sec.dDaDt*dTimeUnit(param.iUnitTime,io.exit_units)/dLengthUnit(param.iUnitLength,io.exit_units),io.iSciNot,io.iDigits);
  fprintf(fp,"\n");
  fprintf(fp,"de/dt: ");
  fprintd(fp,sec.dDeDt*dTimeUnit(param.iUnitTime,io.exit_units),io.iSciNot,io.iDigits);
  fprintf(fp,"\n");

  tmp = DOrbPerDt(pri.dMass,sec.dSemi,sec.dDaDt);
  fprintf(fp,"dP/dt: ");
  fprintd(fp,tmp,io.iSciNot,io.iDigits);
  fprintf(fp,"\n");

  fprintf(fp,"\n");

  fprintf(fp,"Derivative of Primary's Spin Rate: ");
  fprintd(fp,pri.dDomegaDt/param.iUnitTime,io.iSciNot,io.iDigits);
  fprintf(fp,"\n");
  fprintf(fp,"Primary dObl/dt: ");
  fprintd(fp,pri.dDobliquityDt*param.iUnitAngle/param.iUnitTime,io.iSciNot,io.iDigits);
  fprintf(fp,"\n");
  fprintf(fp,"Primary Equilibrium Rotation Rate: ");
  fprintd(fp,dEqSpinRate[0]*dTimeUnit(param.iUnitTime,io.exit_units),io.iSciNot,io.iDigits);
  fprintf(fp,"\n");
  fprintf(fp,"Primary Equilibrium Rotation Period: ");
  fprintd(fp,dFreqToPer(dEqSpinRate[0])/dTimeUnit(param.iUnitTime,io.exit_units),io.iSciNot,io.iDigits);
  fprintf(fp,"\n");
  fprintf(fp,"Primary Tidal Heat [W]: ");
  fprintd(fp,dTideHeat[0],io.iSciNot,io.iDigits);
  fprintf(fp,"\n");
  fprintf(fp,"Primary Tidal Surface Flux [W/m^2]: ");
  fprintd(fp,dTideSurfFlux[0],io.iSciNot,io.iDigits);
  fprintf(fp,"\n");
  fprintf(fp,"Primary Equilibrium Tidal Surface Flux [W/m^2]: ");
  fprintd(fp,dEqHeatFlux[0],io.iSciNot,io.iDigits);
  fprintf(fp,"\n");
  fprintf(fp,"\n");

  fprintf(fp,"Derivative of Secondary's Spin Rate: ");
  fprintd(fp,sec.dDomegaDt,io.iSciNot,io.iDigits);
  fprintf(fp,"\n");
  fprintf(fp,"Secondary dObl/dt: ");
  fprintd(fp,sec.dDobliquityDt*param.iUnitAngle/param.iUnitTime,io.iSciNot,io.iDigits);
  fprintf(fp,"\n");
  fprintf(fp,"Secondary Equilibrium Rotation Rate: ");
  fprintd(fp,dEqSpinRate[1]*dTimeUnit(param.iUnitTime,io.exit_units),io.iSciNot,io.iDigits);
  fprintf(fp,"\n");
  fprintf(fp,"Secondary Equilibrium Rotation Period: ");
  fprintd(fp,dFreqToPer(dEqSpinRate[1])/dTimeUnit(param.iUnitTime,io.exit_units),io.iSciNot,io.iDigits);
  fprintf(fp,"\n");
  fprintf(fp,"Secondary Tidal Heat [W]: ");
  fprintd(fp,dTideHeat[1],io.iSciNot,io.iDigits);
  fprintf(fp,"\n");
  fprintf(fp,"Secondary Tidal Surface Flux [W/m^2]: ");
  fprintd(fp,dTideSurfFlux[1],io.iSciNot,io.iDigits);
  fprintf(fp,"\n");
  fprintf(fp,"Secondary Equilibrium Tidal Surface Flux [W/m^2]: ");
  fprintd(fp,dEqHeatFlux[1],io.iSciNot,io.iDigits);
  fprintf(fp,"\n\n");

  if (param.iTideModel == CPL) {
    fprintf(fp,"Use Discrete Rotation States: %d\n",param.bDiscreteRot);
    fprintf(fp,"Discrete Equilibrium Spin Period: ");
    fprintd(fp,dFreqToPer(EqSpinRate_CPL2Discrete(sec.dMeanMotion,sec.dEcc))/dTimeUnit(param.iUnitTime,io.exit_units),io.iSciNot,io.iDigits);
    fprintf(fp,"\n");
    fprintf(fp,"Continuous Equilibrium Spin Period: ");
    fprintd(fp,dFreqToPer(EqSpinRate_CPL2Cont(sec.dMeanMotion,sec.dEcc))/dTimeUnit(param.iUnitTime,io.exit_units),io.iSciNot,io.iDigits);
    fprintf(fp,"\n");

    fprintf(fp,"Primary epsilons: %d %d %d %d %d %d\n",epsilon[0][0],epsilon[0][1],epsilon[0][2],epsilon[0][5],epsilon[0][8],epsilon[0][9]);
    fprintf(fp,"Primary Gamma_Rotation: ");
    fprintd(fp,dGammaRot(sec.dEcc,pri.dObliquity,epsilon[0]),io.iSciNot,io.iDigits);
    fprintf(fp,"\n");
    fprintf(fp,"Primary Gamma_Orbital: ");
    fprintd(fp,dGammaOrb(sec.dEcc,pri.dObliquity,epsilon[0]),io.iSciNot,io.iDigits);
    fprintf(fp,"\n");

    fprintf(fp,"Secondary epsilons: %d %d %d %d %d %d\n",epsilon[1][0],epsilon[1][1],epsilon[1][2],epsilon[1][5],epsilon[1][8],epsilon[1][9]);
    fprintf(fp,"Secondary Gamma_Rotation: ");
    fprintd(fp,dGammaRot(sec.dEcc,sec.dObliquity,epsilon[1]),io.iSciNot,io.iDigits);
    fprintf(fp,"\n");
    fprintf(fp,"Secondary Gamma_Orbital: ");
    fprintd(fp,dGammaOrb(sec.dEcc,sec.dObliquity,epsilon[1]),io.iSciNot,io.iDigits);
    fprintf(fp,"\n");
    
  } else if (param.iTideModel == CTL) {
    fprintf(fp,"Secondary's Equilibrium Rotation Period: ");
    fprintd(fp,dFreqToPer(dEqSpinRate[1])/dTimeUnit(param.iUnitTime,io.exit_units),io.iSciNot,io.iDigits);
    fprintf(fp,"\n");
  }

  fprintf(fp,"\n----------- Halt ---------\n\n");

  /* Minimum Obliquity */
  fprintf(fp,"Primary's Minimum Obliquity: ");
  if (param.iUnitAngle == 0) {
    tmp=param.halt.dMinPriObl;
  } else {
    tmp=param.halt.dMinPriObl/dAngleUnit(param.iUnitAngle,io.exit_units);
  }
  fprintd(fp,tmp,io.iSciNot,io.iDigits);
  fprintf(fp,"\n");

  fprintf(fp,"Secondary's Minimum Obliquity: ");
  if (param.iUnitAngle == 0) {
    tmp=param.halt.dMinSecObl;
  } else {
    tmp=param.halt.dMinSecObl/dAngleUnit(param.iUnitAngle,io.exit_units);
  }
  fprintd(fp,tmp,io.iSciNot,io.iDigits);
  fprintf(fp,"\n");

  if (param.halt.bDblSync) 
    fprintf(fp,"HALT at double synchronous state.\n");
  
  free(epsilon[1]);
  free(epsilon[0]);
  free(epsilon);
  free(z);
  free(zprime);
  free(f);
  free(chi);
  free(dTideHeat);
  free(dTideSurfFlux);
}

void Output(PARAM *param,PRIMARY *pri,SECONDARY *sec,OUTPUT *output,IO *io,double dTime,double dDt,FILE *fp) {
  int j,k,ncol,n=0;
  double col[NUMOUT];
  int **epsilon,foo; 
  double *z,*chi,*f,beta;
  
  ncol=0;
  
  epsilon=malloc(2*sizeof(int*));
  epsilon[0]=malloc(10*sizeof(int));
  epsilon[1]=malloc(10*sizeof(int));
  
  z=malloc(2*sizeof(double));
  chi=malloc(2*sizeof(double));
  f=malloc(5*sizeof(double));
  
  if (param->iTideModel == CPL) {
    AssignEpsilon(pri->dSpinRate,sec->dMeanMotion,epsilon[0]);
    AssignEpsilon(sec->dSpinRate,sec->dMeanMotion,epsilon[1]);
    AssignZprime(pri,sec,z);
  } else if (param->iTideModel == CTL) {
    f[0]=AssignF1(sec->dEcc);
    f[1]=AssignF2(sec->dEcc);
    f[2]=AssignF3(sec->dEcc);
    f[3]=AssignF4(sec->dEcc);
    f[4]=AssignF5(sec->dEcc);
    beta=AssignBeta(sec->dEcc);
    AssignZ(pri,sec,z);
  }
  AssignChi(pri,sec,chi);
  
  /* Assign outputs */
  for (j=0;j<NUMOUT;j++) {
    if (!strcmp(param->cOutputOrder[j],output->cParam[OUT_EQSPINPERCONT])) {
      col[j+n]=dFreqToPer(EqSpinRate_CPL2Cont(sec->dMeanMotion,sec->dEcc));
      if (output->iNeg[OUT_EQSPINPERCONT]) 
	col[j+n] *= output->dConvert[OUT_EQSPINPERCONT];
      else
	col[j+n] /= dTimeUnit(param->iUnitTime,io->exit_units);
      ncol++;
    }
    
    if (!strcmp(param->cOutputOrder[j],output->cParam[OUT_EQSPINRATECONT])) {
      col[j+n]=EqSpinRate_CPL2Cont(sec->dMeanMotion,sec->dEcc);
      if (output->iNeg[OUT_EQSPINRATECONT]) 
	col[j+n] *= output->dConvert[OUT_EQSPINRATECONT];
      else
	col[j+n] /= dTimeUnit(param->iUnitTime,io->exit_units);
      ncol++;
    }
    
    if (!strcmp(param->cOutputOrder[j],output->cParam[OUT_EQSPINPERDISCRETE])) {
      col[j+n]=dFreqToPer(EqSpinRate_CPL2Discrete(sec->dMeanMotion,sec->dEcc));
      if (output->iNeg[OUT_EQSPINPERDISCRETE]) 
	col[j+n] *= output->dConvert[OUT_EQSPINPERDISCRETE];
      else
	col[j+n] /= dTimeUnit(param->iUnitTime,io->exit_units);
      ncol++;
    }
    
    if (!strcmp(param->cOutputOrder[j],output->cParam[OUT_EQSPINRATEDISCRETE])) {
      col[j+n]=EqSpinRate_CPL2Discrete(sec->dMeanMotion,sec->dEcc);
      if (output->iNeg[OUT_EQSPINRATEDISCRETE]) 
	col[j+n] *= output->dConvert[OUT_EQSPINRATEDISCRETE];
      else
	col[j+n] /= dTimeUnit(param->iUnitTime,io->exit_units);
      ncol++;
    }
    
    /*
     *   ORBIT
     */
    
    /* Orbital Angular Momentum */
    if (!strcmp(param->cOutputOrder[j],output->cParam[OUT_ORBANGMOM])) {
      col[j+n]=dOrbAngMom(pri,sec);
      ncol++;
    }
    
    /* Eccentricity */
    if (!strcmp(param->cOutputOrder[j],output->cParam[OUT_ORBECC])) {
      col[j+n]=sec->dEcc;
      ncol++;
    }
    
    /* Total da/dt */
    if (!strcmp(param->cOutputOrder[j],output->cParam[OUT_ORBDADT])) {
      col[j+n]=sec->dDaDt;
      if (output->iNeg[OUT_ORBDADT])
	col[j+n] *= output->dConvert[OUT_ORBDADT];
      else /* AU/Gyr */
	col[j+n] *= dTimeUnit(param->iUnitTime,io->exit_units)/dLengthUnit(param->iUnitLength,io->exit_units);
      ncol++;
    }
    
    /* Total dOrbPerDt */
    
    /* de/dt */
    if (!strcmp(param->cOutputOrder[j],output->cParam[OUT_ORBDEDT])) {
      col[j+n]=sec->dDeDt;
      if (output->iNeg[OUT_ORBDEDT])
	col[j+n] *= output->dConvert[OUT_ORBDEDT];
      else /* /Gyr */
	col[j+n] *= dTimeUnit(param->iUnitTime,io->exit_units);
      ncol++;
    }
    
    /* Mean Motion */
    if (!strcmp(param->cOutputOrder[j],output->cParam[OUT_ORBMEANMOTION])) {
      col[j+n]=sec->dMeanMotion*dTimeUnit(param->iUnitTime,io->exit_units);
      ncol++;
    }
    
    /* Semi-Major Axis */
    if (!strcmp(param->cOutputOrder[j],output->cParam[OUT_ORBSEMI])) {
      col[j+n]=sec->dSemi;
      if (output->iNeg[OUT_ORBSEMI])
	col[j+n] *= output->dConvert[OUT_ORBSEMI];
      else /* AU */
	col[j+n] /= dLengthUnit(param->iUnitLength,io->exit_units);
      ncol++;
    }
    
    /* Orbital Period */
    if (!strcmp(param->cOutputOrder[j],output->cParam[OUT_ORBPER])) {
      col[j+n]=a2p(sec->dSemi,(sec->dMass+pri->dMass));
      if (output->iNeg[OUT_ORBPER])
	col[j+n] *= output->dConvert[OUT_ORBPER];
      else 
	col[j+n] /= dTimeUnit(param->iUnitTime,io->exit_units);
      ncol++;
    }
    
    /*
     *   PRIMARY
     */
    
    /* Rotational Angular Momentum */
    if (!strcmp(param->cOutputOrder[j],output->cParam[OUT_PRIANGMOM])) {
      col[j+n]=dSpinAngMom(pri->dRG,pri->dMass,pri->dRadius,pri->dSpinRate);
      ncol++;
    }
    
    /* Primary's Contribution to da/dt */
    if (!strcmp(param->cOutputOrder[j],output->cParam[OUT_PRIDADT])) {
      if (param->iTideModel == CPL)  /* CPL2 */
	col[j+n] = dDaDt1_CPL2(pri->dMass,sec->dMass,sec->dSemi,sec->dEcc,pri->dObliquity,epsilon[0],z[0]);
      else /* CTL8 */
	col[j+n] = dDaDt1_CTL8(pri->dMass,sec->dMass,sec->dSemi,sec->dEcc,sec->dMeanMotion,z[0],pri->dSpinRate,pri->dObliquity,f,beta);
      
      if (output->iNeg[OUT_PRIDADT])
	col[j+n] *= output->dConvert[OUT_PRIDADT];
      else
	col[j+n] *= dTimeUnit(param->iUnitTime,io->exit_units)/dLengthUnit(param->iUnitLength,io->exit_units);
      ncol++;
    }
    
    /* Primary's Contribution to de/dt */
    if (!strcmp(param->cOutputOrder[j],output->cParam[OUT_PRIDEDT])) {
      if (param->iTideModel == CPL)  /* CPL2 */
	col[j+n] = dDeDt1_CPL2(pri->dMass,sec->dMass,sec->dSemi,sec->dEcc,epsilon[0],z[0]);
      else /* CTL8 */
	col[j+n] = dDeDt1_CTL8(pri->dMass,sec->dMass,sec->dSemi,sec->dEcc,sec->dMeanMotion,z[0],pri->dSpinRate,pri->dObliquity,f,beta);
      
      if (output->iNeg[OUT_PRIDEDT])
	col[j+n] *= output->dConvert[OUT_PRIDEDT];
      else
	col[j+n] *= dTimeUnit(param->iUnitTime,io->exit_units)/dLengthUnit(param->iUnitLength,io->exit_units);
      
      ncol++;
    }
    
    /* Rate of Change of Obliquity in Primary Body */
    if (!strcmp(param->cOutputOrder[j],output->cParam[OUT_PRIDOBLDT])) {
      col[j+n]=pri->dDobliquityDt;
      if (output->iNeg[OUT_PRIDOBLDT])
	col[j+n] *= output->dConvert[OUT_PRIDOBLDT];
      else /* deg/Gyr */
	col[j+n] *= dTimeUnit(param->iUnitTime,io->exit_units)/dAngleUnit(param->iUnitAngle,io->exit_units);
      ncol++;
    }
    
    /* Rate of Change of Rotation Period of Primary Body */
    if (!strcmp(param->cOutputOrder[j],output->cParam[OUT_PRIDPERDT])) {
      col[j+n]=dDPerDt(pri->dSpinRate,pri->dDomegaDt);
      
      if (output->iNeg[OUT_PRIDPERDT]) 
	col[j+n] *= output->dConvert[OUT_PRIDPERDT];
      else 
	col[j+n] *= dTimeUnit(param->iUnitTime,io->exit_units)*dTimeUnit(param->iUnitTime,io->exit_units);
      
      ncol++;
    }
    
    /* Rate of Change of Primary's Spin Rate*/
    if (!strcmp(param->cOutputOrder[j],output->cParam[OUT_PRIDSPINDT])) {
      col[j+n]=pri->dDomegaDt;
      ncol++;
    }
    
    /* Primary's Equilibrium Spin Period */
    if (!strcmp(param->cOutputOrder[j],output->cParam[OUT_PRIEQSPINPER])) {

      col[j+n] = dFreqToPer(param->fEqSpin(sec->dMeanMotion,sec->dEcc,pri->dObliquity,param->bDiscreteRot));

      if (output->iNeg[OUT_PRIEQSPINPER])
	col[j+n] *= output->dConvert[OUT_PRIEQSPINPER];
      else
	col[j+n] /= dTimeUnit(param->iUnitTime,io->exit_units);
      ncol++;
    }
    
    /* Primary's Equilibrium Spin Rate */
    if (!strcmp(param->cOutputOrder[j],output->cParam[OUT_PRIEQSPINRATE])) {

      col[j+n] = param->fEqSpin(sec->dMeanMotion,sec->dEcc,pri->dObliquity,param->bDiscreteRot);

      if (output->iNeg[OUT_PRIEQSPINRATE])
	col[j+n] *= output->dConvert[OUT_PRIEQSPINRATE];
      else
	col[j+n] *= dTimeUnit(param->iUnitTime,io->exit_units);
      ncol++;
    }
    
    /* Primary's Gamma_Orbital */
    if (!strcmp(param->cOutputOrder[j],output->cParam[OUT_PRIGAMMAORB])) {
      col[j+n]=dGammaOrb(sec->dEcc,pri->dObliquity,epsilon[0]);
      ncol++;
    }
    
    /* Primary's Gamma_Rotation */
    if (!strcmp(param->cOutputOrder[j],output->cParam[OUT_PRIGAMMAROT])) {
      col[j+n]=dGammaRot(sec->dEcc,pri->dObliquity,epsilon[0]);
      ncol++;
    }
    
    /* Primary Body's Heat */
    if (!strcmp(param->cOutputOrder[j],output->cParam[OUT_PRIHEAT])) {
      if (param->iTideModel == CPL)  /* CPL2 */
	col[j+n]=dTideHeat_CPL2(epsilon[0],z[0],sec->dEcc,sec->dMeanMotion,pri->dSpinRate,pri->dObliquity);
      else /* CTL8 */
	col[j+n]=dTideHeat_CTL8(z[0],f,beta,pri->dSpinRate,pri->dObliquity,sec->dMeanMotion);
      
      if (output->iNeg[OUT_PRIHEAT]) 
	col[j+n] *= output->dConvert[OUT_PRIHEAT];
      else /* TeraWatts */
	/* Convert to system units?! */
	{}
      ncol++;
    }
    
    /* Primary's Obliquity */
    if (!strcmp(param->cOutputOrder[j],output->cParam[OUT_PRIOBL])) {
      col[j+n]=pri->dObliquity/dAngleUnit(param->iUnitAngle,io->exit_output);
      ncol++;
    }
    
    /* Primary's Rotational Energy */
    if (!strcmp(param->cOutputOrder[j],output->cParam[OUT_PRIROTEN])) {
      col[j+n]=dRotEn(pri->dMass,pri->dRadius,pri->dRG,pri->dSpinRate);
      if (output->iNeg[OUT_PRIROTEN]) 
	/* ergs is for negative, so do nothing */
	{}
      else
	/* Convert to System Units!? */
	{}
      ncol++;
    }
    
    /* Primary's Spin Period */
    if (!strcmp(param->cOutputOrder[j],output->cParam[OUT_PRISPINPER])) {
      col[j+n] = dFreqToPer(pri->dSpinRate);
      if (output->iNeg[OUT_PRISPINPER])
	col[j+n] *= output->dConvert[OUT_PRISPINPER];
      else
	col[j+n] /= dTimeUnit(param->iUnitTime,io->exit_param);
      ncol++;
    }
    
    /* Primary's Rotation Rate */
    if (!strcmp(param->cOutputOrder[j],output->cParam[OUT_PRISPINRATE])) {
      col[j+n]=pri->dSpinRate;
      if (output->iNeg[OUT_PRISPINRATE])
	col[j+n] *= output->dConvert[OUT_PRISPINRATE];
      else
	col[j+n] *= dTimeUnit(param->iUnitTime,io->exit_output);
      ncol++;
    }
    
    /* Primary's Surface Heat Flux */
    if (!strcmp(param->cOutputOrder[j],output->cParam[OUT_PRISURFFLUX])) {
      if (param->iTideModel == CPL)  /* CPL2 */
	col[j+n]=dTideHeat_CPL2(epsilon[0],z[0],sec->dEcc,sec->dMeanMotion,pri->dSpinRate,pri->dObliquity);
      else /* CTL8 */
	col[j+n]=dTideHeat_CTL8(z[0],f,beta,pri->dSpinRate,pri->dObliquity,sec->dMeanMotion);
      col[j+n] /= 4*PI*pri->dRadius*pri->dRadius;
      
      if (output->iNeg[OUT_PRISURFFLUX])
	col[j+n] *= output->dConvert[OUT_PRISURFFLUX];
      else 
	/* Convert to system units?! */
	{}
      ncol++;
    }
    
    /* Primary Body's Tidal Heat */
    if (!strcmp(param->cOutputOrder[j],output->cParam[OUT_PRITIDEHEAT])) {
      if (param->iTideModel == CPL)  /* CPL2 */
	col[j+n]=dTideHeat_CPL2(epsilon[0],z[0],sec->dEcc,sec->dMeanMotion,pri->dSpinRate,pri->dObliquity);
      else /* CTL8 */
	col[j+n]=dTideHeat_CTL8(z[0],f,beta,pri->dSpinRate,pri->dObliquity,sec->dMeanMotion);
      
      if (output->iNeg[OUT_PRITIDEHEAT] == 0)
	col[j+n] *= output->dConvert[OUT_PRITIDEHEAT];
      else /* TeraWatts */
	/* Convert to system units?! */
	{}
      ncol++;
    }
    
    /* Primary's Rotational Velocity */
    if (!strcmp(param->cOutputOrder[j],output->cParam[OUT_PRIVROT])) {
      col[j+n] = dRotVel(pri->dRadius,pri->dSpinRate);
      if (output->iNeg[OUT_PRIVROT])
	col[j+n] *= output->dConvert[OUT_PRIVROT];
      else /* km/s */
	col[j+n] *= dTimeUnit(param->iUnitTime,io->exit_param)/dLengthUnit(param->iUnitLength,io->exit_param);
      
      ncol++;
    }
    
    /*
     *   SECONDARY
     */
    
    /* Rotational Angular Momentum */
    if (!strcmp(param->cOutputOrder[j],output->cParam[OUT_SECANGMOM])) {
      col[j+n]=dSpinAngMom(sec->dRG,sec->dMass,sec->dRadius,sec->dSpinRate);
      ncol++;
    }
    
    /* Seondary's Contribution to da/dt */
    if (!strcmp(param->cOutputOrder[j],output->cParam[OUT_SECDADT])) {
      if (param->iTideModel == CPL)  /* CPL2 */
	col[j+n] = dDaDt1_CPL2(sec->dMass,pri->dMass,sec->dSemi,sec->dEcc,sec->dObliquity,epsilon[1],z[1]);
      else /* CTL8 */
	col[j+n] = dDaDt1_CTL8(sec->dMass,pri->dMass,sec->dSemi,sec->dEcc,sec->dMeanMotion,z[1],sec->dSpinRate,sec->dObliquity,f,beta);
      
      if (output->iNeg[OUT_SECDADT])
	col[j+n] *= output->dConvert[OUT_SECDADT];
      else
	col[j+n] *= dTimeUnit(param->iUnitTime,io->exit_units)/dLengthUnit(param->iUnitLength,io->exit_units);
      ncol++;
    }
    
    /* Secondary's Contribution to de/dt */
    if (!strcmp(param->cOutputOrder[j],output->cParam[OUT_SECDEDT])) {
      if (param->iTideModel == CPL)  /* CPL2 */
	col[j+n] = dDeDt1_CPL2(sec->dMass,pri->dMass,sec->dSemi,sec->dEcc,epsilon[1],z[1]);
      else /* CTL8 */
	col[j+n] = dDeDt1_CTL8(sec->dMass,pri->dMass,sec->dSemi,sec->dEcc,sec->dMeanMotion,z[1],sec->dSpinRate,sec->dObliquity,f,beta);
      
      if (output->iNeg[OUT_SECDEDT])
	col[j+n] *= output->dConvert[OUT_SECDEDT];
      else
	col[j+n] *= dTimeUnit(param->iUnitTime,io->exit_units);
      ncol++;
    }
    
    /* Rate of Change of Obliquity in Secondary */
    if (!strcmp(param->cOutputOrder[j],output->cParam[OUT_SECDOBLDT])) {
      col[j+n]=sec->dDobliquityDt;
      if (output->iNeg[OUT_SECDOBLDT])
	col[j+n] *= output->dConvert[OUT_SECDOBLDT];
      else
	col[j+n] *= dTimeUnit(param->iUnitTime,io->exit_units)/dAngleUnit(param->iUnitAngle,io->exit_units);
      ncol++;
    }
    
    /* Rate of Change of Secondary's Rotation Rate */
    if (!strcmp(param->cOutputOrder[j],output->cParam[OUT_SECDSPINDT])) {
      col[j+n]=sec->dDomegaDt;
      ncol++;
    }
    
    /* Rate of Change of Rotation Period of Secondary */
    if (!strcmp(param->cOutputOrder[j],output->cParam[OUT_SECDPERDT])) {
      col[j+n]=dDPerDt(sec->dSpinRate,sec->dDomegaDt);
      
      if (output->iNeg[OUT_SECDPERDT]) 
	col[j+n] *= output->dConvert[OUT_SECDPERDT];
      /* else Unitless */
      ncol++;
    }
    
    /* Secondary's Equilibrium Spin Period */
    if (!strcmp(param->cOutputOrder[j],output->cParam[OUT_SECEQSPINPER])) {

      col[j+n] = dFreqToPer(param->fEqSpin(sec->dMeanMotion,sec->dEcc,sec->dObliquity,param->bDiscreteRot));

      if (output->iNeg[OUT_SECEQSPINPER])
	col[j+n] *= output->dConvert[OUT_SECEQSPINPER];
      else
	col[j+n] *= dTimeUnit(param->iUnitTime,io->exit_units);
      ncol++;
    }
    
    /* Secondary's Equilibrium Spin Rate */
    if (!strcmp(param->cOutputOrder[j],output->cParam[OUT_SECEQSPINRATE])) {

      col[j+n] = param->fEqSpin(sec->dMeanMotion,sec->dEcc,sec->dObliquity,param->bDiscreteRot);

      if (output->iNeg[OUT_SECEQSPINRATE])
	col[j+n] *= output->dConvert[OUT_SECEQSPINRATE];
      else
	col[j+n] *= dTimeUnit(param->iUnitTime,io->exit_units);
      ncol++;
    }
    
    /* Secondary's Equilibrium Tidal Heat Flux */
    if (!strcmp(param->cOutputOrder[j],output->cParam[OUT_SECEQSURFFLUX])) {
      if (param->iTideModel == CPL)  /* CPL2 */
	col[j+n] = dTideHeatEq_CPL2(z[1],sec->dEcc,sec->dObliquity,sec->dMeanMotion,param->bDiscreteRot);
      else /* CTL8 */
	col[j+n] = dTideHeatEq_CTL8(z[1],f,beta,sec->dObliquity,sec->dMeanMotion);
      
      col[j+n] /= 4*PI*sec->dRadius*sec->dRadius;
      
      if (output->iNeg[OUT_SECEQSURFFLUX])
	col[j+n] *= output->dConvert[OUT_SECEQSURFFLUX];
      else
	col[j+n] *= 1;
      ncol++;
    }
    
    /* Secondary's Gamma_Orbital */
    if (!strcmp(param->cOutputOrder[j],output->cParam[OUT_SECGAMMAORB])) {
      col[j+n]=dGammaOrb(sec->dEcc,sec->dObliquity,epsilon[1]);
      ncol++;
    }
    
    /* Secondary's Gamma_Rotation */
    if (!strcmp(param->cOutputOrder[j],output->cParam[OUT_SECGAMMAROT])) {
      col[j+n]=dGammaRot(sec->dEcc,sec->dObliquity,epsilon[1]);
      ncol++;
    }
    
    /* Secondary's Heat */
    if (!strcmp(param->cOutputOrder[j],output->cParam[OUT_SECHEAT])) {
      if (param->iTideModel == CPL)  /* CPL2 */
	col[j+n]=dTideHeat_CPL2(epsilon[1],z[1],sec->dEcc,sec->dMeanMotion,sec->dSpinRate,sec->dObliquity);
      else /* CTL8 */
	col[j+n]=dTideHeat_CTL8(z[1],f,beta,sec->dSpinRate,sec->dObliquity,sec->dMeanMotion);
      
      if (output->iNeg[OUT_SECHEAT])
	col[j+n] *= output->dConvert[OUT_SECHEAT];
      else 
	/* Convert to system units?! */
	{}
      
      ncol++;
    }
    
    /* Secondary's Obliquity */
    if (!strcmp(param->cOutputOrder[j],output->cParam[OUT_SECOBL])) {
      col[j+n]=sec->dObliquity/dAngleUnit(param->iUnitAngle,io->exit_output);
      ncol++;
    }
    
    /* Secondary's Tidal Q */
    if (!strcmp(param->cOutputOrder[j],output->cParam[OUT_SECQ])) {
      col[j+n]=sec->dQ;
      ncol++;
    }
    
    /* Secondary's Rotational Energy */
    if (!strcmp(param->cOutputOrder[j],output->cParam[OUT_SECROTEN])) {
      col[j+n]=dRotEn(sec->dMass,sec->dRadius,sec->dRG,sec->dSpinRate);
      if (!output->iNeg[OUT_SECROTEN]) 
	/* ergs is for negative, so do nothing */
	{}
      else
	/* Convert to System Units!? */
	{}
      ncol++;
    }
    
    /* Secondary's Spin Period */
    if (!strcmp(param->cOutputOrder[j],output->cParam[OUT_SECSPINPER])) {
      col[j+n] = dFreqToPer(sec->dSpinRate);
      if (output->iNeg[OUT_SECSPINPER])
	col[j+n] *= output->dConvert[OUT_SECSPINPER];
      else
	col[j+n] /= dTimeUnit(param->iUnitTime,io->exit_param);
      ncol++;
    }
    
    /* Secondary's Radius */
    if (!strcmp(param->cOutputOrder[j],output->cParam[OUT_SECRAD])) {
      col[j+n] = sec->dRadius;
      if (output->iNeg[OUT_SECRAD])
	col[j+n] *= output->dConvert[OUT_SECRAD];
      else 
	col[j+n] /= dLengthUnit(param->iUnitLength,io->exit_units);
      
      ncol++;
    }
    
    /* Secondary's Rotation Rate */
    if (!strcmp(param->cOutputOrder[j],output->cParam[OUT_SECSPINRATE])) {
      col[j+n]=sec->dSpinRate;
      if (output->iNeg[OUT_SECSPINRATE])
	col[j+n] *= output->dConvert[OUT_SECSPINRATE];
      else
	col[j+n] *= dTimeUnit(param->iUnitTime,io->exit_output);
      ncol++;
    }
    
    /* Secondary's Surface Heat Flux */
    if (!strcmp(param->cOutputOrder[j],output->cParam[OUT_SECSURFFLUX])) {
      if (param->iTideModel == CPL)  /* CPL2 */
	col[j+n]=dTideHeat_CPL2(epsilon[1],z[1],sec->dEcc,sec->dMeanMotion,sec->dSpinRate,sec->dObliquity);
      else /* CTL8 */
	col[j+n]=dTideHeat_CTL8(z[1],f,beta,sec->dSpinRate,sec->dObliquity,sec->dMeanMotion);
      
      col[j+n] /= 4*PI*sec->dRadius*sec->dRadius;
      
      if (output->iNeg[OUT_SECSURFFLUX])
	col[j+n] *= output->dConvert[OUT_SECSURFFLUX];
      else /* W/m^2 ?? */
	/* Convert to system units?! */
	{}
      
      ncol++;
    }
    
    /* Secondary's Tidal Heat */
    if (!strcmp(param->cOutputOrder[j],output->cParam[OUT_SECTIDEHEAT])) {
      if (param->iTideModel == CPL)  /* CPL2 */
	col[j+n]=dTideHeat_CPL2(epsilon[1],z[1],sec->dEcc,sec->dMeanMotion,sec->dSpinRate,sec->dObliquity);
      else /* CTL8 */
	col[j+n]=dTideHeat_CTL8(z[1],f,beta,sec->dSpinRate,sec->dObliquity,sec->dMeanMotion);
      
      if (output->iNeg[OUT_SECTIDEHEAT])
	col[j+n] *= output->dConvert[OUT_SECTIDEHEAT];
      else /* TeraWatts */
	/* Convert to system units?! */
	{}
      ncol++;
    }
    
    /* Secondary's Rotational Velocity */
    if (!strcmp(param->cOutputOrder[j],output->cParam[OUT_SECVROT])) {
      col[j+n] = dRotVel(pri->dRadius,pri->dSpinRate);
      if (output->iNeg[OUT_SECVROT])
	col[j+n] *= output->dConvert[OUT_SECVROT];
      else
	col[j+n] *= dTimeUnit(param->iUnitTime,io->exit_param)/dLengthUnit(param->iUnitLength,io->exit_param);
      
      ncol++;
    }
    
    /* Current Time */
    if (!strcmp(param->cOutputOrder[j],output->cParam[OUT_TIME])) {
      col[j+n]=dTime;
      if (output->iNeg[OUT_TIME])
	col[j+n] *= output->dConvert[OUT_TIME];
      else
	col[j+n] /= dTimeUnit(param->iUnitTime,io->exit_output);
      ncol++;
    }
    
    /* Age */
    if (!strcmp(param->cOutputOrder[j],output->cParam[OUT_AGE])) {
      col[j+n]=sec->dAge;
      if (output->iNeg[OUT_AGE])
	col[j+n] *= output->dConvert[OUT_AGE];
      else 
	col[j+n] /= dTimeUnit(param->iUnitTime,io->exit_output);
      
      ncol++;
    }
    
    /* Timestep */
    if (!strcmp(param->cOutputOrder[j],output->cParam[OUT_DT])) {
      col[j+n]=dDt;
      if (output->iNeg[OUT_DT])
	col[j+n] *= output->dConvert[OUT_DT];
      else
	col[j+n] /= dTimeUnit(param->iUnitTime,io->exit_output);
      ncol++;
    }
    
    /* Total Angular Momentum */
    if (!strcmp(param->cOutputOrder[j],output->cParam[OUT_TOTANGMOM])) {
      col[j+n] =  dOrbAngMom(pri,sec) + dSpinAngMom(pri->dRG,pri->dMass,pri->dRadius,pri->dSpinRate) + dSpinAngMom(sec->dRG,sec->dMass,sec->dRadius,sec->dSpinRate);
      ncol++;
    }
    
    /* Total Energy */
    if (!strcmp(param->cOutputOrder[j],output->cParam[OUT_TOTEN])) {
      col[j+n] = dOrbEn(pri->dMass,sec->dMass,sec->dSemi) + dRotEn(pri->dMass,pri->dRadius,pri->dRG,pri->dSpinRate) + dRotEn(sec->dMass,sec->dRadius,sec->dRG,sec->dSpinRate);
      if (output->iNeg[OUT_TOTEN]) 
	/* ergs is for negative, so do nothing */
	{}
      else
	/* Convert to System Units!? */
	{}
      ncol++;
    }
    
    /*
     * Timescales
     */
    
    /* Semi-Major Axis Timescale */
    if (!strcmp(param->cOutputOrder[j],output->cParam[OUT_TAUSEMI])) {
      if (sec->dDaDt != 0) 
	col[j+n] = fabs(sec->dSemi/sec->dDaDt);
      else
	col[j+n] = HUGE;
      
      if (output->iNeg[OUT_TAUSEMI])
	col[j+n] *= output->dConvert[OUT_TAUSEMI];
      else
	col[j+n] /= dTimeUnit(param->iUnitTime,io->exit_output);
      ncol++;
    }
    
    /* Eccentricity Timescale */
    if (!strcmp(param->cOutputOrder[j],output->cParam[OUT_TAUECC])) {
      if (sec->dDeDt != 0)
	col[j+n] = fabs(sec->dEcc/sec->dDeDt);
      else 
	col[j+n] = HUGE;
      
      if (output->iNeg[OUT_TAUECC])
	col[j+n] *= output->dConvert[OUT_TAUECC];
      else
	col[j+n] /= dTimeUnit(param->iUnitTime,io->exit_output);
      ncol++;
    }
    
    /* Primary Spin Timescale */
    if (!strcmp(param->cOutputOrder[j],output->cParam[OUT_TAUPRISPIN])) {
      if (pri->dDomegaDt != 0)
	col[j+n] = fabs(pri->dSpinRate/pri->dDomegaDt);
      else
	col[j+n] = HUGE;
      
      if (output->iNeg[OUT_TAUPRISPIN])
	col[j+n] *= output->dConvert[OUT_TAUPRISPIN];
      else
	col[j+n] /= dTimeUnit(param->iUnitTime,io->exit_output);
      ncol++;
    }
    
    /* Primary Obliquity Timescale */
    if (!strcmp(param->cOutputOrder[j],output->cParam[OUT_TAUPRIOBL])) {
      if (pri->dDobliquityDt != 0)
	col[j+n] = fabs(pri->dObliquity/pri->dDobliquityDt);
      else
	col[j+n] = HUGE;
      
      if (output->iNeg[OUT_TAUPRIOBL])
	col[j+n] *= output->dConvert[OUT_TAUPRIOBL];
      else
	col[j+n] /= dTimeUnit(param->iUnitTime,io->exit_output);
      ncol++;
    }
    
    /* Secondary Spin Timescale */
    if (!strcmp(param->cOutputOrder[j],output->cParam[OUT_TAUSECSPIN])) {
      if (sec->dDomegaDt != 0)
	col[j+n] = fabs(sec->dSpinRate/sec->dDomegaDt);
      else
	col[j+n] = HUGE;
      
      if (output->iNeg[OUT_TAUSECSPIN])
	col[j+n] *= output->dConvert[OUT_TAUSECSPIN];
      else
	col[j+n] /= dTimeUnit(param->iUnitTime,io->exit_output);
      ncol++;
    }
    
    /* Secondary Obliquity Timescale */
    if (!strcmp(param->cOutputOrder[j],output->cParam[OUT_TAUSECOBL])) {
      if (sec->dDobliquityDt != 0)
	col[j+n] = fabs(sec->dObliquity/sec->dDobliquityDt);
      else
	col[j+n] = HUGE;
      
      if (output->iNeg[OUT_TAUSECOBL])
	col[j+n] *= output->dConvert[OUT_TAUSECOBL];
      else
	col[j+n] /= dTimeUnit(param->iUnitTime,io->exit_output);
      ncol++;
    }
  }	
  
  /* Write out data */
  
  for (j=0;j<ncol;j++) {
    fprintd(fp,col[j],io->iSciNot,io->iDigits);
    fprintf(fp," ");
  }
  fprintf(fp,"\n");
  fflush(fp);

  free(epsilon[0]); 
  free(epsilon[1]); 
  free(epsilon); 
  free(z);
  free(chi);
  free(f);
}

