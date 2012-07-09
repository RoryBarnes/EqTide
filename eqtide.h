#define MEARTH        5.9742e27
#define MSUN          1.98892e33
#define AUCM          1.49598e13
#define LSUN          3.827e33
#define RSUN          6.955e10
#define YEARSEC       3.15576e7
#define DAYSEC        86400
#define REARTH        6.3781e8
#define RJUP          7.1492e9
#define MJUP          1.8987e30

#define BIGG          6.672e-8
#define PI            3.1415926535
#define SIGMA         5.67e-5
#define WIEN          0.28977685

#define RNEP          2.4764e9
#define MNEP          1.0244e29
#define RHOEARTH      5.52
#define LAMBDAU238    1.5e-10
#define FEARTH        0.08
#define IEARTH        1366
#define eEARTH        0.016710219
#define YEARDAY       365.25
#define MSAT          5.6851e29
#define DEGRAD        0.017453292519444445

#define MASSHYD       1.67e-24

#define EXIT_EXE      1
#define EXIT_PARAM    2
#define EXIT_UNITS    3
#define EXIT_WRITE    4
#define EXIT_INT      5
#define EXIT_OUTPUT   6

#define VERBERR       1
#define VERBPROG      2    
#define VERBINPUT     3
#define VERBUNITS     4
#define VERBALL       5

#define NUMOUT        1000       /* Number of output parameters */

#define TINY          (1./HUGE)

typedef struct {
  int bHalt;            /* Any halts? */
  int bDblSync;
  int bDry;             /* Halt if dry? */
  int bMerge;           /* Halt for merge? */
  double dMinSemi;	/* Halt integration at this semi-major axis */
  double dMinPriObl;	/* Halt integration at this obliquity */
  double dMinSecObl;	/* Halt integration at this obliquity */
  double dMaxEcc;	/* Halt integration at this eccentricity */
  double dMinEcc;	/* Halt integration at this eccentricity */
  int bPosDeDt;         /* Halt when de/dt > 0 */
  int bPriLock;         /* Halt primary becomes tide locked? */
  int bSecLock;         /* Halt secondary becomes tide locked? */
  int bSecSync;         /* Halt is secondary rotation becomes synchronous? */
} HALT;


typedef struct {

  HALT halt;

  int iVerbose;                 /* Verbosity: 0=no STDOUT, 1=all */

  int iUnitMass;                /* Mass Unit for input/output */
  int iUnitLength;              /* Length Unit for input/output */
  int iUnitAngle;               /* Angle Unit for input/output */
  int iUnitTemp;                /* Temperature Unit for input/output */
  int iUnitTime;                /* Time Unit for input/output */

  char cSystemName[16];	        /* System name */
  int iTideModel;               /* Integer version of tide model */
  int bDiscreteRot;		/* Use Discrete Rotation model (CPL) */

  int bLog;                     /* Write log file? */

  double l0;                    /* Initial angular momentum */
  double dl;                    /* Change in angular momentum: (l-l0)/l0 */
 
  char cOutputOrder[NUMOUT][24];/* Output order */
  int iNumCols;                 /* Number if columns in output file */

  /* Output Notation */
  int iDigits;
  int iSciNot;

  double dMinValue;             /* Minimum value for e and obl */

  /* Parameters to perform forward integration */
  int bDoForward;		/* Perform forward integration? */
  double dForwTimeStep;	        /* Time step for forward integration (years) */
  double dForwStopTime;	        /* Forward integration time (years) */
  double dForwOutputTime;	/* Output interval in forward integration (years) */
  int bForwVarDt;               /* Use variable timestep? */
  double dForwEta;              /* Variable Timestep coefficients */

  /* Parameters to perform reverse integration */
  int bDoBackward;		/* Perfrom backward integration? */
  double dBackTimeStep;	        /* Time step for backward integration (years) */
  double dBackStopTime;	        /* Backward integration time (years) */
  double dBackOutputTime;	/* Output interval in Backward integration (years) */
  int bBackVarDt;               /* Use variable timestep? */
  double dBackEta;              /* Variable Timestep coefficients */

  int bGrnhsWaterLoss;   	/* Track water loss from Runaway
				   Greenhouse? */
  double dGrnhsWaterLossTime;   /* Time to lose water from Runaway
				   Greenhouse/Photolysis/Jeans Escape */

  int bAtmMassLoss;             /* Track actual water loss? */
  double dHBlowOffTime;         /* Time to blow all H off */

} PARAM;

typedef struct {
  char *cExe; /* Name of executable */
  char *cIn;  /* Input file */
  char cFor[32]; /* Forward integration */
  char cBack[32];/* Backward integraiton */
  char cLog[32]; /* Log file */
} FILES;

typedef struct {
  int exit_param;       /* Exit code for failure to specify necessary paramter */
  int exit_io;          /* Exit code for failure to open/close a file */
  int exit_input;       /* Exit code for incompatible paramters */
  int exit_exe;         /* Exit code for execution error */
  int exit_units;
  int exit_output;
} IO;

