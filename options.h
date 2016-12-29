/**************************************************************************
 * options.h                                                              *
 *                                                                        *
 * Written by Rory Barnes                                                 * 
 *                                                                        *
 * This file contains definitions for input options.                      *
 *                                                                        *
 **************************************************************************/

/* 
 * Input Options
 *
 * Add integer definition here
 * Then add to
 * 1) InitializeOptions
 * 2) ReadInit
 * 3) WriteLog
 *
 */

/* UGH! This should really be done with enum! */

#define OPT_AGE			10

#define OPT_BACK		12
#define OPT_BACKFILE   	        14
#define OPT_BACKOUTPUTTIME	18
#define OPT_BACKSTOPTIME	20
#define OPT_BACKTIMESTEP	22

#define OPT_DISCRETEROT         26

#define OPT_FORW		28
#define OPT_FORWFILE		30
#define OPT_FORWOUTPUTTIME	34
#define OPT_FORWSTOPTIME	36
#define OPT_FORWTIMESTEP	38

#define OPT_HALTDBLSYNC	        42
#define OPT_HALTMAXECC	        44
#define OPT_HALTMERGE           46
#define OPT_HALTMINECC          48
#define OPT_HALTMINPRIOBL       50
#define OPT_HALTMINSECOBL       52
#define OPT_HALTMINSEMI		54
#define OPT_HALTPOSDEDT         56 
#define OPT_HALTPRILOCK         58
#define OPT_HALTSECLOCK         60
#define OPT_HALTSECSYNC         62

#define OPT_INTEGRATION         63

#define OPT_LOG		        64
#define OPT_LOGFILE		66

#define OPT_MINVALUE            68

#define OPT_ORBECC		70
#define OPT_ORBMEANMOTION	72
#define OPT_ORBPER		74
#define OPT_ORBSEMI		76

#define OPT_OUTDIGITS           78
#define OPT_OUTPUTORDER		80
#define OPT_OUTSCINOT	        82

#define OPT_PRIFORCEEQSPIN      84 
#define OPT_PRIK2		86
#define OPT_PRIMASS		88
#define OPT_PRIMASSRAD	        90
#define OPT_PRIMAXLOCKDIFF      92
#define OPT_PRIOBL	        94
#define OPT_PRIQ		96
#define OPT_PRIRAD	        98
#define OPT_PRIRG		100
#define OPT_PRISPINPER	        102
#define OPT_PRISPINRATE	        104
#define OPT_PRITAU		106
#define OPT_PRIVROT             108

#define OPT_SECFORCEEQSPIN      110
#define OPT_SECK2               112
#define OPT_SECMASS		114
#define OPT_SECMASSRAD	        116
#define OPT_SECMAXLOCKDIFF      118
#define OPT_SECOBL	        120
#define OPT_SECQ		122
#define OPT_SECRAD	        124
#define OPT_SECRG		126
#define OPT_SECSPINPER	        128
#define OPT_SECSPINRATE	        130
#define OPT_SECSYNCECC          132
#define OPT_SECTAU		134
#define OPT_SECVROT             136

#define OPT_SYSTEMNAME		138

#define OPT_TIDEMODEL		140
#define OPT_TIMESTEPCOEFF       141

#define OPT_UNITANGLE		142
#define OPT_UNITLENGTH		144
#define OPT_UNITMASS		146
#define OPT_UNITTEMP	        148
#define OPT_UNITTIME		150

#define OPT_VARDT               151
#define OPT_VERBOSE		152

/* This struct is for variable that store the line of redundant
   options. */
   
typedef struct {
  int lPriSpinRate;
  int lPriSpinPer;
  int lPriVRot;
  int lSecSpinRate;
  int lSecSpinPer;
  int lSecVRot;
  int lSemi;
  int lMeanMotion;
  int lPeriod;
} LINE;

/* This struct is for redundant variables */

typedef struct {
  double dPeriod;
  double dPriSpinPer;
  double dPriVRot;
  double dSecSpinPer;
  double dSecVRot;
  double dSemi;
} INPUT;


