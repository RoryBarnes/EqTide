/**********************************************************************
 *                                                                    *
 * EQTIDE is a general integrator of "equilibrium tide" models        *
 * as originally conceived by George Darwin. These equation were      *
 * first assembled in Heller, Leconte & Barnes (2011), and the        *
 * version first is released simultaneously with Barnes, R. (2016).   *
 *                                                                    *
 * EQTIDE is optimized for a rocky planet orbiting a main sequence    *
 * star, but is easily applied to any generic two-body system.        *
 * Example runs are provided at https://github.com/RoryBarnes/EqTide. *
 * Please submit pull requests if you find an error!                  *
 *                                                                    *
 * EQTIDE was written by Rory Barnes.                                 *
 *                                                                    *
 * eqtide.h                                                           *
 *                                                                    *
 * Header file for EQTIDE. All structs and global variables are       *
 * declared here and in options.h and output.h.                       *
 *                                                                    *
 **********************************************************************/

#include <float.h>

#define MEARTH        5.972186e27 // Prsa et al. 2016
#define MSUN          1.988416e33 // Prsa et al. 2016
#define AUCM          1.49598e13
#define RSUN          6.957e10     // Prsa et al. 2016
#define YEARSEC       3.15576e7
#define DAYSEC        86400
#define REARTH        6.3781e8    // Equatorial; Prsa et al. 2016
#define RJUP          7.1492e9    // Equatorial; Prsa et al. 2016
#define MJUP          1.898130e30 // Prsa et al. 2016

#define BIGG          6.67428e-8  // From Luzum et al., 2011; value recommended by IAU NSFA in Prsa et al. 2016
#define PI            3.1415926535

#define RNEP          2.4764e9
#define MNEP          1.0244e29
#define RHOEARTH      5.52
#define YEARDAY       365.25
#define MSAT          5.6851e29
#define DEGRAD        0.017453292519444445

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

#define OPTLEN        24
#define LINELEN       256
#define NUMOUT        1000       /* Number of output parameters */
#define NUMOPT	      1000

/* EPS is the floor for which two numbers are deemed to be unequal. If the
difference between two numbers is < EPS, then they are assumed to be the same.
This is particularly important for tidally locked body for which round-off
errors can cause the difference of the mean motion and rotational frequency
to be non-zero. */
#define EPS           1e-10

#define TINY          (1./DBL_MAX)

#define CPL           1
#define CTL           10

#define EULER         0
#define RK4           1

/*! The primary (more massive) body in the system. This struct
  does not contain orbital information. */

typedef struct {
  double dMass;		/**< Mass of primary */
  double dRadius;	/**< Radius of primary */
  double dTau;          /**< Time lag */
  double dQ;		/**< Tidal Q of primary */
  double dK2;		/**< Love number of primary */
  double dObliquity;    /**< Obliquity */
  double dSpinRate;     /**< Rotation Rate */
  double dRG;           /**< Radius of Gyration */
  double *iEpsilon;     /**< Signs of phase lags */

  int bForceEqSpin;     /**< Force spin rate to be equilibrium? */
  double dMaxLockDiff;  /**< When to set spin rate to equilibrium */

  int iMassRad;         /**< Mass-Radius relationship identifier */

  double dDomegaDt;     /**< Time derivative of spin rate */
  double dDobliquityDt; /**< Time derivative of obliquity */
} PRIMARY;

/*! The secondary (less massive) body in the system. This struct
  contains the orbital information. */

typedef struct {
  double dMass;		/**< Planetary Mass */
  double dRadius;	/**< Secondary Radius */
  double dTau;          /**< Time lag */
  double dQ;		/**< Secondary Tidal Q */
  double dK2;		/**< Secondary Love number */
  double dObliquity;    /**< Obliquity */
  double dSpinRate;     /**< Rotation rate */
  double dRG;           /**< Radius of Gyration */
  double dSyncEcc;      /**< Synchronous rotation below this e */

  int bForceEqSpin;     /**< Tidally lock second? */
  double dMaxLockDiff;  /**< When to set spin rate to equilibrium */

  double dSemi;         /**< Semi-major axis */
  double dEcc;          /**< Eccentricity */
  double dMeanMotion;   /**< Mean Motion */
  double dAge;          /**< System Age */

  int iMassRad;         /**< Mass-Radius relationship identifier */

  double dDomegaDt;     /**< Time derivative of spin rate */
  double dDobliquityDt; /**< Time derivative of obliquity */
  double dDaDt;         /**< Time derivative of semi-major axis */
  double dDeDt;         /**< Time derivative of eccentricity */
} SECONDARY;

typedef struct {
  int iVerbose;         /**< Verbosity: 0=no STDOUT, 1=all */

  int iDigits;          /**< Number of digits after the decimal. */
  int iSciNot;          /**< Decade to change from flt. point to exp. notation */

  int exit_param;       /**< Exit code for failure to specify necessary paramter */
  int exit_io;          /**< Exit code for failure to open/close a file */
  int exit_input;       /**< Exit code for incompatible paramters */
  int exit_exe;         /**< Exit code for execution error */
  int exit_units;       /**< Exit code for unit conversion error */
  int exit_output;      /**< Exit code for error during output */

} IO;