typedef struct {
  double dMass;		/* Mass of star */
  double dRadius;	/* Radius of star */
  double dTau;          /* Time lag */
  double dQ;		/* Tidal Q of star */
  double dK2;		/* Love number of star */
  double dLuminosity;	/* Setllar luminosity */
  double dTeff;		/* Stellar effective temperature */
  double dObliquity;    /* Obliquity */
  double dSpinRate;     /* Rotation Rate */
  double dRG;           /* Radius of Gyration */
  double *iEpsilon;     /* Signs of phase lags */

  int bDoQPrime;	/* Read in Q' instead of Q (and ignore k2)? */
  int bForceEqSpin;     /* Force spin rate to be equilibrium? */
  double dMaxLockDiff;  /* When to set spin rate to equilibrium */

  int iMassRad;         /* Mass-Radius relationship identifier */
  int iMassLum;         /* Mass-Luminosity relationship identifier */
  double iRadContr;     /* Radius contraction model */
  int bWD;              /* White Dwarf? */
  double dLumX;         /* Total energy emitted in X-Ray/XUV */
} PRIMARY;

typedef struct {
  double dMass;		/* Planetary Mass */
  double dRadius;	/* Planetary Radius */
  double dTau;          /* Time lag */
  double dQ;		/* Planetary Tidal Q */
  double dWetQ;		/* Q if secondary has Ocean */
  double dDryQ;		/* Q if secondary is Dry */
  double dWetTau;	/* tau if secondary has Ocean */
  double dDryTau;     /* Tau if secondary is dry */
  int bWet;		/* Is Secondary Wet? */
  double dK2;		/* Planetary Love number */
  double dTeq;		/* Planetary equilibtirum temperature */
  double dObliquity;    /* Obliquity */
  double dSpinRate;     /* Rotation rate */
  double dRG;           /* Radius of Gyration */
  double *iEpsilon;     /* Signs of phase lags */
  double dLuminosity;	/* Setllar luminosity */
  double dTeff;		/* Stellar effective temperature */
  double dAlbedo;       /* Albedo */
  int iAlbedoModel;     /* Albedo Model */
  double dGrhnsFlux;	/* Critical flux to trigger Runaway Greenhouse */
  double dSyncEcc;      /* Synchronous rotation below this e */

  int bDoQPrime;	/* Read in Q' instead of Q (and ignore k2)? */
  int bForceEqSpin;     /* Tidally lock planet? */
  double dMaxLockDiff;  /* When to set spin rate to equilibrium */

  double dSemi;         /* Semi-major axis */
  double dEcc;          /* Eccentricity */
  double dMeanMotion;   /* Mean Motion */
  double dAge;          /* System Age */

  double dAtmXAbsEff;   /* Efficiency of X-Ray/XUV Absorption */
  double dRadX;         /* Radius to X-Ray/XUV photons */
  double dMassH;        /* Mass of hydrogen available for escape */

  int iMassRad;         /* Mass-Radius relationship identifier */
  int iMassLum;         /* Mass-Luminosity relationship identifier */
  double iRadContr;     /* Radius contraction model */
} SECONDARY;

typedef struct {
  int iModelIHZ;        /* ID of IHZ model */
  double *dInnerIHZ;    /* Inner IHZ boundaries */
  double *dOuterIHZ;    /* Outer IHZ boundaries */
  int iNumIHZ;          /* Number of IHZ boundaries */
  int bHaltIHZ;         /* Halt integration at IHZ Crossing? */

  int iModelHHZ;        /* ID of IHZ model */
  double *dInnerHHZ;    /* Highest habitable heat flux */
  double *dOuterHHZ;    /* Lowest habitable heat flux */
  int bHaltHHZ;         /* Halt integration at HHZ Crossing? */
  int iNumHHZ;          /* Number of heat flux HZ boundaries */

  int iRnwyGrnhsModel;   /* Moist Greenhouse Model */
  double dRnwyGrnhsFlux; /* Flux to trigger Moist Greenhouse */

  double dMinObliquity; /* Minimum obliquity */
} HZ;

typedef struct {
  char cParam[NUMOUT][24];
  char cDescr[NUMOUT][256];
  int iNeg[NUMOUT];
  char cNeg[NUMOUT][24];
  double dConvert[NUMOUT];

} OUTPUT;


void fprintd(FILE*,double,int,int);


/* Mass-Radius relations */
double dBaylessOrosz06_MassRad(double); 
double dGordaSvech99_MassRad(double);
double dReidHawley_MassRad(double);
double dSotin07_MassRad(double);
double dBaylessOrosz06_RadMass(double); 
double dGordaSvech99_RadMass(double);
double dReidHawley_RadMass(double);
double dSotin07_RadMass(double);

/* Mass-Luminosity Relations */
double dReidHawley_MassLum(double);
double dScalo07_MassLum(double);
double dReidHawley_LumMass(double);
double dScalo07_LumMass(double);

