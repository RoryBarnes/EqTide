#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include "eqtide.h"
#include "output.h"

void InitializeIO(IO *io) {
    io->exit_param = EXIT_PARAM;
    io->exit_units = EXIT_UNITS;
    io->exit_exe = EXIT_EXE;
    io->exit_output = EXIT_OUTPUT;
}

void InitializeOutput(OUTPUT *output) {
    int i;

    for (i=0;i<NUMOUT;i++) {
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
     * Atmospheric Mass Loss
     */

    sprintf(output->cParam[OUT_SECMASSLOSS],"SecMassLoss");
    sprintf(output->cDescr[OUT_SECMASSLOSS],"Secondary's Atmospheric Mass Loss Rate");
    output->iNeg[OUT_SECMASSLOSS] = 0;
    sprintf(output->cNeg[OUT_SECMASSLOSS],"Earth/Gyr");
    output->dConvert[OUT_SECMASSLOSS] = (YEARSEC*1e9)/MEARTH;

    sprintf(output->cParam[OUT_SECMASSH],"SecMassH");
    sprintf(output->cDescr[OUT_SECMASSH],"Secondary's Mass in Hydrogren Available for Escape");
    output->iNeg[OUT_AGE] = 0;
    sprintf(output->cNeg[OUT_AGE],"Earth");
    output->dConvert[OUT_AGE] = 1./MEARTH;

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
     * G
     */

    /*
     *   HABITABLE ZONE
     */

    sprintf(output->cParam[OUT_HZINNERINSOL],"HZInnerInsol");
    sprintf(output->cDescr[OUT_HZINNERINSOL],"Semi-major Axis of Inner Insolation HZ Boundary(ies)");
    output->iNeg[OUT_HZINNERINSOL] = 0;
    sprintf(output->cNeg[OUT_HZINNERINSOL],"AU");
    output->dConvert[OUT_HZINNERINSOL] = 1./AUCM;
    
    sprintf(output->cParam[OUT_HZOUTERINSOL],"HZOuterInsol");
    sprintf(output->cDescr[OUT_HZOUTERINSOL],"Semi-major Axis of Outer Insolation HZ Boundary(ies)");
    output->iNeg[OUT_HZOUTERINSOL] = 0;
    sprintf(output->cNeg[OUT_HZOUTERINSOL],"AU");
    output->dConvert[OUT_HZOUTERINSOL] = 1./AUCM;

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

    sprintf(output->cParam[OUT_PRICONTRDPERDT],"PriContrDPerDt");
    sprintf(output->cDescr[OUT_PRICONTRDPERDT],"Primary Spin Period Change due to Radial Contraction");
    output->iNeg[OUT_PRICONTRDPERDT] = 0;
    sprintf(output->cNeg[OUT_PRICONTRDPERDT],"days/Myr");
    output->dConvert[OUT_PRICONTRDPERDT] = DAYSEC/(YEARSEC*1e6);

    sprintf(output->cParam[OUT_PRICONTRDRADDT],"PriContrDRadDt");
    sprintf(output->cDescr[OUT_PRICONTRDRADDT],"Rate of Primary's Radius Contraction");
    output->iNeg[OUT_PRICONTRDRADDT] = 0;
    sprintf(output->cNeg[OUT_PRICONTRDRADDT],"Solar/Myr");
    output->dConvert[OUT_PRICONTRDRADDT] = YEARSEC*1e6/RSUN;

    sprintf(output->cParam[OUT_PRICONTRDSPINDT],"PriContrDSpinDt");
    sprintf(output->cDescr[OUT_PRICONTRDSPINDT],"Rate of Primary's Radius Spin Rate");

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

    sprintf(output->cParam[OUT_PRILUM],"PriLuminosity");
    sprintf(output->cDescr[OUT_PRILUM],"Primary's Luminosity");
    output->iNeg[OUT_PRILUM] = 0;
    sprintf(output->cNeg[OUT_PRILUM],"Solar");
    output->dConvert[OUT_PRILUM] = 1./LSUN;

    sprintf(output->cParam[OUT_PRIRAD],"PriRadius");
    sprintf(output->cDescr[OUT_PRIRAD],"Primary's Radius");
    output->iNeg[OUT_PRIRAD] = 0;
    sprintf(output->cNeg[OUT_PRIRAD],"Solar");
    output->dConvert[OUT_PRIRAD] = 1./RSUN;

    sprintf(output->cParam[OUT_PRIROTEN],"PriRotEnergy");
    sprintf(output->cDescr[OUT_PRIROTEN],"Primary's Rotational Energy");
    output->iNeg[OUT_PRIROTEN] = 0;
    sprintf(output->cNeg[OUT_PRIROTEN],"ergs");

    sprintf(output->cParam[OUT_PRISPINPER],"PriSpinPer");
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

    sprintf(output->cParam[OUT_PRITEFF],"PriTeff");
    sprintf(output->cDescr[OUT_PRITEFF],"Primary's Effective Temperature");
    output->iNeg[OUT_PRITEFF] = 0;
    sprintf(output->cNeg[OUT_PRITEFF],"K");
    output->dConvert[OUT_PRITEFF] = 1;

    sprintf(output->cParam[OUT_PRITIDEDPERDT],"PriTideDPerDt");
    sprintf(output->cDescr[OUT_PRITIDEDPERDT],"Time Rate of Change of Primary's Spin Period due to Tides");
    output->iNeg[OUT_PRITIDEDPERDT] = 0;
    sprintf(output->cNeg[OUT_PRITIDEDPERDT],"days/Myr");
    output->dConvert[OUT_PRITIDEDPERDT] = DAYSEC/(1e6*YEARSEC);

    sprintf(output->cParam[OUT_PRITIDEDSPINDT],"PriTideDSpinDt");
    sprintf(output->cDescr[OUT_PRITIDEDSPINDT],"Time Rate of Change of Primary's Rotational Frequency due to Tides");
   
    sprintf(output->cParam[OUT_PRITIDEHEAT],"PriTideHeat");
    sprintf(output->cDescr[OUT_PRITIDEHEAT],"Primary's Tidal Heat");
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
    
    sprintf(output->cParam[OUT_SECCONTRDPERDT],"SecContrDPerDt");
    sprintf(output->cDescr[OUT_SECCONTRDPERDT],"Secondary Spin Period Change due to Radial Contraction");
    output->iNeg[OUT_SECCONTRDPERDT] = 0;
    sprintf(output->cNeg[OUT_SECCONTRDPERDT],"days/Myr");
    output->dConvert[OUT_SECCONTRDPERDT] = YEARSEC*1e6/DAYSEC;

    sprintf(output->cParam[OUT_SECCONTRDRADDT],"SecContrDRadDt");
    sprintf(output->cDescr[OUT_SECCONTRDRADDT],"Rate of Secondary's Radius Contraction");
    output->iNeg[OUT_SECCONTRDRADDT] = 0;
    sprintf(output->cNeg[OUT_SECCONTRDRADDT],"Solar/Myr");
    output->dConvert[OUT_SECCONTRDRADDT] = YEARSEC*1e6/RSUN;

    sprintf(output->cParam[OUT_SECCONTRDSPINDT],"SecContrDSpinDt");
    sprintf(output->cDescr[OUT_SECCONTRDSPINDT],"Rate of Secondary's Radius Spin Rate");

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

    sprintf(output->cParam[OUT_SECHEAT],"SecHeat");
    sprintf(output->cDescr[OUT_SECHEAT],"Secondary's Internal Heat");
    output->iNeg[OUT_SECHEAT] = 0;
    sprintf(output->cNeg[OUT_SECHEAT],"TW");
    output->dConvert[OUT_SECHEAT] = 1e-19;
    
    sprintf(output->cParam[OUT_SECINSOLATION],"SecInsol");
    sprintf(output->cDescr[OUT_SECINSOLATION],"Secondary's Insolation");
    output->iNeg[OUT_SECINSOLATION] = 0;
    sprintf(output->cNeg[OUT_SECINSOLATION],"W/m^2");
    output->dConvert[OUT_SECINSOLATION] = 1e-3;
    
    sprintf(output->cParam[OUT_SECOBL],"SecObliquity");
    sprintf(output->cDescr[OUT_SECOBL],"Secondary's Obliquity");
    
    sprintf(output->cParam[OUT_SECQ],"SecQ");
    sprintf(output->cDescr[OUT_SECQ],"Secondary's Tidal Q");
    output->iNeg[OUT_SECQ] = 0;
    sprintf(output->cNeg[OUT_SECQ],"none");
    output->dConvert[OUT_SECQ] = 1;
    
    sprintf(output->cParam[OUT_SECPEAKSUBNAKEDIRR],"SecPeakSubNakedIrr");
    sprintf(output->cDescr[OUT_SECPEAKSUBNAKEDIRR],"Secondary's Substellar Peak Wavelength (Naked, Irradiation)");
    output->iNeg[OUT_SECPEAKSUBNAKEDIRR] = 0;
    sprintf(output->cNeg[OUT_SECPEAKSUBNAKEDIRR],"nm");
    output->dConvert[OUT_SECPEAKSUBNAKEDIRR] = 1e9;

    sprintf(output->cParam[OUT_SECPEAKSUBNAKEDALL],"SecPeakSubNakedAll");
    sprintf(output->cDescr[OUT_SECPEAKSUBNAKEDALL],"Secondary's Substellar Peak Wavelength (Naked, All Sources)");
    output->iNeg[OUT_SECPEAKSUBNAKEDALL] = 0;
    sprintf(output->cNeg[OUT_SECPEAKSUBNAKEDALL],"nm");
    output->dConvert[OUT_SECPEAKSUBNAKEDALL] = 1e9;

    sprintf(output->cParam[OUT_SECPEAKSURFGRIRR],"SecPeakSurfGrnhsIrr");
    sprintf(output->cDescr[OUT_SECPEAKSURFGRIRR],"Secondary's Surface Peak Wavelength (Greenhouse, Irradiation)");
    output->iNeg[OUT_SECPEAKSURFGRIRR] = 0;
    sprintf(output->cNeg[OUT_SECPEAKSURFGRIRR],"nm");
    output->dConvert[OUT_SECPEAKSURFGRIRR] = 1e9;

    sprintf(output->cParam[OUT_SECPEAKSURFGRTIDE],"SecPeakSurfGrnhsTide");
    sprintf(output->cDescr[OUT_SECPEAKSURFGRTIDE],"Secondary's Surface Peak Wavelength (Greenhouse, Tides)");
    output->iNeg[OUT_SECPEAKSURFGRTIDE] = 0;
    sprintf(output->cNeg[OUT_SECPEAKSURFGRTIDE],"nm");
    output->dConvert[OUT_SECPEAKSURFGRTIDE] = 1e9;

    sprintf(output->cParam[OUT_SECPEAKSURFGRALL],"SecPeakSurfGrnhsAll");
    sprintf(output->cDescr[OUT_SECPEAKSURFGRALL],"Secondary's Surface Peak Wavelength (Naked, All)");
    output->iNeg[OUT_SECPEAKSURFGRALL] = 0;
    sprintf(output->cNeg[OUT_SECPEAKSURFGRALL],"nm");
    output->dConvert[OUT_SECPEAKSURFGRALL] = 1e9;

    sprintf(output->cParam[OUT_SECRAD],"SecRadius");
    sprintf(output->cDescr[OUT_SECRAD],"Secondary's Radius");
    output->iNeg[OUT_SECRAD] = 0;
    sprintf(output->cNeg[OUT_SECRAD],"Earth");
    output->dConvert[OUT_SECRAD] = 1./REARTH;

    sprintf(output->cParam[OUT_SECROTEN],"SecRotEnergy");
    sprintf(output->cDescr[OUT_SECROTEN],"Secondary's Rotational Energy");
    output->iNeg[OUT_SECROTEN] = 0;
    sprintf(output->cNeg[OUT_SECROTEN],"ergs");
    
    sprintf(output->cParam[OUT_SECSPINPER],"SecSpinPer");
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
    
    sprintf(output->cParam[OUT_SECTAU],"SecTau");
    sprintf(output->cDescr[OUT_SECTAU],"Secondary's Tidal Time Lag");
    output->iNeg[OUT_SECTAU] = 0;
    sprintf(output->cNeg[OUT_SECTAU],"sec");
    output->dConvert[OUT_SECTAU] = 1;
    
    sprintf(output->cParam[OUT_SECTEMPSUBNAKEDIRR],"SecTempSubNakedIrr");
    sprintf(output->cDescr[OUT_SECTEMPSUBNAKEDIRR],"Secondary's Substellar Temperature (Naked, Irradiation)");
    output->iNeg[OUT_SECTEMPSUBNAKEDIRR] = 0;

    sprintf(output->cParam[OUT_SECTEMPSUBNAKEDALL],"SecTempSubNakedAll");
    sprintf(output->cDescr[OUT_SECTEMPSUBNAKEDALL],"Secondary's Substellar Temperature (Naked, All Sources)");
    output->iNeg[OUT_SECTEMPSUBNAKEDALL] = 0;

    sprintf(output->cParam[OUT_SECTEMPSURFGRIRR],"SecTempSurfGrnhsIrr");
    sprintf(output->cDescr[OUT_SECTEMPSURFGRIRR],"Secondary's Surface Temperature (Greenhouse, Irradiation)");
    output->iNeg[OUT_SECPEAKSURFGRIRR] = 0;

    sprintf(output->cParam[OUT_SECTEMPSURFGRTIDE],"SecTempSurfGrnhsTide");
    sprintf(output->cDescr[OUT_SECTEMPSURFGRTIDE],"Secondary's Surface Temperature (Greenhouse, Tides)");
    output->iNeg[OUT_SECTEMPSURFGRTIDE] = 0;

    sprintf(output->cParam[OUT_SECTEMPSURFGRALL],"SecTempSurfGrnhsAll");
    sprintf(output->cDescr[OUT_SECTEMPSURFGRALL],"Secondary's Surface Temperature (Naked, All)");
    output->iNeg[OUT_SECPEAKSURFGRALL] = 0;

    sprintf(output->cParam[OUT_SECTIDEDPERDT],"SecTideDPerDt");
    sprintf(output->cDescr[OUT_SECTIDEDPERDT],"Time Rate of Change of Secondary's Spin Period due to Tides");
    output->iNeg[OUT_SECTIDEDPERDT] = 0;
    sprintf(output->cNeg[OUT_SECTIDEDPERDT],"days/Myr");
    output->dConvert[OUT_SECTIDEDPERDT] = DAYSEC/(1e6*YEARSEC);

    sprintf(output->cParam[OUT_SECTIDEDSPINDT],"SecTideDSpinDt");
    sprintf(output->cDescr[OUT_SECTIDEDSPINDT],"Time Rate of Change of Secondary's Rotational Frequency due to Tides");
   
    sprintf(output->cParam[OUT_SECTIDEHEAT],"SecTideHeat");
    sprintf(output->cDescr[OUT_SECTIDEHEAT],"Secondary's Tidal Heat");
    output->iNeg[OUT_SECTIDEHEAT] = 0;
    sprintf(output->cNeg[OUT_SECTIDEHEAT],"TW");
    output->dConvert[OUT_SECTIDEHEAT] = 1e-19;
    
    sprintf(output->cParam[OUT_SECTIDELOCK],"SecTideLock");
    sprintf(output->cDescr[OUT_SECTIDELOCK],"Secondary Tidally Locked?");
    output->iNeg[OUT_SECTIDELOCK] = 0;
    sprintf(output->cNeg[OUT_SECTIDELOCK],"0,1");
    
    sprintf(output->cParam[OUT_SECTIDESURFFLUX],"SecTideSurfFlux");
    sprintf(output->cDescr[OUT_SECTIDESURFFLUX],"Secondary's Tidal Heat Flux");
    output->iNeg[OUT_SECTIDESURFFLUX] = 0;
    sprintf(output->cNeg[OUT_SECTIDESURFFLUX],"W/m^2");
    output->dConvert[OUT_SECTIDESURFFLUX] = 1e-3;
    
    sprintf(output->cParam[OUT_SECVROT],"SecRotVel");
    sprintf(output->cDescr[OUT_SECVROT],"Secondary's Rotational Velocity");
    output->iNeg[OUT_SECVROT] = 0;
    sprintf(output->cNeg[OUT_SECVROT],"km/s");
    output->dConvert[OUT_SECVROT] = 1e-5;
    
    sprintf(output->cParam[OUT_SECWET],"SecWet");
    sprintf(output->cDescr[OUT_SECWET],"Secondary Water Status");
    output->iNeg[OUT_SECWET] = 0;
    sprintf(output->cNeg[OUT_SECWET],"0,1");
    
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







void WriteLog(PARAM param,PRIMARY pri,SECONDARY sec,HZ hz,OUTPUT output,FILES files,IO io,int iEnd) {
  int i,j,k,ok,**epsilon;
  FILE *fp;
  double tmp;
  char cTime[16],cTmp[24];
  double z[2],zprime[2],chi[2],f[5],dDomegaDt[2],dDoblDt[2];
  double dDaDt,dDeDt,dTideHeat[2],dTideSurfFlux[2],ttv,dEqSpinRate[2],dBeta;
  double dEqHeatFlux[2],dIrradiation;

  epsilon=malloc(2*sizeof(int*));
  epsilon[0]=malloc(9*sizeof(int));
  epsilon[1]=malloc(9*sizeof(int));


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
    if (param.iTideModel == 0) {
      fprintf(fp,"Constant-Phase-Lag, 2nd order\n");
    } else if (param.iTideModel == 1) {
      fprintf(fp,"Constant-Phase-Lag, 4th order\n");
    } else if (param.iTideModel == 2) {
      fprintf(fp,"Constant-Phase-Lag, 6th order\n");
    } else if (param.iTideModel == 3) {
      fprintf(fp,"Constant-Phase-Lag, 8th order\n");
    } else if (param.iTideModel == 4) {
      fprintf(fp,"Constant-Phase-Lag, 10th order\n");
    } else if (param.iTideModel == 5) {
      fprintf(fp,"Constant-Phase-Lag, 12th order\n");
    } else if (param.iTideModel == 6) {
      fprintf(fp,"Constant-Time-Lag, 2nd order\n");
    } else if (param.iTideModel == 7) {
      fprintf(fp,"Constant-Time-Lag, 4th order\n");
    } else if (param.iTideModel == 8) {
      fprintf(fp,"Constant-Time-Lag, 6th order\n");
    } else if (param.iTideModel == 9) {
      fprintf(fp,"Constant-Time-Lag, 8th order\n");
    } else if (param.iTideModel == 10) {
      fprintf(fp,"Constant-Time-Lag, 10th order\n");
    } else if (param.iTideModel == 11) {
      fprintf(fp,"Constant-Time-Lag, 12th order\n");
    }
 
    if (param.iTideModel == 0) 
	fprintf(fp,"Use Discrete Rotation Rate Model: %d\n",param.bDiscreteRot);
   
    /* Verbosity */
    
    fprintf(fp,"Verbosity Level: %d\n",param.iVerbose);
    
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
    
    /* Temperature Units */
    
    fprintf(fp,"Temperature Units: ");
    if (param.iUnitTemp == 0) {
      fprintf(fp,"Kelvin\n");
    } else if (param.iUnitTemp == 1) {
      fprintf(fp,"Celsius\n");
    } else if (param.iUnitTemp == 2) {
      fprintf(fp,"Farenheit\n");
    }
    

    fprintf(fp,"Minimum Value of ecc and obl: ");
    fprintd(fp,param.dMinValue,param.iSciNot,param.iDigits);
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
    
    if (param.bDoForward) {
      fprintf(fp,"\n--------- Forward Integration ---------\n\n");
      fprintf(fp,"%s Time Step: ",cTime);
      fprintd(fp,param.dForwTimeStep/dTimeUnit(param.iUnitTime,io.exit_units),param.iSciNot,param.iDigits);
      fprintf(fp,"\n");

      fprintf(fp,"Stop Time: ");
      fprintd(fp,param.dForwStopTime/dTimeUnit(param.iUnitTime,io.exit_units),param.iSciNot,param.iDigits);
      fprintf(fp,"\n");

      fprintf(fp,"Output Interval: ");
      fprintd(fp,param.dForwOutputTime/dTimeUnit(param.iUnitTime,io.exit_units),param.iSciNot,param.iDigits);
      fprintf(fp,"\n");

      fprintf(fp,"%s Minimum Semi-major Axis: ",cTime);
      fprintd(fp,param.halt.dMinSemi/dLengthUnit(param.iUnitLength,io.exit_units),param.iSciNot,param.iDigits);
      fprintf(fp,"\n");

      fprintf(fp,"Use Variable Timestep: ");
      if (param.bForwVarDt == 0) {
	fprintf(fp,"No\n");
      } else {
	fprintf(fp,"Yes\n");
	fprintf(fp,"dEta: ");
	fprintd(fp,param.dForwEta,param.iSciNot,param.iDigits);
	fprintf(fp,"\n");
      }
    }   
     
    if (param.bDoBackward) {
      fprintf(fp,"\n--------- Backward Integration ---------\n\n");
      fprintf(fp,"%s Time Step: ",cTime);
      fprintd(fp,param.dBackTimeStep/dTimeUnit(param.iUnitTime,io.exit_units),param.iSciNot,param.iDigits);
      fprintf(fp,"\n");

      fprintf(fp,"Stop Time: ");
      fprintd(fp,param.dBackStopTime/dTimeUnit(param.iUnitTime,io.exit_units),param.iSciNot,param.iDigits);
      fprintf(fp,"\n");

      fprintf(fp,"Output Time: ");
      fprintd(fp,param.dBackOutputTime/dTimeUnit(param.iUnitTime,io.exit_units),param.iSciNot,param.iDigits);
      fprintf(fp,"\n");

      fprintf(fp,"Use Variable Timestep: ");
      if (param.bBackVarDt == 0) {
	fprintf(fp,"No\n");
      } else {
	fprintf(fp,"Yes\n");
	fprintf(fp,"dEta: ");
	fprintd(fp,param.dBackEta,param.iSciNot,param.iDigits);
	fprintf(fp,"\n");
      }
    }

      /* This should be moved into a subroutine */

    if (param.bDoBackward || param.bDoForward) {
      fprintf(fp,"Output Order: ");
      for (i=0;i<param.iNumCols;i++) {
        ok=0;
        for (j=0;j<NUMOUT;j++) {
          for (k=0;k<24;k++)
            cTmp[k]=output.cParam[j][k];
          /* Note IHZ boundaries can be multiple!! */
          
          if (memcmp(lower(param.cOutputOrder[i]),"iinnerihz",9) == 0) {
            for (j=0;j>hz.iNumIHZ;j++) {
              fprintf(fp,"%s[%d] ",param.cOutputOrder[j],j);
            }
            ok=1;
          } else if (memcmp(lower(param.cOutputOrder[i]),"iouterihz",9) == 0) {
            for (j=0;j>hz.iNumIHZ;j++) {
              fprintf(fp,"%s[%d] ",param.cOutputOrder[j],j);
            }    
            ok=1;
          } else if (strcmp(lower(param.cOutputOrder[i]),lower(cTmp)) == 0) {
	    
            /* Normal column */
	    
            fprintf(fp,"%s",output.cParam[j]);
	    if (output.iNeg[j])
		fprintf(fp,"[%s]",output.cNeg[j]);
	    fprintf(fp," ");
            ok=1;
          }
        }
        if (!ok) {
	  fprintf(stderr,"ERROR: Unknown Parameter in cOutputOrder: param.cOutputOrder[%d] = %s.\n",j,param.cOutputOrder[j]);
	  exit(io.exit_param);
        }
      }
      fprintf(fp,"\n");
    }
  } /* iEnd */

  /* 

   * Properties of Primary Body 
   */

  fprintf(fp,"\n--------- Primary Body ---------\n\n");
  if (!iEnd) {
    fprintf(fp,"White Dwarf: %d\n",pri.bWD);
      fprintf(fp,"Mass: ");
      fprintd(fp,pri.dMass/dMassUnit(param.iUnitMass,io.exit_units),param.iSciNot,param.iDigits);
      fprintf(fp,"\n");

      fprintf(fp,"Radius: ");
      fprintd(fp,pri.dRadius/dLengthUnit(param.iUnitLength,io.exit_units),param.iSciNot,param.iDigits);
      fprintf(fp,"\n");

      if (param.iTideModel < 6) {
	  fprintf(fp,"Tidal Q: ");
	  fprintd(fp,pri.dQ,param.iSciNot,param.iDigits);
	  fprintf(fp,"\n");
      if (pri.bDoQPrime) {
	fprintf(fp,"Use Q': Yes\n");
      } else {
	  fprintf(fp,"Love Number k2: ");
	  fprintd(fp,pri.dK2,param.iSciNot,param.iDigits);
      }
    }
    if (param.iTideModel > 5 && param.iTideModel < 12) {
	fprintf(fp,"Time Lag: ");
	fprintd(fp,pri.dTau/dTimeUnit(param.iUnitTime,io.exit_units),param.iSciNot,param.iDigits);
    }
    fprintf(fp,"\n");
  } /* iEnd */

  fprintf(fp,"%s Obliquity: ",cTime);
  fprintd(fp,pri.dObliquity/dAngleUnit(param.iUnitAngle,io.exit_units),param.iSciNot,param.iDigits);
  fprintf(fp,"\n");

  fprintf(fp,"%s Spin Period: ",cTime);
  fprintd(fp,dFreqToPer(pri.dSpinRate)/dTimeUnit(param.iUnitTime,io.exit_units),param.iSciNot,param.iDigits);
  fprintf(fp,"\n");

  fprintf(fp,"%s Spin Frequency: ",cTime);
  fprintd(fp,pri.dSpinRate*dTimeUnit(param.iUnitTime,io.exit_units),param.iSciNot,param.iDigits);
  fprintf(fp,"\n");

  if (!iEnd) {
      fprintf(fp,"Radius of Gyration: ");
      fprintd(fp,pri.dRG,param.iSciNot,param.iDigits);
      fprintf(fp,"\n");

      if (pri.dLuminosity > 0) {
	fprintf(fp,"Luminosity: ");
	fprintd(fp,pri.dLuminosity/LSUN,param.iSciNot,param.iDigits);
	fprintf(fp," [solar]\n");

	pri.dTeff = dTeff(pri.dLuminosity,pri.dRadius);
	fprintf(fp,"Effective Temperature: ");
	fprintd(fp,dTempUnit(pri.dTeff,0,param.iUnitTemp,io.exit_units),param.iSciNot,param.iDigits);
	fprintf(fp,"\n");

	if (pri.iMassLum > 0) {
	    fprintf(fp,"Mass-Luminosity Relationship: ");
	    if (pri.iMassLum == 2) {
		fprintf(fp,"New Light on Dark Stars\n");
	    } else if (pri.iMassLum == 1) {
		fprintf(fp,"Scalo, J. 2007, Astrobiology, 7, 85-166.\n");
	    }
        }
	
      }

      if (pri.iMassRad > 0) {
	fprintf(fp,"Mass-Radius Relationship: ");
	if (pri.iMassRad == 1) {
	  fprintf(fp,"New Light On Dark Stars\n");
	} else if (pri.iMassRad == 2) {
	  fprintf(fp,"Gorda, S.Y. & Svechnikov, M.A. 1999, Astronomy Reports, 43, 521-525.\n");
	} else if (pri.iMassRad == 3) {
	  fprintf(fp,"Bayless, A.J. & Orosz, J.A. 2006, ApJ, 651, 1155-1165.\n");
	} else if (pri.iMassRad == 4) {
	  fprintf(fp,"Sotin, C. et al. 2007, Icarus, 191, 337-351.\n");
	}
      }
      if (pri.iRadContr) {
	fprintf(fp,"Radius Contraction Model: ");
	if (pri.iRadContr == 1) {
	  fprintf(fp,"Baraffe et al. 1998\n");
	} else if (pri.iRadContr == 2) {
	  fprintf(fp,"D'Antona & Mazitelli 1997\n");
	}
      }
      if (pri.dLumX != 0) {
	fprintf(fp,"Fraction of Luminosity in X-Ray/XUV: ");
	fprintd(fp,pri.dLumX/pri.dLuminosity,param.iSciNot,param.iDigits);
	fprintf(fp,"\n");

	fprintf(fp,"X-Ray/XUV Luminosity: ");
	fprintd(fp,pri.dLumX/LSUN,param.iSciNot,param.iDigits);
	fprintf(fp," [solar]\n");
      }


  } /* iEnd */

  /* 
   * Properties of Secondary
   */

  fprintf(fp,"\n--------- Secondary Body ---------\n\n");
  if (!iEnd) {
    fprintf(fp,"Mass: ");
    fprintd(fp,sec.dMass/dMassUnit(param.iUnitMass,io.exit_units),param.iSciNot,param.iDigits);
    fprintf(fp,"\n");
    
    fprintf(fp,"Radius: ");
    fprintd(fp,sec.dRadius/dLengthUnit(param.iUnitLength,io.exit_units),param.iSciNot,param.iDigits);
    fprintf(fp,"\n");
    
    if (param.iTideModel < 6) {
      fprintf(fp,"Tidal Q: ");
      fprintd(fp,sec.dQ,param.iSciNot,param.iDigits);
      fprintf(fp,"\n");

      fprintf(fp,"Wet Q: ");
      fprintd(fp,sec.dWetQ,param.iSciNot,param.iDigits);
      fprintf(fp,"\n");
      
      fprintf(fp,"Dry Q: ");
      fprintd(fp,sec.dDryQ,param.iSciNot,param.iDigits);
      fprintf(fp,"\n");

      if (sec.bDoQPrime) {
	fprintf(fp,"Use Q': Yes\n");
      } else {
	fprintf(fp,"Love Number k2: ");
	fprintd(fp,sec.dK2,param.iSciNot,param.iDigits);
	fprintf(fp,"\n");
      }
    }
    if (param.iTideModel > 5 && param.iTideModel < 12) {
      fprintf(fp,"Time Lag: ");
      fprintd(fp,sec.dTau/dTimeUnit(param.iUnitTime,io.exit_units),param.iSciNot,param.iDigits);
      fprintf(fp,"\n");

      fprintf(fp,"Wet Time Lag: ");
      fprintd(fp,sec.dWetTau/dTimeUnit(param.iUnitTime,io.exit_units),param.iSciNot,param.iDigits);
      fprintf(fp,"\n");
      
      fprintf(fp,"Dry Time Lag: ");
      fprintd(fp,sec.dDryTau/dTimeUnit(param.iUnitTime,io.exit_units),param.iSciNot,param.iDigits);
      fprintf(fp,"\n");
    }

    fprintf(fp,"Water Status: %d\n",sec.bWet);

    fprintf(fp,"Radius of Gyration: ");
    fprintd(fp,sec.dRG,param.iSciNot,param.iDigits);
    fprintf(fp,"\n");
    
    fprintf(fp,"Albedo: ");
    fprintd(fp,sec.dAlbedo,param.iSciNot,param.iDigits);
    fprintf(fp,"\n");

    if (sec.iAlbedoModel != -1) {
      fprintf(fp,"Albedo Model: ");
      if (sec.iAlbedoModel == 0) 
	fprintf(fp,"Ramires et al. 2011, in prep.\n");
    }

    if (sec.dLuminosity > 0) {

      /* It's a star */

      fprintf(fp,"Luminosity: ");
      fprintd(fp,sec.dLuminosity/LSUN,param.iSciNot,param.iDigits);
      fprintf(fp," [solar]\n");
      
      fprintf(fp,"Effective Temperature: ");
      fprintd(fp,dTempUnit(sec.dTeff,0,param.iUnitTemp,io.exit_units),param.iSciNot,param.iDigits);
      fprintf(fp,"\n");
      if (sec.iMassLum > 0) {
	fprintf(fp,"Mass-Luminosity Relationship: ");
	if (sec.iMassLum == 0) {
	  fprintf(fp,"Reid & Hawley 2000\n");
	} else if (sec.iMassLum == 1) {
	  fprintf(fp,"Scalo, J. et al. 2007, Astrobiology, 7, 85-166.\n");
	}
      }
    } else {

      /* It's a planet */

      dIrradiation = dInsolation(pri.dLuminosity,sec.dSemi,sec.dEcc)*(1-sec.dAlbedo);

      fprintf(fp,"Substellar Surface Temperature (Naked, Synchronous, Irradiation): ");
      fprintd(fp,dTempUnit(dNakedPlanetTemp(sec.dAlbedo,pri.dRadius,sec.dSemi,pri.dTeff,0),0,param.iUnitTemp,io.exit_units),param.iSciNot,param.iDigits);
      fprintf(fp,"\n");

      fprintf(fp,"Substellar Surface Temperature (Naked, Synchronous, All Sources): ");
      fprintd(fp,dTempUnit(dFluxToTemperature(dTideSurfFlux[1]+dIrradiation),0,param.iUnitTemp,io.exit_units),param.iSciNot,param.iDigits);
      fprintf(fp,"\n");

      fprintf(fp,"Surface Temperature (Greenhouse, Irradiation): ");
      fprintd(fp,dTempUnit(dFluxToTemperature(dIrradiation/4),0,param.iUnitTemp,io.exit_units),param.iSciNot,param.iDigits);
      fprintf(fp,"\n");

      fprintf(fp,"Surface Temperature (Tides): ");
      fprintd(fp,dTempUnit(dFluxToTemperature(dTideSurfFlux[1]),0,param.iUnitTemp,io.exit_units),param.iSciNot,param.iDigits);
      fprintf(fp,"\n");

      fprintf(fp,"Surface Temperature (Greenhouse, All Sources): ");
      fprintd(fp,dTempUnit(dFluxToTemperature(dIrradiation/4+dTideSurfFlux[1]),0,param.iUnitTemp,io.exit_units),param.iSciNot,param.iDigits);
      fprintf(fp,"\n");

      fprintf(fp,"Substellar Peak Wavelength (Naked, Synchronous, Irradiation): ");
      fprintd(fp,dFluxToPeakWavelength(dIrradiation)*dLengthUnit(param.iUnitLength,io.exit_units),param.iSciNot,param.iDigits);
      fprintf(fp,"\n");

      fprintf(fp,"Substellar Peak Wavelength (Naked, Synchronous, All Sources): ");
      fprintd(fp,dFluxToPeakWavelength(dFluxToPeakWavelength(dIrradiation+dTideSurfFlux[1])*dLengthUnit(param.iUnitLength,io.exit_units)),param.iSciNot,param.iDigits);
      fprintf(fp,"\n");

      fprintf(fp,"Surface Peak Wavelength (Greenhouse, Irradiation): ");
      fprintd(fp,dFluxToPeakWavelength(dIrradiation/4)*dLengthUnit(param.iUnitLength,io.exit_units),param.iSciNot,param.iDigits);
      fprintf(fp,"\n");

      fprintf(fp,"Substellar Peak Wavelength (Greenhouse, All Sources): ");
      fprintd(fp,dFluxToPeakWavelength(dFluxToPeakWavelength(dIrradiation/4 + dTideSurfFlux[1])*dLengthUnit(param.iUnitLength,io.exit_units)),param.iSciNot,param.iDigits);
      fprintf(fp,"\n");
      
      fprintf(fp,"Critical Greenhouse Flux: ");
      fprintd(fp,hz.dRnwyGrnhsFlux/1000,param.iSciNot,param.iDigits);
      fprintf(fp," [W/m^2]\n");

      if (hz.iRnwyGrnhsModel != -1) {
	fprintf(fp,"Runaway Greenhouse Model: ");
	if (hz.iRnwyGrnhsModel == 0) 
	  fprintf(fp,"Pierrehumbert, R. 2010\n"); 
	if (hz.iRnwyGrnhsModel == 1) 
	  fprintf(fp,"Kasting & Catling 2012\n");
	if (hz.iRnwyGrnhsModel == 2) 
	  fprintf(fp,"Abe, Y. et al. 2011, AsBio, 11, 443-460.\n");
      }
		
      fprintf(fp,"Tidal Mass Loss Coefficient (Ktide): ");
      fprintd(fp,dKTide(pri.dMass,sec.dMass,sec.dSemi,sec.dRadX),param.iSciNot,param.iDigits);
      fprintf(fp,"\n");

      fprintf(fp,"Total Atmospheric Mass Loss Rate: ");
      fprintd(fp,dAtmMassLossErkaev07(sec,pri.dLumX,pri.dMass)*dTimeUnit(param.iUnitTime,io.exit_units)/dMassUnit(param.iUnitMass,io.exit_units),param.iSciNot,param.iDigits);
      fprintf(fp,"\n");

      fprintf(fp,"Total Atmospheric Hydrogen Loss Rate: ");
      fprintd(fp,4*PI*dAtmMassLossErkaev07(sec,pri.dLumX,pri.dMass)/MASSHYD*dTimeUnit(param.iUnitTime,io.exit_units),param.iSciNot,param.iDigits);
      fprintf(fp,"\n");

      fprintf(fp,"Total Atmospheric Hydrogen Flux: ");
      fprintd(fp,dAtmMassLossErkaev07(sec,pri.dLumX,pri.dMass)/MASSHYD/(sec.dRadius*sec.dRadius),param.iSciNot,param.iDigits);
      fprintf(fp," [cm^-2 s^-1]\n");

      fprintf(fp,"Desiccation Timescale: ");
      fprintd(fp,dDesiccationTimeErkaev07(sec,pri.dLumX,pri.dMass)/dTimeUnit(param.iUnitTime,io.exit_units),param.iSciNot,param.iDigits);
      fprintf(fp,"\n");

      tmp = dDesiccationTimeErkaev07(sec,pri.dLumX,pri.dMass);

      fprintf(fp,"Incident XUV Flux: ");
      fprintd(fp,0.25*dInsolation(pri.dLumX,sec.dSemi,sec.dEcc)/1000,param.iSciNot,param.iDigits);
      fprintf(fp," [W/m^2]\n");

      fprintf(fp,"Absorbed XUV Flux: ");
      fprintd(fp,dInsolation(pri.dLumX,sec.dSemi,sec.dEcc)*sec.dAtmXAbsEff/1000,param.iSciNot,param.iDigits);
      fprintf(fp," [W/m^2]\n");

      fprintf(fp,"Atmospheric X-Ray/XUV Absorption Efficiency: ");
      fprintd(fp,sec.dAtmXAbsEff,param.iSciNot,param.iDigits);
      fprintf(fp,"\n");
     
      fprintf(fp,"Radius in X-Ray/EUV: ");
      fprintd(fp,sec.dRadX/dLengthUnit(param.iUnitLength,io.exit_param),param.iSciNot,param.iDigits);
      fprintf(fp,"\n");
     

      fprintf(fp,"\n");


    }

    if (sec.iMassRad > 0) {
      fprintf(fp,"Mass-Radius Relationship: ");
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
    if (sec.iRadContr) {
      fprintf(fp,"Radius Contraction Model: ");
      if (sec.iRadContr == 1) {
	fprintf(fp,"Baraffe et al. 1998\n");
      } else if (sec.iRadContr == 2) {
	fprintf(fp,"D'Antona & Mazitelli 1997\n");
      }
    }
  }

  fprintf(fp,"%s Obliquity: ",cTime);
  fprintd(fp,sec.dObliquity/dAngleUnit(param.iUnitAngle,io.exit_units),param.iSciNot,param.iDigits);
  fprintf(fp,"\n");
  
  fprintf(fp,"%s Spin Period: ",cTime);
  fprintd(fp,dFreqToPer(sec.dSpinRate)/dTimeUnit(param.iUnitTime,io.exit_units),param.iSciNot,param.iDigits);
  fprintf(fp,"\n");
  
  fprintf(fp,"%s Spin Frequency: ",cTime);
  fprintd(fp,sec.dSpinRate*dTimeUnit(param.iUnitTime,io.exit_units),param.iSciNot,param.iDigits);
  fprintf(fp,"\n");
  
  tmp=2*atan(pri.dRadius/sec.dSemi);
  if (param.iUnitAngle == 1)
    tmp /= dAngleUnit(param.iUnitAngle,io.exit_units);
  fprintf(fp,"%s Angular Size of Primary body: ",cTime);
  fprintd(fp,tmp,param.iSciNot,param.iDigits);
  fprintf(fp,"\n");

  if (pri.dLuminosity > 0) {
    fprintf(fp,"%s Instellation: ",cTime);
    tmp = dInsolation(pri.dLuminosity,sec.dSemi,sec.dEcc)*(1-sec.dAlbedo)/4;
    fprintd(fp,tmp/1000,param.iSciNot,param.iDigits);
    fprintf(fp," [W/m^2]\n");

    tmp=dTeq(pri.dLuminosity,sec.dSemi,sec.dEcc);
    tmp=dTempUnit(tmp,0,param.iUnitTemp,io.exit_units);
    fprintf(fp,"%s Equilibrium Temperature: ",cTime);
    fprintd(fp,tmp,param.iSciNot,param.iDigits);
    fprintf(fp,"\n");

  }

  fprintf(fp,"\n-------------- Orbit ----------\n\n");
  
  fprintf(fp,"%s Semi-major Axis: ",cTime);
  fprintd(fp,sec.dSemi/dLengthUnit(param.iUnitLength,io.exit_units),param.iSciNot,param.iDigits);
  fprintf(fp,"\n");

  fprintf(fp,"%s Orbital Mean Motion: ",cTime);
  fprintd(fp,sec.dMeanMotion*dTimeUnit(param.iUnitTime,io.exit_units),param.iSciNot,param.iDigits);
  fprintf(fp,"\n");

  fprintf(fp,"%s Orbital Period: ",cTime);
  fprintd(fp,dFreqToPer(sec.dMeanMotion)/dTimeUnit(param.iUnitTime,io.exit_units),param.iSciNot,param.iDigits);
  fprintf(fp,"\n");

  fprintf(fp,"%s Eccentricity: ",cTime);
  fprintd(fp,sec.dEcc,param.iSciNot,param.iDigits);
  fprintf(fp,"\n");

       
  /* 
   * Tidal properties 
   */

  fprintf(fp,"\n--------- Tidal Effects ---------\n\n");

  if (param.iTideModel == 0) {
    /* Constant-Phase-Lag, order 2 */
    AssignEpsilon(pri.dSpinRate,sec.dMeanMotion,epsilon[0]);
    AssignEpsilon(sec.dSpinRate,sec.dMeanMotion,epsilon[1]);
    AssignZprime(pri,sec,z);
    AssignChi(pri,sec,chi);

    dDaDt=dDaDt_CPL2(pri,sec,epsilon,z)*dTimeUnit(param.iUnitTime,io.exit_units)/dLengthUnit(param.iUnitLength,io.exit_units);
    dDeDt=dDeDt_CPL2(pri,sec,epsilon,z)*dTimeUnit(param.iUnitTime,io.exit_units);
    dDomegaDt[0] = dDomegaDt_CPL2(pri.dMass,pri.dRadius,sec.dMeanMotion,sec.dEcc,pri.dRG,pri.dObliquity,epsilon[0],z[0]);
    dDomegaDt[1] = dDomegaDt_CPL2(sec.dMass,sec.dRadius,sec.dMeanMotion,sec.dEcc,sec.dRG,sec.dObliquity,epsilon[1],z[1]);
    dDoblDt[0] = dDoblDt_CPL2(pri.dMass,pri.dRadius,pri.dSpinRate,sec.dMeanMotion,pri.dRG,epsilon[0],chi[0],pri.dObliquity,z[0]);
    dDoblDt[1] = dDoblDt_CPL2(sec.dMass,sec.dRadius,sec.dSpinRate,sec.dMeanMotion,sec.dRG,epsilon[1],chi[1],sec.dObliquity,z[1]);

    /* in Watts */
    dTideHeat[0] = dTideHeat_CPL2(epsilon[0],z[0],sec.dEcc,sec.dMeanMotion,pri.dSpinRate,pri.dObliquity)/1e7;
    dTideHeat[1] = dTideHeat_CPL2(epsilon[1],z[1],sec.dEcc,sec.dMeanMotion,sec.dSpinRate,sec.dObliquity)/1e7;

    /* in W/m^2 */
    dTideSurfFlux[0] = dTideHeat[0]/(4*PI*pri.dRadius*pri.dRadius)*1e4;
    dTideSurfFlux[1] = dTideHeat[1]/(4*PI*sec.dRadius*sec.dRadius)*1e4;

    /* ttv = dDpDt(dDaDt,pri.dMass+sec.dMass,sec.dSemi); */
    dEqSpinRate[0] = EqSpinRate_CPL2(sec.dMeanMotion,sec.dEcc,param.bDiscreteRot);
    dEqSpinRate[1] = EqSpinRate_CPL2(sec.dMeanMotion,sec.dEcc,param.bDiscreteRot);

    /* in W/m^2 */
    dEqHeatFlux[0] = dTideHeatEq_CPL2(z[0],sec.dEcc,pri.dObliquity,sec.dMeanMotion,param.bDiscreteRot)/(4*PI*pri.dRadius*pri.dRadius)/1e3;
    dEqHeatFlux[1] = dTideHeatEq_CPL2(z[1],sec.dEcc,sec.dObliquity,sec.dMeanMotion,param.bDiscreteRot)/(4*PI*sec.dRadius*sec.dRadius)/1e3;

  } else if (param.iTideModel == 9) {

    AssignChi(pri,sec,chi);
    AssignZ(pri,sec,z);
    dBeta = pow(1-sec.dEcc*sec.dEcc,0.5);
    f[0] = AssignF1(sec.dEcc);
    f[1] = AssignF2(sec.dEcc);
    f[2] = AssignF3(sec.dEcc);
    f[3] = AssignF4(sec.dEcc);
    f[4] = AssignF5(sec.dEcc);

    /* Calculate tidal properties */

    
    dDaDt = dDaDt_CTL8(pri,sec,z,dBeta,f);
    dDeDt = dDeDt_CTL8(pri,sec,z,dBeta,f);
    dDomegaDt[0] = dDomegaDt_CTL8(pri.dMass,pri.dRadius,sec.dMeanMotion,sec.dEcc,pri.dRG,pri.dSpinRate,pri.dObliquity,z[0],dBeta,f);
    dDomegaDt[1] = dDomegaDt_CTL8(sec.dMass,sec.dRadius,sec.dMeanMotion,sec.dEcc,sec.dRG,sec.dSpinRate,sec.dObliquity,z[0],dBeta,f);
    dDoblDt[0] = dDoblDt_CTL8(pri.dMass,pri.dRadius,sec.dMeanMotion,sec.dEcc,pri.dRG,pri.dSpinRate,pri.dObliquity,z[0],dBeta,f,chi[0]);
    dDoblDt[1] = dDoblDt_CTL8(sec.dMass,sec.dRadius,sec.dMeanMotion,sec.dEcc,sec.dRG,sec.dSpinRate,sec.dObliquity,z[1],dBeta,f,chi[0]);

    /* in Watts */
    dTideHeat[0]=dTideHeat_CTL8(z[0],f,dBeta,pri.dSpinRate,pri.dObliquity,sec.dMeanMotion)/1e7;
    dTideHeat[1]=dTideHeat_CTL8(z[1],f,dBeta,sec.dSpinRate,sec.dObliquity,sec.dMeanMotion)/1e7;

    /* in W/m^2 */
    dTideSurfFlux[0]=dTideHeat[0]/(4*PI*pri.dRadius*pri.dRadius)*1e4;
    dTideSurfFlux[1]=dTideHeat[1]/(4*PI*sec.dRadius*sec.dRadius)*1e4;

    /* ttv = dDpDt(dDaDt,pri.dMass+sec.dMass,sec.dSemi); */
    dEqSpinRate[0] = EqSpinRate_CTL8(sec.dMeanMotion,sec.dEcc,pri.dObliquity);
    dEqSpinRate[1] = EqSpinRate_CTL8(sec.dMeanMotion,sec.dEcc,sec.dObliquity);

    dEqHeatFlux[0] = dTideHeatEq_CTL8(z[0],f,dBeta,pri.dObliquity,sec.dMeanMotion)/(4*PI*pri.dRadius*pri.dRadius)/1e3;
    dEqHeatFlux[1] = dTideHeatEq_CTL8(z[1],f,dBeta,sec.dObliquity,sec.dMeanMotion)/(4*PI*sec.dRadius*sec.dRadius)/1e3;
  }

  fprintf(fp,"da/dt: ");
  fprintd(fp,dDaDt*dTimeUnit(param.iUnitTime,io.exit_units)/dLengthUnit(param.iUnitLength,io.exit_units),param.iSciNot,param.iDigits);
  fprintf(fp,"\n");
  fprintf(fp,"de/dt: ");
  fprintd(fp,dDeDt*dTimeUnit(param.iUnitTime,io.exit_units),param.iSciNot,param.iDigits);
  fprintf(fp,"\n");
  fprintf(fp,"dP/dt: ");
  fprintd(fp,ttv,param.iSciNot,param.iDigits);
  fprintf(fp,"\n");

  fprintf(fp,"\n");

  fprintf(fp,"Primary dSpinRate/dt: ");
  fprintd(fp,dDomegaDt[0],param.iSciNot,param.iDigits);
  fprintf(fp,"\n");
  fprintf(fp,"Primary dObl/dt: ");
  fprintd(fp,dDoblDt[0]*param.iUnitAngle/param.iUnitTime,param.iSciNot,param.iDigits);
  fprintf(fp,"\n");
  fprintf(fp,"Primary Tidal Heat: ");
  fprintd(fp,dTideHeat[0],param.iSciNot,param.iDigits);
  fprintf(fp," [W]\n");
  fprintf(fp,"Primary Tidal Surface Flux: ");
  fprintd(fp,dTideSurfFlux[0],param.iSciNot,param.iDigits);
  fprintf(fp," [W/m^2]\n");
  fprintf(fp,"Primary's Equilibrium Rotation Rate: ");
  fprintd(fp,dEqSpinRate[0]*dTimeUnit(param.iUnitTime,io.exit_units),param.iSciNot,param.iDigits);
  fprintf(fp,"\n");
  fprintf(fp,"Primary's Equilibrium Rotation Period: ");
  fprintd(fp,dFreqToPer(dEqSpinRate[0])/dTimeUnit(param.iUnitTime,io.exit_units),param.iSciNot,param.iDigits);
  fprintf(fp,"\n");
  fprintf(fp,"Primary's Equilibrium Tidal Surface Flux: ");
  fprintd(fp,dEqHeatFlux[0],param.iSciNot,param.iDigits);
  fprintf(fp," [W/m^2]\n");
  fprintf(fp,"\n");

  fprintf(fp,"Secondary dSpinRate/dt: ");
  fprintd(fp,dDomegaDt[0],param.iSciNot,param.iDigits);
  fprintf(fp,"\n");
  fprintf(fp,"Secondary dObl/dt: ");
  fprintd(fp,dDoblDt[1]*param.iUnitAngle/param.iUnitTime,param.iSciNot,param.iDigits);
  fprintf(fp,"\n");

  /*
  fprintf(fp,"Primary Annual Heat: ");
  fprintd(fp,dAnnHeat[0],param.iSciNot,param.iDigits);
  fprintf(fp," [W/m^2]\n");
  */
  fprintf(fp,"Secondary Tidal Heat: ");
  fprintd(fp,dTideHeat[1],param.iSciNot,param.iDigits);
  fprintf(fp," [W]\n");
  fprintf(fp,"Secondary Tidal Surface Flux: ");
  fprintd(fp,dTideSurfFlux[1],param.iSciNot,param.iDigits);
  fprintf(fp," [W/m^2]\n");
  fprintf(fp,"Secondary's Equilibrium Rotation Rate: ");
  fprintd(fp,dEqSpinRate[1]*dTimeUnit(param.iUnitTime,io.exit_units),param.iSciNot,param.iDigits);
  fprintf(fp,"\n");
  fprintf(fp,"Secondary's Equilibrium Tidal Surface Flux: ");
  fprintd(fp,dEqHeatFlux[1],param.iSciNot,param.iDigits);
  fprintf(fp," [W/m^2]\n");
  fprintf(fp,"\n");

  if (param.iTideModel == 0) {
    fprintf(fp,"Use Discrete Rotation States: %d\n",param.bDiscreteRot);
    fprintf(fp,"Discrete Equilibrium Spin Period: ");
    fprintd(fp,dFreqToPer(EqSpinRate_CPL2Discrete(sec.dMeanMotion,sec.dEcc))/dTimeUnit(param.iUnitTime,io.exit_units),param.iSciNot,param.iDigits);
    fprintf(fp,"\n");
    fprintf(fp,"Continuous Equilibrium Spin Period: ");
    fprintd(fp,dFreqToPer(EqSpinRate_CPL2Cont(sec.dMeanMotion,sec.dEcc))/dTimeUnit(param.iUnitTime,io.exit_units),param.iSciNot,param.iDigits);
    fprintf(fp,"\n");

    fprintf(fp,"Primary epsilons: %d %d %d %d %d %d\n",epsilon[0][0],epsilon[0][1],epsilon[0][2],epsilon[0][5],epsilon[0][8],epsilon[0][9]);
    fprintf(fp,"Primary Gamma_Rotation: ");
    fprintd(fp,dGammaRot(sec.dEcc,pri.dObliquity,epsilon[0]),param.iSciNot,param.iDigits);
    fprintf(fp,"\n");
    fprintf(fp,"Primary Gamma_Orbital: ");
    fprintd(fp,dGammaOrb(sec.dEcc,pri.dObliquity,epsilon[0]),param.iSciNot,param.iDigits);
    fprintf(fp,"\n");

    fprintf(fp,"Secondary epsilons: %d %d %d %d %d %d\n",epsilon[1][0],epsilon[1][1],epsilon[1][2],epsilon[1][5],epsilon[1][8],epsilon[1][9]);
    fprintf(fp,"Secondary Gamma_Rotation: ");
    fprintd(fp,dGammaRot(sec.dEcc,sec.dObliquity,epsilon[1]),param.iSciNot,param.iDigits);
    fprintf(fp,"\n");
    fprintf(fp,"Secondary Gamma_Orbital: ");
    fprintd(fp,dGammaOrb(sec.dEcc,sec.dObliquity,epsilon[1]),param.iSciNot,param.iDigits);
    fprintf(fp,"\n");

  } else if (param.iTideModel == 9) {
    fprintf(fp,"Secondary's Equilibrium Rotation Period: ");
    fprintd(fp,dFreqToPer(dEqSpinRate[1])/dTimeUnit(param.iUnitTime,io.exit_units),param.iSciNot,param.iDigits);
    fprintf(fp,"\n");
  }
  
  fprintf(fp,"Tidal Luminosity: ");
  fprintd(fp,dFluxToLuminosity(dTideSurfFlux[1],sec.dRadius),param.iSciNot,param.iDigits);
  fprintf(fp,"\n");

  fprintf(fp,"Equilibrium Tidal Luminosity: ");
  fprintd(fp,dFluxToLuminosity(dEqHeatFlux[1],sec.dRadius),param.iSciNot,param.iDigits);
  fprintf(fp,"\n");

  /*
  fprintf(fp,"Secondary Annual Heat: ");
  fprintd(fp,dAnnHeat[1],param.iSciNot,param.iDigits);
  fprintf(fp," [W/m^2]\n");
  */

  /*
   * Habitable Zone
   */

  if (!iEnd) {
    if (hz.iModelIHZ > -1) {
      fprintf(fp,"\n--------- Insolation Habitable Zone ---------\n\n");
      
      fprintf(fp,"Model: ");
      if (hz.iModelIHZ == 0) {
	  /* Kasting93 */
	  fprintf(fp,"Kasting, J. et al. (1993), Icarus, 101, 108\n"); 
      } else if (hz.iModelIHZ == 1) {
	  /* Selsis07 */      
	  fprintf(fp,"Selsis, F. et al. (2007), A&A, 476, 1373-1387.\n");
      } else if (hz.iModelIHZ == 2) {
	  /* Barnes08 */
	  fprintf(fp,"Barnes, R. et al. (2008), Astrobio, 8, 557-568.\n");
      } else if (hz.iModelIHZ == 3) {
	  /* Ramires11 */
	  fprintf(fp,"Ramires, R. et al., in prep.\n");
      } else if (hz.iModelIHZ == 9) {
	  /* Agol11 */
	  fprintf(fp,"Agol, E. 2011, ApJ, 731, L31\n");
      }
      fprintf(fp,"Number of IHZ boundaries: %d\n",hz.iNumIHZ);
      fprintf(fp,"Inner Boundaries: ");
      for (i=0;i<hz.iNumIHZ;i++) {
	  fprintd(fp,hz.dInnerIHZ[i]/dLengthUnit(param.iUnitLength,io.exit_units),param.iSciNot,param.iDigits);
	  fprintf(fp," ");
	  }
      fprintf(fp,"\n");
      fprintf(fp,"Outer Boundaries: ");
      for (i=0;i<hz.iNumIHZ;i++) {
	  fprintd(fp,hz.dOuterIHZ[i]/dLengthUnit(param.iUnitLength,io.exit_units),param.iSciNot,param.iDigits);
	  fprintf(fp," ");
	  }
      fprintf(fp,"\n");
      
      fprintf(fp,"Halt at IHZ Crossing: ");
      if (hz.bHaltIHZ == 0) {
	fprintf(fp,"No\n");
      } else {
	fprintf(fp,"Yes\n");
      }
      
    }
    
    fprintf(fp,"\n----------- Halt ---------\n\n");

    /* Minimum Obliquity */
    fprintf(fp,"Minimum Obliquity: ");
    if (param.iUnitAngle == 0) {
	tmp=hz.dMinObliquity;
    } else {
	tmp=hz.dMinObliquity/dAngleUnit(param.iUnitAngle,io.exit_units);
    }
    fprintd(fp,tmp,param.iSciNot,param.iDigits);
    fprintf(fp,"\n");

    if (param.halt.bDblSync) 
	fprintf(fp,"HALT at double synchronous state.\n");
    

    if (hz.iModelHHZ != 0) {
      fprintf(fp,"\n--------- Heat Flux Habitable Zone ---------\n\n");
      
      if (hz.iModelHHZ == 1) {
	/* Jackson08 */
	fprintf(fp,"Model: Jackson et al. (2008), MNRAS, 391, 237-245\n");
	fprintf(fp,"Number of HHZ boundaries: %d\n",hz.iNumHHZ);
	fprintf(fp,"Inner Boundaries (W/m^2): ");
	for (i=0;i<hz.iNumHHZ;i++) {
	  fprintf(fp,"%.5e ",hz.dInnerHHZ[i]);
	}
	fprintf(fp,"Outer Boundaries (W/m^2): ");
	for (i=0;i<hz.iNumHHZ;i++) {
	  fprintf(fp,"%.5e ",hz.dOuterHHZ[i]);
	}
      }
      
      fprintf(fp,"Halt at HHZ Crossing: ");
      if (hz.bHaltHHZ == 0) {
	fprintf(fp,"No\n");
      } else {
	fprintf(fp,"Yes\n");
      }
    }


  }/* iEnd */
}










void Output(PARAM param,PRIMARY pri,SECONDARY sec,HZ hz,OUTPUT output,IO io,double dTime,double dDaDt,double dDeDt,double dDomegaDt[2],double dDpsiDt[2],double dDt,FILE *fp) {
  int j,k,ncol,n=0;
    double col[NUMOUT];
    int **epsilon; 
    double *z,*chi,*f,beta,dIrradiation;

    ncol=0;

    epsilon=malloc(2*sizeof(int*));
    epsilon[0]=malloc(9*sizeof(int));
    epsilon[1]=malloc(9*sizeof(int));

    /* Note that z is overloaded here. In the rest of the code, z is
       used for CTL8, whereas zprime is used for CTL2 */

    z=malloc(2*sizeof(double));
    chi=malloc(2*sizeof(double));
    f=malloc(5*sizeof(double));

    if (param.iTideModel == 0) {
	AssignEpsilon(pri.dSpinRate,sec.dMeanMotion,epsilon[0]);
	AssignEpsilon(sec.dSpinRate,sec.dMeanMotion,epsilon[1]);
	AssignZprime(pri,sec,z);
    } else if (param.iTideModel == 9) {
	f[0]=AssignF1(sec.dEcc);
	f[1]=AssignF2(sec.dEcc);
	f[2]=AssignF3(sec.dEcc);
	f[3]=AssignF4(sec.dEcc);
	f[4]=AssignF5(sec.dEcc);
	beta=AssignBeta(sec.dEcc);
	AssignZ(pri,sec,z);
    }
    AssignChi(pri,sec,chi);


      dIrradiation = dInsolation(pri.dLuminosity,sec.dSemi,sec.dEcc)*(1-sec.dAlbedo);

    /* Assign outputs */
    for (j=0;j<NUMOUT;j++) {

	/* 
	 * E
	 */

	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_EQSPINPERCONT])) {
	    col[j+n]=dFreqToPer(EqSpinRate_CPL2Cont(sec.dMeanMotion,sec.dEcc));
	  if (output.iNeg[OUT_EQSPINPERCONT]) 
	    col[j+n] *= output.dConvert[OUT_EQSPINPERCONT];
	  else
	    col[j+n] /= dTimeUnit(param.iUnitTime,io.exit_units);
	  ncol++;
	}


	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_EQSPINRATECONT])) {
	    col[j+n]=EqSpinRate_CPL2Cont(sec.dMeanMotion,sec.dEcc);
	  if (output.iNeg[OUT_EQSPINRATECONT]) 
	    col[j+n] *= output.dConvert[OUT_EQSPINRATECONT];
	  else
	    col[j+n] /= dTimeUnit(param.iUnitTime,io.exit_units);
	  ncol++;
	}


	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_EQSPINPERDISCRETE])) {
	    col[j+n]=dFreqToPer(EqSpinRate_CPL2Discrete(sec.dMeanMotion,sec.dEcc));
	  if (output.iNeg[OUT_EQSPINPERDISCRETE]) 
	    col[j+n] *= output.dConvert[OUT_EQSPINPERDISCRETE];
	  else
	    col[j+n] /= dTimeUnit(param.iUnitTime,io.exit_units);
	  ncol++;
	}


	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_EQSPINRATEDISCRETE])) {
	    col[j+n]=EqSpinRate_CPL2Discrete(sec.dMeanMotion,sec.dEcc);
	  if (output.iNeg[OUT_EQSPINRATEDISCRETE]) 
	    col[j+n] *= output.dConvert[OUT_EQSPINRATEDISCRETE];
	  else
	    col[j+n] /= dTimeUnit(param.iUnitTime,io.exit_units);
	  ncol++;
	}


      /*
       *   HABITABLE ZONE
       *
       */

	/* Inner IHZ */
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_HZINNERINSOL])) {
	  if (hz.iModelIHZ == 0) {
	    /* Call Kasting93 */
	  } else if (hz.iModelIHZ == 1) {
	    /* Selsis07 */
	    HZSelsis07Inner(pri.dLuminosity,pri.dTeff,hz.dInnerIHZ);
	  } else if (hz.iModelIHZ == 2) {
	    /* Barnes08 */
	    HZBarnes08Inner(pri.dLuminosity,pri.dTeff,sec.dEcc,hz.dInnerIHZ);
	  } else if (hz.iModelIHZ == 3) {
	    /* Ramires11 */
	    HZRamires11Inner(pri.dLuminosity,pri.dTeff,hz.dInnerIHZ);
	  } else if (hz.iModelIHZ == 9) {
	    /* Agol11 */
	  }
	  for (k=0;k<hz.iNumIHZ;k++) {
	      col[j+n+k]=hz.dInnerIHZ[k];
	      if (output.iNeg[OUT_HZINNERINSOL]) 
		col[j+n+k] *= output.dConvert[OUT_HZINNERINSOL];
	      else
		col[j+n+k] /= dLengthUnit(param.iUnitLength,io.exit_units);
	  }
	  ncol += k;
	  n += hz.iNumIHZ-1; /* Subtract one because still expect 1 column. */
	}
	
	/* Outer IHZ */
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_HZOUTERINSOL])) {
	  if (hz.iModelIHZ == 0) {
	    /* Call Kasting93 */
	  } else if (hz.iModelIHZ == 1) {
	    /* Selsis07 */
	    HZSelsis07Outer(pri.dLuminosity,pri.dTeff,hz.dOuterIHZ);
	  } else if (hz.iModelIHZ == 2) {
	    /* Barnes08 */
	    HZBarnes08Outer(pri.dLuminosity,pri.dTeff,sec.dEcc,hz.dOuterIHZ);
	  } else if (hz.iModelIHZ == 3) {
	    /* Ramires11 */
	    HZRamires11Outer(pri.dLuminosity,pri.dTeff,hz.dOuterIHZ);
	  } else if (hz.iModelIHZ == 9) {
	    /* Agol11 */
	  }
	  for (k=0;k<hz.iNumIHZ;k++) {
	      col[j+n+k]=hz.dOuterIHZ[k];
	      if (output.iNeg[OUT_HZOUTERINSOL]) 
		col[j+n+k] *= output.dConvert[OUT_HZOUTERINSOL];
	      else
		col[j+n+k] /= dLengthUnit(param.iUnitLength,io.exit_units);
	  }
	  ncol += k;
	  n += hz.iNumIHZ-1;
	}

	/*
	 *   ORBIT
	 */

	/* Orbital Angular Momentum */
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_ORBANGMOM])) {
	  col[j+n]=dOrbAngMom(pri,sec);
	  ncol++;
	}

	/* Eccentricity */
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_ORBECC])) {
	    col[j+n]=sec.dEcc;
	    ncol++;
	}

	/* Total da/dt */
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_ORBDADT])) {
	  col[j+n]=dDaDt;
	  if (output.iNeg[OUT_ORBDADT])
	    col[j+n] *= output.dConvert[OUT_ORBDADT];
	  else /* AU/Gyr */
	    col[j+n] *= dTimeUnit(param.iUnitTime,io.exit_units);
	  ncol++;
	}

	/* Total dOrbPerDt */

	/* de/dt */
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_ORBDEDT])) {
	  col[j+n]=dDeDt;
	  if (output.iNeg[OUT_ORBDEDT])
	    col[j+n] *= output.dConvert[OUT_ORBDEDT];
	  else /* /Gyr */
	    col[j+n] *= dTimeUnit(param.iUnitTime,io.exit_units);
	  ncol++;
	}

	/* Mean Motion */
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_ORBMEANMOTION])) {
	  col[j+n]=sec.dMeanMotion*dTimeUnit(param.iUnitTime,io.exit_units);
	  ncol++;
	}

	/* Semi-Major Axis */
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_ORBSEMI])) {
	  col[j+n]=sec.dSemi;
	  if (output.iNeg[OUT_ORBSEMI])
	    col[j+n] *= output.dConvert[OUT_ORBSEMI];
	  else /* AU */
	    col[j+n] /= dLengthUnit(param.iUnitLength,io.exit_units);
	  ncol++;
	}

	/* Orbital Period */
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_ORBPER])) {
	    col[j+n]=a2p(sec.dSemi,(sec.dMass+pri.dMass));
	    if (output.iNeg[OUT_ORBPER])
		col[j+n] *= output.dConvert[OUT_ORBPER];
	    else 
		col[j+n] /= dTimeUnit(param.iUnitTime,io.exit_units);
	    ncol++;
	}

	/*
	 *   PRIMARY
	 */
	
	/* Rotational Angular Momentum */
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_PRIANGMOM])) {
	  col[j+n]=dSpinAngMom(pri.dRG,pri.dMass,pri.dRadius,pri.dSpinRate);
	  ncol++;
	}
	
	/* dPerDt due to radial contraction */
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_PRICONTRDPERDT])) {
	  col[j+n]=dDPerDt(pri.dSpinRate,dDSpinRateContrDt(pri.dRadius,pri.dSpinRate,pri.iRadContr,sec.dAge));
	  if (output.iNeg[OUT_PRICONTRDPERDT])
	    col[j+n] *= output.dConvert[OUT_PRICONTRDPERDT];

	  ncol++;
	}

	/* dRadiusDt due to radial contraction */
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_PRICONTRDRADDT])) {
	  if (pri.iRadContr == 1) {
	    col[j+n] = dDRadDt_Baraffe98(sec.dAge);
	  } else if (pri.iRadContr == 2) {
	    col[j+n] = dDRadDt_DM97(sec.dAge);
	  } else {
	    col[j+n] = 0;
	  }
	  if (output.iNeg[OUT_PRICONTRDRADDT]) 
	    col[j+n] *= output.dConvert[OUT_PRICONTRDRADDT];
	  else 
	    col[j+n] *= dTimeUnit(param.iUnitTime,io.exit_units)/dLengthUnit(param.iUnitLength,io.exit_units);

	  ncol++;
	}

	/* dSpinRateDt due to radial contraction */
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_PRICONTRDSPINDT])) {
	  col[j+n]=dDSpinRateContrDt(pri.dRadius,pri.dSpinRate,pri.iRadContr,sec.dAge);
	  if (output.iNeg[OUT_PRICONTRDSPINDT])
	    col[j+n] *= output.dConvert[OUT_PRICONTRDSPINDT];

	  ncol++;
	}

	/* Primary's Contribution to da/dt */
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_PRIDADT])) {
	  if (param.iTideModel == 0)  /* CPL2 */
	    col[j+n] = dDaDt1_CPL2(pri.dMass,sec.dMass,sec.dSemi,sec.dEcc,pri.dObliquity,epsilon[0],z[0]);
	  else /* CTL8 */
	      col[j+n] = dDaDt1_CTL8(pri.dMass,sec.dMass,sec.dSemi,sec.dEcc,sec.dMeanMotion,z[0],pri.dSpinRate,pri.dObliquity,f,beta);

	  if (output.iNeg[OUT_PRIDADT])
	    col[j+n] *= output.dConvert[OUT_PRIDADT];
	  else
	    col[j+n] *= dTimeUnit(param.iUnitTime,io.exit_units)/dLengthUnit(param.iUnitLength,io.exit_units);
	  ncol++;
	}

	/* Primary's Contribution to de/dt */
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_PRIDEDT])) {
	  if (param.iTideModel == 0)  /* CPL2 */
	    col[j+n] = dDeDt1_CPL2(pri.dMass,sec.dMass,sec.dSemi,sec.dEcc,epsilon[0],z[0]);
	  else /* CTL8 */
	    col[j+n] = dDeDt1_CTL8(pri.dMass,sec.dMass,sec.dSemi,sec.dEcc,sec.dMeanMotion,z[0],pri.dSpinRate,pri.dObliquity,f,beta);

	  if (output.iNeg[OUT_PRIDEDT])
	    col[j+n] *= output.dConvert[OUT_PRIDEDT];
	  else
	    col[j+n] *= dTimeUnit(param.iUnitTime,io.exit_units)/dLengthUnit(param.iUnitLength,io.exit_units);

	  ncol++;
	}

	/* Rate of Change of Obliquity in Primary Body */
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_PRIDOBLDT])) {
	  col[j+n]=dDpsiDt[0];
	  if (output.iNeg[OUT_PRIDOBLDT])
	    col[j+n] *= output.dConvert[OUT_PRIDOBLDT];
	  else /* deg/Gyr */
	    col[j+n] *= dTimeUnit(param.iUnitTime,io.exit_units)/dAngleUnit(param.iUnitAngle,io.exit_units);
	  ncol++;
	}

	/* Rate of Change of Rotation Period of Primary Body */
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_PRIDPERDT])) {
	    col[j+n]=dDPerDt(pri.dSpinRate,dDomegaDt[0]);

	  /* Radial contraction? */
	  if (pri.iRadContr > 0) 
	      col[j+n] += dDPerDt(pri.dSpinRate,dDSpinRateContrDt(pri.dRadius,pri.dSpinRate,pri.iRadContr,sec.dAge));

	  if (output.iNeg[OUT_PRIDPERDT]) 
	    col[j+n] *= output.dConvert[OUT_PRIDPERDT];
	  else 
	    col[j+n] *= dTimeUnit(param.iUnitTime,io.exit_units)*dTimeUnit(param.iUnitTime,io.exit_units);

	    ncol++;
	}

	/* Rate of Change of Primary's Spin Rate*/
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_PRIDSPINDT])) {
	    col[j+n]=dDomegaDt[0];

	  /* Radial contraction? */
	  if (pri.iRadContr > 0) 
	      col[j+n] += dDSpinRateContrDt(pri.dRadius,pri.dSpinRate,pri.iRadContr,sec.dAge);

	  if (output.iNeg[OUT_PRIDSPINDT]) 
	    col[j+n] *= output.dConvert[OUT_PRIDSPINDT];
	  /* else Unitless */

	    ncol++;
	}

	/* Primary's Equilibrium Spin Period */
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_PRIEQSPINPER])) {
	  if (param.iTideModel == 0)  /* CPL2 */
	      col[j+n] = dFreqToPer(EqSpinRate_CPL2(sec.dMeanMotion,sec.dEcc,param.bDiscreteRot));
	  else /* CTL8 */
	    col[j+n] = dFreqToPer(EqSpinRate_CTL8(sec.dMeanMotion,sec.dEcc,pri.dObliquity));

	  if (output.cNeg[OUT_PRIEQSPINPER])
	    col[j+n] *= output.dConvert[OUT_PRIEQSPINPER];
	  else
	    col[j+n] /= dTimeUnit(param.iUnitTime,io.exit_units);
	  ncol++;
	}


	/* Primary's Equilibrium Spin Rate */
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_PRIEQSPINRATE])) {
	  if (param.iTideModel == 0)  /* CPL2 */
	      col[j+n] = EqSpinRate_CPL2(sec.dMeanMotion,sec.dEcc,param.bDiscreteRot);
	  else /* CTL8 */
	    col[j+n] = EqSpinRate_CTL8(sec.dMeanMotion,sec.dEcc,pri.dObliquity);

	  if (output.cNeg[OUT_PRIEQSPINRATE])
	    col[j+n] *= output.dConvert[OUT_PRIEQSPINRATE];
	  else
	    col[j+n] *= dTimeUnit(param.iUnitTime,io.exit_units);
	  ncol++;
	}

	/* Primary's Gamma_Orbital */
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_PRIGAMMAORB])) {
	  col[j+n]=dGammaOrb(sec.dEcc,pri.dObliquity,epsilon[0]);
	  ncol++;
	}
	
	/* Primary's Gamma_Rotation */
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_PRIGAMMAROT])) {
	  col[j+n]=dGammaRot(sec.dEcc,pri.dObliquity,epsilon[0]);
	  ncol++;
	}
	
	/* Primary Body's Heat */
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_PRIHEAT])) {
	  if (param.iTideModel == 0)  /* CPL2 */
	      col[j+n]=dTideHeat_CPL2(epsilon[0],z[0],sec.dEcc,sec.dMeanMotion,pri.dSpinRate,pri.dObliquity);
	  else /* CTL8 */
	      col[j+n]=dTideHeat_CTL8(z[0],f,beta,pri.dSpinRate,pri.dObliquity,sec.dMeanMotion);

	  col[j+n] += dRadHeat();

	  if (output.iNeg[OUT_PRIHEAT]) 
	    col[j+n] *= output.dConvert[OUT_PRIHEAT];
	  else /* TeraWatts */
	    /* Convert to system units?! */
	    {}
	  ncol++;
	}

	/* Primary's Obliquity */
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_PRIOBL])) {
	  col[j+n]=pri.dObliquity/dAngleUnit(param.iUnitAngle,io.exit_output);
	  ncol++;
	}

	/* Primary's Luminosity */
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_PRILUM])) {
	  col[j+n]=pri.dLuminosity;
	  if (output.iNeg[OUT_PRILUM]) {
	    col[j+n] *= output.dConvert[OUT_PRILUM];
	  }
	  else
	    /* Convert to System Units!? */
	    {}
	  ncol++;
	}

	/* Primary's Radius */
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_PRIRAD])) {
	  col[j+n] = pri.dRadius;
	  if (output.iNeg[OUT_PRIRAD])
	    col[j+n] *= output.dConvert[OUT_PRIRAD];
	  else 
	    col[j+n] /= dLengthUnit(param.iUnitLength,io.exit_units);

	  ncol++;
	}
    
	/* Primary's Rotational Energy */
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_PRIROTEN])) {
	  col[j+n]=dRotEn(pri.dMass,pri.dRadius,pri.dRG,pri.dSpinRate);
	  if (output.iNeg[OUT_PRIROTEN]) 
	    /* ergs is for negative, so do nothing */
	    {}
	  else
	    /* Convert to System Units!? */
	    {}
	  ncol++;
	}

	/* Primary's Spin Period */
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_PRISPINPER])) {
	  col[j+n] = dFreqToPer(pri.dSpinRate);
	  if (output.iNeg[OUT_PRISPINPER])
	    col[j+n] *= output.dConvert[OUT_PRISPINPER];
	  else
	    col[j+n] /= dTimeUnit(param.iUnitTime,io.exit_param);
	  ncol++;
	}

	/* Primary's Rotation Rate */
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_PRISPINRATE])) {
	    col[j+n]=pri.dSpinRate;
	    if (output.iNeg[OUT_PRISPINRATE])
		col[j+n] *= output.dConvert[OUT_PRISPINRATE];
	    else
		col[j+n] *= dTimeUnit(param.iUnitTime,io.exit_output);
	  ncol++;
	}

	/* Primary's Surface Heat Flux */
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_PRISURFFLUX])) {
	  if (param.iTideModel == 0)  /* CPL2 */
	      col[j+n]=dTideHeat_CPL2(epsilon[0],z[0],sec.dEcc,sec.dMeanMotion,pri.dSpinRate,pri.dObliquity);
	  else /* CTL8 */
	      col[j+n]=dTideHeat_CTL8(z[0],f,beta,pri.dSpinRate,pri.dObliquity,sec.dMeanMotion);
	  col[j+n] += dRadHeat();
	  col[j+n] /= 4*PI*pri.dRadius*pri.dRadius;

	  if (output.iNeg[OUT_PRISURFFLUX])
	    col[j+n] *= output.dConvert[OUT_PRISURFFLUX];
	  else 
	    /* Convert to system units?! */
	    {}
	  ncol++;
	}

	/* Primary's Effective Temperature */
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_PRITEFF])) {
	  col[j+n]=pri.dTeff;
	  if (output.iNeg[OUT_PRITEFF]) {
	    col[j+n] *= output.dConvert[OUT_PRITEFF];
	  }
	  /* Default is Kelvin */
	  ncol++;
	}

	/* Rate of Change of Rotation Period of Primary due to Tides*/
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_PRITIDEDPERDT])) {
	    col[j+n]=dDPerDt(pri.dSpinRate,dDomegaDt[0]);
	    if (output.iNeg[OUT_PRITIDEDPERDT])
		col[j+n] *= output.dConvert[OUT_PRITIDEDPERDT];
	    /* else unitless */

	    ncol++;
	}
	  
	/* Rate of Change of Spin Rate of Primary due to Tides*/
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_PRITIDEDSPINDT])) {
	    col[j+n]=dDomegaDt[0];
	    if (output.iNeg[OUT_PRITIDEDSPINDT])
		col[j+n] *= output.dConvert[OUT_PRITIDEDSPINDT];
	    /* else unitless */
	    else 
		col[j+n] *= dTimeUnit(param.iUnitTime,io.exit_units)*dTimeUnit(param.iUnitTime,io.exit_units);

	    ncol++;
	}
	  
	/* Primary Body's Tidal Heat */
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_PRITIDEHEAT])) {
	  if (param.iTideModel == 0)  /* CPL2 */
	      col[j+n]=dTideHeat_CPL2(epsilon[0],z[0],sec.dEcc,sec.dMeanMotion,pri.dSpinRate,pri.dObliquity);
	  else /* CTL8 */
	      col[j+n]=dTideHeat_CTL8(z[0],f,beta,pri.dSpinRate,pri.dObliquity,sec.dMeanMotion);

	  if (output.iNeg[OUT_PRITIDEHEAT] == 0)
	    col[j+n] *= output.dConvert[OUT_PRITIDEHEAT];
	  else /* TeraWatts */
	    /* Convert to system units?! */
	    {}
	    ncol++;
	}

	/* Primary's Surface Heat Flux due to Tide*/
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_PRITIDESURFFLUX])) {
	  if (param.iTideModel == 0)  /* CPL2 */
	      col[j+n]=dTideHeat_CPL2(epsilon[0],z[0],sec.dEcc,sec.dMeanMotion,pri.dSpinRate,pri.dObliquity);
	  else /* CTL8 */
	      col[j+n]=dTideHeat_CTL8(z[0],f,beta,pri.dSpinRate,pri.dObliquity,sec.dMeanMotion);
	  col[j+n] += dRadHeat();
	  col[j+n] /= 4*PI*pri.dRadius*pri.dRadius;

	  if (output.iNeg[OUT_PRITIDESURFFLUX])
	    col[j+n] *= output.dConvert[OUT_PRITIDESURFFLUX];
	  else /* W/m^2 ?? */
	    /* Convert to system units?! */
	    {}
	  
	  ncol++;
	}

	/* Primary's Rotational Velocity */
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_PRIVROT])) {
	  col[j+n] = dRotVel(pri.dRadius,pri.dSpinRate);
	  if (output.iNeg[OUT_PRIVROT])
	    col[j+n] *= output.dConvert[OUT_PRIVROT];
	  else /* km/s */
	    col[j+n] *= dTimeUnit(param.iUnitTime,io.exit_param)/dLengthUnit(param.iUnitLength,io.exit_param);
	  
	  ncol++;
	}

	/*
	 *   SECONDARY
	 */

	/* Rotational Angular Momentum */
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_SECANGMOM])) {
	  col[j+n]=dSpinAngMom(sec.dRG,sec.dMass,sec.dRadius,sec.dSpinRate);
	  ncol++;
	}

	/* dPerDt due to radial contraction */
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_SECCONTRDPERDT])) {
	  col[j+n]=dFreqToPer(dDSpinRateContrDt(sec.dRadius,sec.dSpinRate,sec.iRadContr,sec.dAge));
	  if (output.iNeg[OUT_SECCONTRDPERDT])
	    col[j+n] *= output.dConvert[OUT_SECCONTRDPERDT];

	  ncol++;
	}

	/* dRadiusDt due to radial contraction */
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_SECCONTRDRADDT])) {
	  if (pri.iRadContr == 1) {
	    col[j+n] = dDRadDt_Baraffe98(sec.dAge);
	  } else if (pri.iRadContr == 2) {
	    col[j+n] = dDRadDt_DM97(sec.dAge);
	  } else {
	    col[j+n] = 0;
	  }
	  if (output.iNeg[OUT_SECCONTRDRADDT]) 
	    col[j+n] *= output.dConvert[OUT_SECCONTRDRADDT];
	  else 
	    col[j+n] *= dTimeUnit(param.iUnitTime,io.exit_units)/dLengthUnit(param.iUnitLength,io.exit_units);

	  ncol++;
	}

	/* dSpinRateDt due to radial contraction */
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_SECCONTRDSPINDT])) {
	  col[j+n]=dFreqToPer(dDSpinRateContrDt(sec.dRadius,sec.dSpinRate,sec.iRadContr,sec.dAge));
	  if (output.iNeg[OUT_SECCONTRDSPINDT])
	    col[j+n] *= output.dConvert[OUT_SECCONTRDSPINDT];

	  ncol++;
	}

	/* Seondary's Contribution to da/dt */
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_SECDADT])) {
	  if (param.iTideModel == 0)  /* CPL2 */
	    col[j+n] = dDaDt1_CPL2(sec.dMass,pri.dMass,sec.dSemi,sec.dEcc,sec.dObliquity,epsilon[1],z[1]);
	  else /* CTL8 */
	    col[j+n] = dDaDt1_CTL8(sec.dMass,pri.dMass,sec.dSemi,sec.dEcc,sec.dMeanMotion,z[1],sec.dSpinRate,sec.dObliquity,f,beta);

	  if (output.cNeg[OUT_SECDADT])
	    col[j+n] *= output.dConvert[OUT_SECDADT];
	  else
	    col[j+n] *= dTimeUnit(param.iUnitTime,io.exit_units)/dLengthUnit(param.iUnitLength,io.exit_units);
	  ncol++;
	}

	/* Secondary's Contribution to de/dt */
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_SECDEDT])) {
	  if (param.iTideModel == 0)  /* CPL2 */
	    col[j+n] = dDeDt1_CPL2(sec.dMass,pri.dMass,sec.dSemi,sec.dEcc,epsilon[1],z[1]);
	  else /* CTL8 */
	    col[j+n] = dDeDt1_CTL8(sec.dMass,pri.dMass,sec.dSemi,sec.dEcc,sec.dMeanMotion,z[1],sec.dSpinRate,sec.dObliquity,f,beta);

	  if (output.cNeg[OUT_SECDEDT])
	    col[j+n] *= output.dConvert[OUT_SECDEDT];
	  else
	    col[j+n] *= dTimeUnit(param.iUnitTime,io.exit_units)/dLengthUnit(param.iUnitLength,io.exit_units);
	  ncol++;
	}

	/* Rate of Change of Obliquity in Secondary */
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_SECDOBLDT])) {
	  col[j+n]=dDpsiDt[1];
	  if (output.iNeg[OUT_SECDOBLDT])
	    col[j+n] *= output.dConvert[OUT_SECDOBLDT];
	  else
	    col[j+n] *= dTimeUnit(param.iUnitTime,io.exit_units)/dAngleUnit(param.iUnitAngle,io.exit_units);
	  ncol++;
	}

	/* Rate of Change of Secondary's Rotation Rate */


	/* ADD */

	/* Rate of Change of Rotation Period of Secondary */
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_SECDPERDT])) {
	    col[j+n]=dDPerDt(sec.dSpinRate,dDomegaDt[1]);
	  
	  /* Radial contraction? */
	  if (sec.iRadContr > 0) 
	      col[j+n] += dDPerDt(sec.dSpinRate,dDSpinRateContrDt(sec.dRadius,sec.dSpinRate,sec.iRadContr,sec.dAge));

	  if (output.iNeg[OUT_SECDPERDT]) 
	    col[j+n] *= output.dConvert[OUT_SECDPERDT];
	  /* else Unitless */
	  ncol++;
	}

	/* Secondary's Equilibrium Spin Period */
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_SECEQSPINPER])) {
	  if (param.iTideModel == 0)  /* CPL2 */
	      col[j+n] = dFreqToPer(EqSpinRate_CPL2(sec.dMeanMotion,sec.dEcc,param.bDiscreteRot));
	  else /* CTL8 */
	    col[j+n] = dFreqToPer(EqSpinRate_CTL8(sec.dMeanMotion,sec.dEcc,sec.dObliquity));

	  if (output.cNeg[OUT_SECEQSPINPER])
	    col[j+n] *= output.dConvert[OUT_SECEQSPINPER];
	  else
	    col[j+n] *= dTimeUnit(param.iUnitTime,io.exit_units);
	  ncol++;
	}

	/* Secondary's Equilibrium Spin Rate */
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_SECEQSPINRATE])) {
	  if (param.iTideModel == 0)  /* CPL2 */
	      col[j+n] = EqSpinRate_CPL2(sec.dMeanMotion,sec.dEcc,param.bDiscreteRot);
	  else /* CTL8 */
	    col[j+n] = EqSpinRate_CTL8(sec.dMeanMotion,sec.dEcc,sec.dObliquity);

	  if (output.cNeg[OUT_SECEQSPINRATE])
	    col[j+n] *= output.dConvert[OUT_SECEQSPINRATE];
	  else
	    col[j+n] *= dTimeUnit(param.iUnitTime,io.exit_units);
	  ncol++;
	}

	/* Secondary's Equilibrium Tidal Heat Flux */
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_SECEQSURFFLUX])) {
	  if (param.iTideModel == 0)  /* CPL2 */
	      col[j+n] = dTideHeatEq_CPL2(z[1],sec.dEcc,sec.dObliquity,sec.dMeanMotion,param.bDiscreteRot);
	  else /* CTL8 */
	    col[j+n] = dTideHeatEq_CTL8(z[1],f,beta,sec.dObliquity,sec.dMeanMotion);

	  col[j+n] /= 4*PI*sec.dRadius*sec.dRadius;

	  if (output.cNeg[OUT_SECEQSURFFLUX])
	    col[j+n] *= output.dConvert[OUT_SECEQSURFFLUX];
	  else
	    col[j+n] *= 1;
	  ncol++;
	}

	/* Secondary's Gamma_Orbital */
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_SECGAMMAORB])) {
	  col[j+n]=dGammaOrb(sec.dEcc,sec.dObliquity,epsilon[1]);
	  ncol++;
	}
	
	/* Secondary's Gamma_Rotation */
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_SECGAMMAROT])) {
	  col[j+n]=dGammaRot(sec.dEcc,sec.dObliquity,epsilon[1]);
	  ncol++;
	}
	
	/* Secondary's Heat */
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_SECHEAT])) {
	  if (param.iTideModel == 0)  /* CPL2 */
	      col[j+n]=dTideHeat_CPL2(epsilon[1],z[1],sec.dEcc,sec.dMeanMotion,sec.dSpinRate,sec.dObliquity);
	  else /* CTL8 */
	      col[j+n]=dTideHeat_CTL8(z[1],f,beta,sec.dSpinRate,sec.dObliquity,sec.dMeanMotion);
	  col[j+n] += dRadHeat();

	  if (output.iNeg[OUT_SECHEAT])
	    col[j+n] *= output.dConvert[OUT_SECHEAT];
	  else 
	    /* Convert to system units?! */
	    {}
	  
	  ncol++;
	}

	/* Secondary's Insolation */
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_SECINSOLATION])) {
	    col[j+n] = dInsolation(pri.dLuminosity,sec.dSemi,sec.dEcc)*(1-sec.dAlbedo)/4;

	  if (output.iNeg[OUT_SECINSOLATION])
	    col[j+n] *= output.dConvert[OUT_SECINSOLATION];
	  else 
	    /* Convert to system units?! */
	    {}
	  
	  ncol++;
	}

	/* Secondary's Obliquity */
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_SECOBL])) {
	  col[j+n]=sec.dObliquity/dAngleUnit(param.iUnitAngle,io.exit_output);
	  ncol++;
	}
	
	/* Secondary's Tidal Q */
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_SECQ])) {
	  col[j+n]=sec.dQ;
	  ncol++;
	}
	
	/* Peak Wavelength Emission: Synchronous, Naked, Radiation */
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_SECPEAKSUBNAKEDIRR])) {
	  col[j+n] = dFluxToPeakWavelength(dInsolation(pri.dLuminosity,sec.dSemi,sec.dEcc)*(1-sec.dAlbedo));
	  if (output.iNeg[OUT_SECPEAKSUBNAKEDIRR])
	    col[j+n] /= dLengthUnit(param.iUnitLength,io.exit_units);
	  else 
	    col[j+n] *= output.dConvert[OUT_SECPEAKSUBNAKEDIRR];
	  ncol++;
	}

	/* Peak Wavelength Emission: Synchronous, Naked, Tides+Radiation */
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_SECPEAKSUBNAKEDALL])) {
	  if (param.iTideModel == 0)  /* CPL2 */
	    col[j+n] = dFluxToPeakWavelength(dInsolation(pri.dLuminosity,sec.dSemi,sec.dEcc)*(1-sec.dAlbedo) + dTideHeat_CPL2(epsilon[1],z[1],sec.dEcc,sec.dMeanMotion,sec.dSpinRate,sec.dObliquity)/(4*PI*sec.dRadius*sec.dRadius) );
	  else /* CTL8 */
	    col[j+n] = dFluxToPeakWavelength( (dInsolation(pri.dLuminosity,sec.dSemi,sec.dEcc))*(1-sec.dAlbedo) + (dTideHeat_CTL8(z[1],f,beta,sec.dSpinRate,sec.dObliquity,sec.dMeanMotion)/(4*PI*sec.dRadius*sec.dRadius)) );

	  if (output.iNeg[OUT_SECPEAKSUBNAKEDALL])
	    col[j+n] /= dLengthUnit(param.iUnitLength,io.exit_units);
	  else 
	    col[j+n] *= output.dConvert[OUT_SECPEAKSUBNAKEDALL];
	  ncol++;
	}

    
	/* Peak Wavelength Emission: Tides */
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_SECPEAKTIDE])) {
	  if (param.iTideModel == 0)  /* CPL2 */
	    col[j+n] = dFluxToPeakWavelength(dTideHeat_CPL2(epsilon[1],z[1],sec.dEcc,sec.dMeanMotion,sec.dSpinRate,sec.dObliquity)/(4*PI*sec.dRadius*sec.dRadius));
	  else /* CTL8 */
	    col[j+n] = dFluxToPeakWavelength(dTideHeat_CTL8(z[1],f,beta,sec.dSpinRate,sec.dObliquity,sec.dMeanMotion)/(4*PI*sec.dRadius*sec.dRadius));

	  if (output.iNeg[OUT_SECPEAKTIDE])
	    col[j+n] /= dLengthUnit(param.iUnitLength,io.exit_units);
	  else 
	    col[j+n] *= output.dConvert[OUT_SECPEAKTIDE];
	  ncol++;
	}
      
	/* Peak Wavelength Emission: Tides */
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_SECPEAKTIDE])) {
	  if (param.iTideModel == 0)  /* CPL2 */
	    col[j+n] = dFluxToPeakWavelength(dTideHeat_CPL2(epsilon[1],z[1],sec.dEcc,sec.dMeanMotion,sec.dSpinRate,sec.dObliquity)/(4*PI*sec.dRadius*sec.dRadius));
	  else /* CTL8 */
	    col[j+n] = dFluxToPeakWavelength(dTideHeat_CTL8(z[1],f,beta,sec.dSpinRate,sec.dObliquity,sec.dMeanMotion)/(4*PI*sec.dRadius*sec.dRadius));
	   
	  if (output.iNeg[OUT_SECPEAKTIDE])
	    col[j+n] /= dLengthUnit(param.iUnitLength,io.exit_units);
	  else 
	    col[j+n] *= output.dConvert[OUT_SECPEAKTIDE];
	  ncol++;
	}
       
	/* Peak Wavelength Emission: Greenhouse, Radiation */  
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_SECPEAKSURFGRIRR])) {
	  col[j+n]= dFluxToPeakWavelength(dInsolation(pri.dLuminosity,sec.dSemi,sec.dEcc)*(1-sec.dAlbedo)/4);
	  if (output.iNeg[OUT_SECPEAKSURFGRIRR])
	    col[j+n] /= dLengthUnit(param.iUnitLength,io.exit_units);
	  else 
	    col[j+n] *= output.dConvert[OUT_SECPEAKSURFGRIRR];
	  ncol++;
	}

	/* Peak Wavelength Emission: Greenhouse, Tides */
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_SECPEAKSURFGRTIDE])) {
	  if (param.iTideModel == 0)  /* CPL2 */
	    col[j+n] = dFluxToPeakWavelength(dTideHeat_CPL2(epsilon[1],z[1],sec.dEcc,sec.dMeanMotion,sec.dSpinRate,sec.dObliquity)/(4*PI*sec.dRadius*sec.dRadius));
	  else /* CTL8 */
	    col[j+n] = dFluxToPeakWavelength(dTideHeat_CTL8(z[1],f,beta,sec.dSpinRate,sec.dObliquity,sec.dMeanMotion)/(4*PI*sec.dRadius*sec.dRadius));

	  if (output.iNeg[OUT_SECPEAKSURFGRTIDE])
	    col[j+n] /= dLengthUnit(param.iUnitLength,io.exit_units);
	  else 
	    col[j+n] *= output.dConvert[OUT_SECPEAKSURFGRTIDE];
	  ncol++;
	}
    
	/* Peak Wavelength Emission: Greenhouse, Tides+Radiation */
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_SECPEAKSURFGRALL])) {
	  if (param.iTideModel == 0)  /* CPL2 */
	    col[j+n]= dFluxToPeakWavelength( (dInsolation(pri.dLuminosity,sec.dSemi,sec.dEcc)*(1-sec.dAlbedo)/4) + (dTideHeat_CPL2(epsilon[1],z[1],sec.dEcc,sec.dMeanMotion,sec.dSpinRate,sec.dObliquity)/(4*PI*sec.dRadius*sec.dRadius)) );
	  else /* CTL */
	    col[j+n]= dFluxToPeakWavelength( (dInsolation(pri.dLuminosity,sec.dSemi,sec.dEcc)*(1-sec.dAlbedo)/4) + (dTideHeat_CTL8(z[1],f,beta,sec.dSpinRate,sec.dObliquity,sec.dMeanMotion)/(4*PI*sec.dRadius*sec.dRadius)) );

	  if (output.iNeg[OUT_SECPEAKSURFGRALL])
	    col[j+n] /= dLengthUnit(param.iUnitLength,io.exit_units);
	  else 
	    col[j+n] *= output.dConvert[OUT_SECPEAKSURFGRALL];
	  ncol++;
	}
    
	/* Secondary's Rotational Energy */
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_SECROTEN])) {
	  col[j+n]=dRotEn(sec.dMass,sec.dRadius,sec.dRG,sec.dSpinRate);
	  if (output.iNeg[OUT_SECROTEN]) 
	    /* ergs is for negative, so do nothing */
	    {}
	  else
	    /* Convert to System Units!? */
	  ncol++;
	}

	/* Secondary's Spin Period */
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_SECSPINPER])) {
	  col[j+n] = dFreqToPer(sec.dSpinRate);
	  if (output.iNeg[OUT_SECSPINPER])
	    col[j+n] *= output.dConvert[OUT_SECSPINPER];
	  else
	    col[j+n] /= dTimeUnit(param.iUnitTime,io.exit_param);
	  ncol++;
	}

	/* Secondary's Radius */
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_SECRAD])) {
	  col[j+n] = sec.dRadius;
	  if (output.iNeg[OUT_SECRAD])
	    col[j+n] *= output.dConvert[OUT_SECRAD];
	  else 
	    col[j+n] /= dLengthUnit(param.iUnitLength,io.exit_units);

	  ncol++;
	}

	/* Secondary's Rotation Rate */
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_SECSPINRATE])) {
	  col[j+n]=sec.dSpinRate;
	  if (output.iNeg[OUT_SECSPINRATE])
	    col[j+n] *= output.dConvert[OUT_SECSPINRATE];
	  else
	    col[j+n] *= dTimeUnit(param.iUnitTime,io.exit_output);
	  ncol++;
	}

	/* Secondary's Surface Heat Flux */
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_SECSURFFLUX])) {
	  if (param.iTideModel == 0)  /* CPL2 */
	      col[j+n]=dTideHeat_CPL2(epsilon[1],z[1],sec.dEcc,sec.dMeanMotion,sec.dSpinRate,sec.dObliquity);
	  else /* CTL8 */
	      col[j+n]=dTideHeat_CTL8(z[1],f,beta,sec.dSpinRate,sec.dObliquity,sec.dMeanMotion);

	  col[j+n] += dRadHeat();
	  col[j+n] /= 4*PI*sec.dRadius*sec.dRadius;
	  col[j+n] += dInsolation(pri.dLuminosity,sec.dSemi,sec.dEcc)*(1-sec.dAlbedo)/4;

	  if (output.iNeg[OUT_SECSURFFLUX])
	    col[j+n] *= output.dConvert[OUT_SECSURFFLUX];
	  else /* W/m^2 ?? */
	    /* Convert to system units?! */
	    {}
	  
	  ncol++;
	}

	/* Secondary's Tidal Time Lag */
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_SECTAU])) {
	  col[j+n] = sec.dTau;
	  if (output.iNeg[OUT_SECTAU])
	    col[j+n] *= output.dConvert[OUT_SECTAU];
	  else
	    col[j+n] /= dTimeUnit(param.iUnitTime,io.exit_param);
	  ncol++;
	}

	/* Rate of Change of Rotation Period of Secondary due to Tides*/
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_SECTIDEDPERDT])) {
	    col[j+n]=dDPerDt(sec.dSpinRate,dDomegaDt[1]);
	    if (output.iNeg[OUT_SECTIDEDPERDT])
		col[j+n] *= output.dConvert[OUT_SECTIDEDPERDT];
	    /* else unitless */

	    ncol++;
	}

	/* Surface Temperature: Synchronous, Substellar Point, Naked, Radiation */
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_SECTEMPSUBNAKEDIRR])) {
	  col[j+n]= dFluxToTemperature(dInsolation(pri.dLuminosity,sec.dSemi,sec.dEcc)*(1-sec.dAlbedo));
	  if (output.iNeg[OUT_SECTEMPSUBNAKEDIRR])
	    col[j+n] = dTempUnit(col[j],0,param.iUnitTemp,io.exit_units);
	  else 
	    col[j+n] *= output.dConvert[OUT_SECTEMPSUBNAKEDIRR];
	  ncol++;
	}
    
	/* Surface Temperature: Synchronous, Substellar Point, Naked, Radiation+Tides */
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_SECTEMPSUBNAKEDALL])) {

	  if (param.iTideModel == 0)  /* CPL2 */
	    col[j+n] = dFluxToTemperature(dInsolation(pri.dLuminosity,sec.dSemi,sec.dEcc)*(1-sec.dAlbedo) + dTideHeat_CPL2(epsilon[1],z[1],sec.dEcc,sec.dMeanMotion,sec.dSpinRate,sec.dObliquity)/(4*PI*sec.dRadius*sec.dRadius) );
	  else /* CTL8 */
	    col[j+n] = dFluxToTemperature( (dInsolation(pri.dLuminosity,sec.dSemi,sec.dEcc)) + (dTideHeat_CTL8(z[1],f,beta,sec.dSpinRate,sec.dObliquity,sec.dMeanMotion)/(4*PI*sec.dRadius*sec.dRadius)) );

	  if (output.iNeg[OUT_SECTEMPSUBNAKEDALL])
	    col[j+n] = dTempUnit(col[j],0,param.iUnitTemp,io.exit_units);
	  else 
	    col[j+n] *= output.dConvert[OUT_SECTEMPSUBNAKEDALL];
	  ncol++;
	}

	/* Surface Temperature: Tides */
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_SECTEMPTIDE])) {
	  if (param.iTideModel == 0)  /* CPL2 */
	    col[j+n] = dFluxToTemperature(dTideHeat_CPL2(epsilon[1],z[1],sec.dEcc,sec.dMeanMotion,sec.dSpinRate,sec.dObliquity)/(4*PI*sec.dRadius*sec.dRadius));
	  else /* CTL8 */
	    col[j+n] = dFluxToTemperature(dTideHeat_CTL8(z[1],f,beta,sec.dSpinRate,sec.dObliquity,sec.dMeanMotion)/(4*PI*sec.dRadius*sec.dRadius));

	  if (output.iNeg[OUT_SECTEMPTIDE])
	    col[j+n] /= dLengthUnit(param.iUnitLength,io.exit_units);
	  else 
	    col[j+n] *= output.dConvert[OUT_SECTEMPTIDE];
	  ncol++;
	}
         
	/* Surface Temperature: Greenhouse, Radiation */
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_SECTEMPSURFGRIRR])) {
	  col[j+n]= dFluxToTemperature(dInsolation(pri.dLuminosity,sec.dSemi,sec.dEcc)*(1-sec.dAlbedo)/4);
	  if (output.iNeg[OUT_SECPEAKSURFGRIRR])
	    col[j+n] = dTempUnit(col[j],0,param.iUnitTemp,io.exit_units);
	  else 
	    col[j+n] *= output.dConvert[OUT_SECTEMPSURFGRIRR];
	  ncol++;
	}
 
	/* Surface Temperature: Greenhouse, Radiation+Tides */
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_SECTEMPSURFGRALL])) {
	  if (param.iTideModel == 0)  /* CPL2 */
	    col[j+n]= dFluxToTemperature( (dInsolation(pri.dLuminosity,sec.dSemi,sec.dEcc)*(1-sec.dAlbedo)/4) + (dTideHeat_CPL2(epsilon[1],z[1],sec.dEcc,sec.dMeanMotion,sec.dSpinRate,sec.dObliquity)/(4*PI*sec.dRadius*sec.dRadius)) );
	  else /* CTL */
	    col[j+n]= dFluxToTemperature( (dInsolation(pri.dLuminosity,sec.dSemi,sec.dEcc)*(1-sec.dAlbedo)/4) + (dTideHeat_CTL8(z[1],f,beta,sec.dSpinRate,sec.dObliquity,sec.dMeanMotion)/(4*PI*sec.dRadius*sec.dRadius)) );

	  if (output.iNeg[OUT_SECPEAKSURFGRALL])
	    col[j+n] /= dLengthUnit(param.iUnitLength,io.exit_units);
	  else 
	    col[j+n] *= output.dConvert[OUT_SECPEAKSURFGRALL];
	  ncol++;
	}

	/* Rate of Change of Rotation Period of Secondary due to Tides*/
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_SECTIDEDSPINDT])) {
	    col[j+n]=dDomegaDt[1];
	    if (output.iNeg[OUT_SECTIDEDSPINDT])
		col[j+n] *= output.dConvert[OUT_SECTIDEDSPINDT];
	    /* else unitless */
	    else 
		col[j+n] *= dTimeUnit(param.iUnitTime,io.exit_units)*dTimeUnit(param.iUnitTime,io.exit_units);

	    ncol++;
	}
	  
	/* Secondary's Tidal Heat */
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_SECTIDEHEAT])) {
	  if (param.iTideModel == 0)  /* CPL2 */
	      col[j+n]=dTideHeat_CPL2(epsilon[1],z[1],sec.dEcc,sec.dMeanMotion,sec.dSpinRate,sec.dObliquity);
	  else /* CTL8 */
	      col[j+n]=dTideHeat_CTL8(z[1],f,beta,sec.dSpinRate,sec.dObliquity,sec.dMeanMotion);

	  if (output.iNeg[OUT_SECTIDEHEAT])
	    col[j+n] *= output.dConvert[OUT_SECTIDEHEAT];
	  else /* TeraWatts */
	    /* Convert to system units?! */
	    {}
	    ncol++;
	}

	/* Secondary Tidally Locked? */
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_SECTIDELOCK])) {
	  col[j+n]=sec.bForceEqSpin;
	  ncol++;
	}

	/* Secondary's Surface Heat Flux due to Tides */
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_SECTIDESURFFLUX])) {
	  if (param.iTideModel == 0)  /* CPL2 */
	      col[j+n]=dTideHeat_CPL2(epsilon[1],z[1],sec.dEcc,sec.dMeanMotion,sec.dSpinRate,sec.dObliquity);
	  else /* CTL8 */
	      col[j+n]=dTideHeat_CTL8(z[1],f,beta,sec.dSpinRate,sec.dObliquity,sec.dMeanMotion);
	  col[j+n] /= 4*PI*sec.dRadius*sec.dRadius;

	  if (output.iNeg[OUT_SECTIDESURFFLUX])
	    col[j+n] *= output.dConvert[OUT_SECTIDESURFFLUX];
	  else
	    /* Convert to system units */
	    {}
	  ncol++;
	}
   
	/* Secondary's Rotational Velocity */
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_SECVROT])) {
	  col[j+n] = dRotVel(pri.dRadius,pri.dSpinRate);
	  if (output.iNeg[OUT_SECVROT])
	    col[j+n] *= output.dConvert[OUT_SECVROT];
	  else
	    col[j+n] *= dTimeUnit(param.iUnitTime,io.exit_param)/dLengthUnit(param.iUnitLength,io.exit_param);
	  
	  ncol++;
	}

	/* Secondary's Atmosphere */

	/* Secondary's Water Status */
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_SECWET])) {
	  col[j+n]=sec.bWet;
	  ncol++;
	}
	
	/* Mass loss rate */
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_SECMASSLOSS])) {
	  col[j+n]=dAtmMassLossErkaev07(sec,pri.dLumX,pri.dMass);
	  if (output.iNeg[OUT_SECMASSLOSS]) 
	    col[j+n] *= output.dConvert[OUT_SECMASSLOSS];
	  else 
	    col[j+n] *= dTimeUnit(param.iUnitTime,io.exit_param)/dMassUnit(param.iUnitMass,io.exit_param);

	  ncol++;
	}

	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_SECMASSH])) {
	  col[j+n]=sec.dMassH;
	  if (output.iNeg[OUT_SECMASSLOSS]) 
	    col[j+n] *= output.dConvert[OUT_SECMASSLOSS];
	  else 
	    col[j+n] *= dMassUnit(param.iUnitMass,io.exit_param);

	  ncol++;
	}
	
	/* Current Time */
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_TIME])) {
	  col[j+n]=dTime;
	  if (output.iNeg[OUT_TIME])
	    col[j+n] *= output.dConvert[OUT_TIME];
	  else
	    col[j+n] /= dTimeUnit(param.iUnitTime,io.exit_output);
	  ncol++;
	}

	/* Age */
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_AGE])) {
	  col[j+n]=sec.dAge;
	  if (output.iNeg[OUT_AGE])
	    col[j+n] *= output.dConvert[OUT_AGE];
	  else 
	    col[j+n] /= dTimeUnit(param.iUnitTime,io.exit_output);

	  ncol++;
	}

	/* Timestep */
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_DT])) {
	  col[j+n]=dDt;
	  if (output.iNeg[OUT_DT])
	      col[j+n] *= output.dConvert[OUT_DT];
	  else
	    col[j+n] /= dTimeUnit(param.iUnitTime,io.exit_output);
	  ncol++;
	}

	/* Total Angular Momentum */
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_TOTANGMOM])) {
	  col[j+n] =  dOrbAngMom(pri,sec) + dSpinAngMom(pri.dRG,pri.dMass,pri.dRadius,pri.dSpinRate) + dSpinAngMom(sec.dRG,sec.dMass,sec.dRadius,sec.dSpinRate);
	  ncol++;
	}

	/* Total Energy */
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_TOTEN])) {
	  col[j+n] = dOrbEn(pri.dMass,sec.dMass,sec.dSemi) + dRotEn(pri.dMass,pri.dRadius,pri.dRG,pri.dSpinRate) + dRotEn(sec.dMass,sec.dRadius,sec.dRG,sec.dSpinRate);
	  if (output.iNeg[OUT_TOTEN]) 
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
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_TAUSEMI])) {
	  if (dDaDt != 0) 
	    col[j+n] = fabs(sec.dSemi/dDaDt);
	  else
	    col[j+n] = HUGE;

	  if (output.iNeg[OUT_TAUSEMI])
	    col[j+n] *= output.dConvert[OUT_TAUSEMI];
	  else
	    col[j+n] /= dTimeUnit(param.iUnitTime,io.exit_output);
	  ncol++;
	}
	
	/* Eccentricity Timescale */
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_TAUECC])) {
	  if (dDeDt != 0)
	    col[j+n] = fabs(sec.dEcc/dDeDt);
	  else 
	    col[j+n] = HUGE;

	  if (output.iNeg[OUT_TAUECC])
	    col[j+n] *= output.dConvert[OUT_TAUECC];
	  else
	    col[j+n] /= dTimeUnit(param.iUnitTime,io.exit_output);
	  ncol++;
	}
	
	/* Primary Spin Timescale */
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_TAUPRISPIN])) {
	  if (dDomegaDt[0] != 0)
	    col[j+n] = fabs(pri.dSpinRate/dDomegaDt[0]);
	  else
	    col[j+n] = HUGE;

	  if (output.iNeg[OUT_TAUPRISPIN])
	    col[j+n] *= output.dConvert[OUT_TAUPRISPIN];
	  else
	    col[j+n] /= dTimeUnit(param.iUnitTime,io.exit_output);
	  ncol++;
	}
	
	/* Primary Obliquity Timescale */
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_TAUPRIOBL])) {
	  if (dDpsiDt[0] != 0)
	    col[j+n] = fabs(pri.dObliquity/dDpsiDt[0]);
	  else
	    col[j+n] = HUGE;

	  if (output.iNeg[OUT_TAUPRIOBL])
	    col[j+n] *= output.dConvert[OUT_TAUPRIOBL];
	  else
	    col[j+n] /= dTimeUnit(param.iUnitTime,io.exit_output);
	  ncol++;
	}
	
	/* Secondary Spin Timescale */
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_TAUSECSPIN])) {
	  if (dDomegaDt[1] != 0)
	    col[j+n] = fabs(sec.dSpinRate/dDomegaDt[1]);
	  else
	    col[j+n] = HUGE;

	  if (output.iNeg[OUT_TAUSECSPIN])
	    col[j+n] *= output.dConvert[OUT_TAUSECSPIN];
	  else
	    col[j+n] /= dTimeUnit(param.iUnitTime,io.exit_output);
	  ncol++;
	}
	
	/* Secondary Obliquity Timescale */
	if (!strcmp(param.cOutputOrder[j],output.cParam[OUT_TAUSECOBL])) {
	  if (dDpsiDt[1] != 0)
	    col[j+n] = fabs(sec.dObliquity/dDpsiDt[1]);
	  else
	    col[j+n] = HUGE;

	  if (output.iNeg[OUT_TAUSECOBL])
	    col[j+n] *= output.dConvert[OUT_TAUSECOBL];
	  else
	    col[j+n] /= dTimeUnit(param.iUnitTime,io.exit_output);
	  ncol++;
	}
    }	

    /* Write out data */

    for (j=0;j<ncol;j++) {
	fprintd(fp,col[j],param.iSciNot,param.iDigits);
	fprintf(fp," ");
    }
    fprintf(fp,"\n");
    fflush(fp);
}

