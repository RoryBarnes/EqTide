/* 
 * Header file for options
 * /

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

#define NUMOPT			1000

#define OPT_AGE			10

#define OPT_SECATMXABSEFF       20
#define OPT_SECMASSH            25
#define OPT_SECMASSHFRAC        30
#define OPT_SECRADX             35
#define OPT_SECRADXFRAC         40
#define OPT_PRILUMX             45
#define OPT_PRLUMXFRAC          50

#define OPT_BACK		100
#define OPT_BACKFILE   	        110
#define OPT_BACKETA             120
#define OPT_BACKOUTPUTTIME	130
#define OPT_BACKSTOPTIME	140
#define OPT_BACKTIMESTEP	150
#define OPT_BACKVARDT	        160

#define OPT_DISCRETEROT         180

#define OPT_FORW		200
#define OPT_FORWFILE		210
#define OPT_FORWETA		220
#define OPT_FORWOUTPUTTIME	230
#define OPT_FORWSTOPTIME	240
#define OPT_FORWTIMESTEP	250
#define OPT_FORWVARDT           260

#define OPT_HALTDBLSYNC	        300
#define OPT_HALTDRY             310
#define OPT_HALTMAXECC          320
#define OPT_HALTMERGE           325
#define OPT_HALTMINECC          330
#define OPT_HALTMINPRIOBL       335
#define OPT_HALTMINSECOBL       340
#define OPT_HALTMINSEMI		345
#define OPT_HALTPOSDEDT         350 
#define OPT_HALTPRILOCK         360
#define OPT_HALTSECLOCK         370
#define OPT_HALTSECSYNC         375

#define OPT_HZDOGRHSWATERLOSS   400
#define OPT_HZGRHSWATERLOSSTIME	410
#define OPT_HZHEATMODEL		420
#define OPT_HZHEAT		430
#define OPT_HZINSOLMODEL	440
#define OPT_HZLOWERHEAT		450
#define OPT_HZRNWYGRNHSMODEL    460
#define OPT_HZRNWYGRNHSFLUX     470
#define OPT_HZUPPERHEAT		480

#define OPT_LOG		        500
#define OPT_LOGFILE		510

#define OPT_MINVALUE            520

#define OPT_ORBECC		530
#define OPT_ORBMEANMOTION	540
#define OPT_ORBPER		550
#define OPT_ORBSEMI		560

#define OPT_OUTDIGITS           570
#define OPT_OUTPUTORDER		580
#define OPT_OUTSCINOT	        590

#define OPT_PRICONTRRAD         600
#define OPT_PRIFORCEEQSPIN      605 
#define OPT_PRIK2		610
#define OPT_PRILUM	        615
#define OPT_PRIMASS		620
#define OPT_PRIMASSLUM	        625
#define OPT_PRIMASSRAD	        630
#define OPT_PRIMAXLOCKDIFF      635
#define OPT_PRIOBL	        640
#define OPT_PRIQ		645
#define OPT_PRIRAD	        650
#define OPT_PRIRG		655
#define OPT_PRISPINPER	        660
#define OPT_PRISPINRATE	        665
#define OPT_PRITAU		670
#define OPT_PRITEFF		675
#define OPT_PRIVROT             680
#define OPT_PRIWD               685

#define OPT_SECALBEDO           700
#define OPT_SECALBEDOMODEL      705
#define OPT_SECCONTRRAD         710
#define OPT_SECFORCEEQSPIN      715
#define OPT_SECK2               720
#define OPT_SECLUM		725
#define OPT_SECMASS		730
#define OPT_SECMASSRAD	        735
#define OPT_SECMASSLUM          740
#define OPT_SECMAXLOCKDIFF      745
#define OPT_SECOBL	        750
#define OPT_SECQ		755
#define OPT_SECWETQ		760
#define OPT_SECWETTAU           762
#define OPT_SECDRYQ		765
#define OPT_SECDRYTAU           767
#define OPT_SECRAD	        770
#define OPT_SECRG		775
#define OPT_SECSPINPER	        780
#define OPT_SECSPINRATE	        782
#define OPT_SECSYNCECC          785
#define OPT_SECTAU		790
#define OPT_SECTEFF             792
#define OPT_SECVROT             795
#define OPT_SECWET              797

#define OPT_SYSTEMNAME		800

#define OPT_TIDEMODEL		850

#define OPT_UNITANGLE		900
#define OPT_UNITLENGTH		910
#define OPT_UNITMASS		920
#define OPT_UNITTEMP	        930
#define OPT_UNITTIME		940

#define OPT_VERBOSE		999

typedef struct {
    char cParam[NUMOPT][24];
    char cDescr[NUMOPT][256];
    int iType[NUMOPT];
    char cDefault[NUMOPT][24];

} OPTIONS;


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
} INPUT;