double dBergeronWDLum(double);

/* Habitable Zones */
double dBarnes08Inner(double,double,double,double);
double dBarnes08Outer(double,double,double,double);

double dDpDt(double,double,double);
void AssignChi(PRIMARY,SECONDARY,double*);
double a2p(double,double);
double p2a(double,double);
double dFreqToPer(double);
double dPerToFreq(double);
double dInsolation(double,double,double);
double dSemiToMeanMotion(double,double);
double dTeff(double,double);
double dTeq(double,double,double);
/* Units */
double dLengthUnit(int,int);
double dTimeUnit(int,int);
double dMassUnit(int,int);
double dTempUnit(double,int,int,int);
double dAngleUnit(int,int);

/* CPL2 Functions */
double EqSpinRate_CPL2(double,double,int);
void AssignEpsilon(double,double,int*);
void AssignZprime(PRIMARY,SECONDARY,double *);
double dDaDtAnn_CPL2(PRIMARY,SECONDARY,int**,double *);
double dDaDt_CPL2(PRIMARY,SECONDARY,int **,double*);
double dDeDt_CPL2(PRIMARY,SECONDARY,int **,double*);
double dDomegaDt_CPL2(double,double,double,double,double,double,int*,double);
double dDoblDt_CPL2(double,double,double,double,double,int*,double,double,double);
double dTideHeat_CPL2(int*,double,double,double,double,double);
void TideRewind_CPL2(PARAM,PRIMARY,SECONDARY,HZ,OUTPUT,FILES,IO);
void TideForward_CPL2(PARAM,PRIMARY,SECONDARY,HZ,OUTPUT,FILES,IO);
double dDaDt1_CPL2(double,double,double,double,double,int*,double);
double dDeDt1_CPL2(double,double,double,double,int*,double);
double dTideHeatEq_CPL2(double,double,double,double,int);
double EqSpinRate_CPL2Discrete(double,double);
double EqSpinRate_CPL2Cont(double,double);
double dGammaRot(double,double,int*);
double dGammaOrb(double,double,int*);

/* CTL8 Functions */
double EqSpinRate_CTL8(double,double,double);
double AssignF1(double);
double AssignF2(double);
double AssignF3(double);
double AssignF4(double);
double AssignBeta(double);
double AssignF5(double);
void AssignZ(PRIMARY,SECONDARY,double *);
double dDaDt_CTL8(PRIMARY,SECONDARY,double *,double,double *);
double dDeDt_CTL8(PRIMARY,SECONDARY,double *,double,double *);
double dDomegaDt_CTL8(double,double,double,double,double,double,double,double,double,double *);
double dDoblDt_CTL8(double,double,double,double,double,double,double,double,double,double*,double);
double dAnnualHeat_CTL8(double,double,double);
double dDailyHeat_CTL8(double,double*,double,double,double,double);
void TideForward_CTL8 (PARAM,PRIMARY,SECONDARY,HZ,OUTPUT,FILES,IO);
void TideRewind_CTL8 (PARAM,PRIMARY,SECONDARY,HZ,OUTPUT,FILES,IO);
double dDaDt1_CTL8(double,double,double,double,double,double,double,double,double *,double);
double dDeDt1_CTL8(double,double,double,double,double,double,double,double,double*,double);
double dTideHeat_CTL8(double,double*,double,double,double,double);
double dTideHeatEq_CTL8(double,double*,double,double,double);

/* Conserved Quantities */
double dOrbAngMom(PRIMARY,SECONDARY);
double dSpinAngMom(double,double,double,double);
double dRotEn(double,double,double,double);
double dOrbEn(double,double,double);

/* Output
void Output(PARAM,PRIMARY,SECONDARY,HZ,OUTPUT,IO,double,double,double,double[2],double[2],double[2],double[2],FILE*);
*/

void LineExit(char[],int, int, int);
char *lower(char[]);

double dRadHeat();

double dDSpinRateContrDt(double,double,int,double);
double dDRadDt_Baraffe98(double);
double dDRadDt_DM97(double);
double dSpinRateDt_dDRDT(double,double,double);
double dDRadDtContr(double,int);
double dDPerDt(double,double);

double dFluxToPeakWavelength(double);
double dTempToPeakWavelength(double);
double dBlackbodyLuminosity(double,double);
double dFluxToLuminosity(double,double);
double dNakedPlanetTemp(double,double,double,double,double);
double dRnwyGrnhsFlux(double,double,int);

/* HZ */
void HZBarnes08Inner(double,double,double,double *);
void HZBarnes08Outer(double,double,double,double *);
void HZSelsis07Inner(double,double,double *);
void HZSelsis07Outer(double,double,double *);
double dAlbedoRamires11(double);

/* Atmospheric Mass Loss */
double dDesiccationTimeErkaev07(SECONDARY,double,double);
double dAtmMassLossErkaev07(SECONDARY,double,double);
/* double dFxuv(double,double,double,double); -- Use dInsolation */
double dKTide(double,double,double,double);