/*! Function pointer to the derivative subroutines. This variable is set
  to the address of the subroutines that calculate the deriviatives.
  The two options are CPL or CTL as set by iTideModel. */
typedef void (*fvDerivs)(PRIMARY*,SECONDARY*,IO*,double*,double*,double*,double,int**,double,int);

/*! Function pointer to the equilibrium (tide-locked) spin frequency, either
  CPL or CTL. */
typedef double (*fdEqSpin)(double,double,double,int);

typedef struct {
  int bHalt;            /**< Check for halts? */
  int bDblSync;         /**< Halt for double synchronous rotation? */
  int bMerge;           /**< Halt for merge? */
  double dMinSemi;	/**< Halt integration at this semi-major axis */
  double dMinPriObl;	/**< Halt integration at this obliquity */
  double dMinSecObl;	/**< Halt integration at this obliquity */
  double dMaxEcc;	/**< Halt integration at this eccentricity */
  double dMinEcc;	/**< Halt integration at this eccentricity */
  int bPosDeDt;         /**< Halt when de/dt > 0 */
  int bPriLock;         /**< Halt primary becomes tide locked? */
  int bSecLock;         /**< Halt secondary becomes tide locked? */
  int bSecSync;         /**< Halt if secondary rotation becomes synchronous? */
} HALT;

typedef struct {
  HALT halt;

  int iUnitMass;        /**< Mass Unit for input/output */
  int iUnitLength;      /**< Length Unit for input/output */
  int iUnitAngle;       /**< Angle Unit for input/output */
  int iUnitTime;        /**< Time Unit for input/output */

  char cSystemName[16];	/**< System name */
  int iTideModel;       /**< Integer version of tide model */
  int bDiscreteRot;	/**< Use Discrete Rotation model (CPL) */

  int bLog;             /**< Write log file? */

  double l0;            /**< Initial angular momentum */
  double dl;            /**< Change in angular momentum: (l-l0)/l0 */

  char *cOutputOrder[NUMOUT]; /**< Output order */
  int iNumCols;         /**< Number if columns in output file */

  double dMinValue;     /**< Minimum value for e and obl */

  /* Parameters to perform forward integration */
  int bDoForward;	/**< Perform forward integration? */
  double dForwTimeStep;	/**< Time step for forward integration (years) */
  double dForwStopTime;	        /**< Forward integration time (years) */
  double dForwOutputTime;	/**< Output interval in forward integration (years) */

  /* Parameters to perform reverse integration */
  int bDoBackward;		/**< Perfrom backward integration? */
  double dBackTimeStep;	        /**< Time step for backward integration (years) */
  double dBackStopTime;	        /**< Backward integration time (years) */
  double dBackOutputTime;	/**< Output interval in Backward integration (years) */

  int bVarDt;                   /**< Use variable timestep? */
  double dTimestepCoeff;        /**< Variable Timestep coefficients */

  int iIntegration;             /**< Integration method */
  fvDerivs fDerivs;             /**< Function pointer to derivatives */
  fdEqSpin fEqSpin;             /**< Function pointer to equilibrium spin rate */
} PARAM;


/*! Function pointer to integration method, either Euler or Runge-Kutta. */
typedef double (*fdStep)(PARAM*,PRIMARY*,SECONDARY*,IO*,double*,double*,double*,double,int**,double*,double,int);

typedef struct {
  char *cExe;     /**< Name of executable */
  char *cIn;      /**< Input file */
  char cFor[32];  /**< Forward integration */
  char cBack[32]; /**< Backward integraiton */
  char cLog[32];  /**< Log file */
} FILES;

typedef struct {
  char cParam[NUMOPT][OPTLEN];   /**< Option name */
  char cDescr[NUMOPT][LINELEN];  /**< Option description */
  int iType[NUMOPT];         /**< Option type (cast) */
  char cDefault[NUMOPT][OPTLEN]; /**< Default value */
} OPTIONS;

typedef struct {
  char *cParam[NUMOUT];   /**< Output parameter name */
  char cDescr[NUMOUT][LINELEN];  /**< Output descripton */
  int iNeg[NUMOUT];          /**< Was option negative? (Sets units) */
  char cNeg[NUMOUT][OPTLEN];     /**< String description of output units */
  double dConvert[NUMOUT];   /**< Conversion value for negative output */
} OUTPUT;

// String/print utilities
char *lower(char[]);
void fprintd(FILE*,double,int,int);
char *InitializeString();

/* Mass-Radius relations */
double dBaylessOrosz06_MassRad(double);
double dGordaSvech99_MassRad(double);
double dReidHawley_MassRad(double);
double dSotin07_MassRad(double);
double dBaylessOrosz06_RadMass(double);
double dBaraffe15_MassRad(double);
double dGordaSvech99_RadMass(double);
double dReidHawley_RadMass(double);
double dSotin07_RadMass(double);

double dDpDt(double,double,double);
void AssignChi(PRIMARY*,SECONDARY*,double*);
double a2p(double,double);
double p2a(double,double);
double dFreqToPer(double);
double dPerToFreq(double);
double dSemiToMeanMotion(double,double);
double dRotVel(double,double);
double DOrbPerDt(double,double,double);

/* Units */
double dLengthUnit(int,int);
double dTimeUnit(int,int);
double dMassUnit(int,int);
double dAngleUnit(int,int);

/* CPL2 Functions */
double EqSpinRate_CPL2(double,double,double,int);
void AssignEpsilon(double,double,int*);
void AssignZprime(PRIMARY*,SECONDARY*,double *);
double dDaDtAnn_CPL2(PRIMARY*,SECONDARY*,int**,double *);
double dDaDt_CPL2(PRIMARY*,SECONDARY*,int **,double*);
double dDeDt_CPL2(PRIMARY*,SECONDARY*,int **,double*);
double dDomegaDt_CPL2(double,double,double,double,double,double,int*,double);
double dDoblDt_CPL2(double,double,double,double,double,int*,double,double,double);
double dTideHeat_CPL2(int*,double,double,double,double,double);
void TideRewind_CPL2(PARAM*,PRIMARY*,SECONDARY*,OUTPUT*,FILES*,IO*,fdStep);
void TideForward_CPL2(PARAM*,PRIMARY*,SECONDARY*,OUTPUT*,FILES*,IO*,fdStep);
double dDaDt1_CPL2(double,double,double,double,double,int*,double);
double dDeDt1_CPL2(double,double,double,double,int*,double);
double dTideHeatEq_CPL2(double,double,double,double,int);
double EqSpinRate_CPL2Discrete(double,double);
double EqSpinRate_CPL2Cont(double,double);
double dGammaRot(double,double,int*);
double dGammaOrb(double,double,int*);
void DerivsCPL(PRIMARY*,SECONDARY*,IO*,double*,double*,double*,double,int**,double,int);

/* CTL8 Functions */
double EqSpinRate_CTL8(double,double,double,int);
double AssignF1(double);
double AssignF2(double);
double AssignF3(double);
double AssignF4(double);
double AssignBeta(double);
double AssignF5(double);
void AssignZ(PRIMARY*,SECONDARY*,double *);
double dDaDt_CTL8(PRIMARY*,SECONDARY*,double *,double,double *);
double dDeDt_CTL8(PRIMARY*,SECONDARY*,double *,double,double *);
double dDomegaDt_CTL8(double,double,double,double,double,double,double,double,double,double *);
double dDoblDt_CTL8(double,double,double,double,double,double,double,double,double,double*,double);
double dAnnualHeat_CTL8(double,double,double);
double dDailyHeat_CTL8(double,double*,double,double,double,double);
void TideForward_CTL8 (PARAM*,PRIMARY*,SECONDARY*,OUTPUT*,FILES*,IO*,fdStep);
void TideRewind_CTL8 (PARAM*,PRIMARY*,SECONDARY*,OUTPUT*,FILES*,IO*,fdStep);
double dDaDt1_CTL8(double,double,double,double,double,double,double,double,double *,double);
double dDeDt1_CTL8(double,double,double,double,double,double,double,double,double*,double);
double dTideHeat_CTL8(double,double*,double,double,double,double);
double dTideHeatEq_CTL8(double,double*,double,double,double);
void DerivsCTL(PRIMARY*,SECONDARY*,IO*,double*,double*,double*,double,int**,double,int);

/* Integration Methods */
double EulerStep(PARAM*,PRIMARY*,SECONDARY*,IO*,double*,double*,double*,double,int**,double*,double,int);
double RK4Step(PARAM*,PRIMARY*,SECONDARY*,IO*,double*,double*,double*,double,int**,double*,double,int);

/* Conserved Quantities */
double dOrbAngMom(PRIMARY*,SECONDARY*);
double dSpinAngMom(double,double,double,double);
double dRotEn(double,double,double,double);
double dOrbEn(double,double,double);

void LineExit(char[],int, int, int);
int iSign(double);
double dDPerDt(double,double);

// Major Functions
void InitializeIO(IO*);
void InitializeOutput(OUTPUT*);
void InitializeOptions(OPTIONS*);
void WriteHelp(OUTPUT,OPTIONS,char[]);
void ReadOptions(char[],OPTIONS,PARAM*,PRIMARY*,SECONDARY*,FILES*,OUTPUT*,IO*,fdStep*);
void WriteLog(PARAM,PRIMARY,SECONDARY,OUTPUT,FILES,IO,int);
void Output(PARAM*,PRIMARY*,SECONDARY*,OUTPUT*,IO*,double,double,FILE*);
