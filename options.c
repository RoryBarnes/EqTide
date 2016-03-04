/**************************************************************************
 * options.c                                                              *
 *                                                                        *
 * Written by Rory Barnes                                                 * 
 *                                                                        *
 * This file contains subroutines related to reading and verifying the    *
 * input options.                                                         *
 *                                                                        *
 **************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include "eqtide.h"
#include "options.h"
#include "output.h"

void ExitMultipleOptions(char infile[],char name[],int line1,int line2,int iExit) {
  fprintf(stderr,"ERROR: Multiple occurences of option %s found.\n",name);
  fprintf(stderr,"\t%s, lines: %d and %d\n",infile,line1,line2);
  exit(iExit);
}

void AddOptionBool(char infile[], char name[], int *param, int *nline, int iExit,int iVerbose) {
  FILE *fp;
  char line[256],word[24],cTmp[24];
  int n=0,done=0,iLen;
  
  iLen=strlen(name);

  fp=fopen(infile,"r");
  if (fp == NULL) {
    fprintf(stderr,"ERROR: Unable to open %s.\n",infile);
    exit(iExit);
  }
  while(fgets(line,256,fp) != NULL) {
    /* Check for # sign */
    if (memcmp(line,"#",1) != 0) {
      /* Check for desired parameter */
      sprintf(word,"null");
      sscanf(line,"%s",word);
      if (memcmp(word,name,iLen) == 0) {
	/* Parameter Found! */
	if (done)
	  ExitMultipleOptions(infile,name,*nline+1,n+1,iExit);
	sscanf(line,"%s %s",cTmp,word);
	*param=atoi(word);
	done=1;
	*nline=n;
      }
    }
    n++;
  }
  if (done) {
    if (*param == 0 || *param == 1) {
      return;
    } else {
      fprintf(stderr,"ERROR: Option %s must be 0 or 1.\n",name);
      LineExit(infile,*nline,iExit,iVerbose);
    }
  }
}

void AddOptionInt(char infile[], char name[], int *param, int *nline, int iExit) {
  FILE *fp;
  char line[256],word[24],cTmp[24];
  int n=0,done=0,iLen;
  
  iLen=strlen(name);

  fp=fopen(infile,"r");
  if (fp == NULL) {
    fprintf(stderr,"ERROR: Unable to open %s.\n",infile);
    exit(iExit);
  }
  while(fgets(line,256,fp) != NULL) {
    /* Check for # sign */
    if (memcmp(line,"#",1) != 0) {
      /* Check for desired parameter */
      sprintf(word,"null");
      sscanf(line,"%s",word);
      if (memcmp(word,name,iLen) == 0) {
	/* Parameter Found! */
	if (done)
	  ExitMultipleOptions(infile,name,*nline+1,n+1,iExit);
	sscanf(line,"%s %s",cTmp,word);
	*param=atoi(word);
	done=1;
	*nline=n;
      }
    }
    n++;
  }
}

void AddOptionDouble(char infile[], char name[], double *param,int *nline, int iExit) {
  FILE *fp;
  char line[256],word[24],cTmp[24];
  int n=0,done=0,iLen;
  
  iLen=strlen(name);

  fp=fopen(infile,"r");
  if (fp == NULL) {
    fprintf(stderr,"ERROR: Unable to open %s.\n",infile);
    exit(iExit);
  }
  while(fgets(line,256,fp) != NULL) {
    /* Check for # sign */
    if (memcmp(line,"#",1) != 0) {
      /* Check for desired parameter */
      sprintf(word,"null");
      sscanf(line,"%s",word);
      if (memcmp(word,name,iLen) == 0) {
	/* Parameter Found! */
	if (done)
	  ExitMultipleOptions(infile,name,*nline+1,n+1,iExit);
	sscanf(line,"%s %s",cTmp,word);
	*param=atof(word);
	done=1;
	*nline=n;
      }
    }
    n++;
  }
}

void AddOptionString(char infile[], char name[],char *param, int *nline, int iExit) {
  FILE *fp;
  char line[256],word[24],cTmp[24];
  int n=0,done=0,iLen;
  
  iLen=strlen(name);

  fp=fopen(infile,"r");
  if (fp == NULL) {
    fprintf(stderr,"ERROR: Unable to open %s.\n",infile);
    exit(iExit);
  }
  while(fgets(line,256,fp) != NULL) {
    /* Check for # sign */
    if (memcmp(line,"#",1) != 0) {
      /* Check for desired parameter */
      sprintf(word,"null");
      sscanf(line,"%s",word);
      if (memcmp(word,name,iLen) == 0) {
	/* Parameter Found! */
	if (done)
	  ExitMultipleOptions(infile,name,*nline+1,n+1,iExit);
	sscanf(line,"%s %s",cTmp,param);
	done=1;
	*nline=n;
      }
    }
    n++;
  }
}

int GetNumOut(char infile[],char name[],int iLen,int *nline,int iExit) {
  char line[256],word[24];
  int i,j,istart,ok,done=0,n=0,num;
  FILE *fp;

  fp=fopen(infile,"r");
  if (fp == NULL) {
    fprintf(stderr,"Unable to open %s.\n",infile);
    exit(iExit);
  }
  while(fgets(line,256,fp) != NULL) {
    /* Check for # sign */
    if (memcmp(line,"#",1) != 0) {
      /* Check for desired parameter */
      sscanf(line,"%s",word);
      if (memcmp(word,name,iLen) == 0) {
	/* Parameter Found! */
	if (done) {
	  fprintf(stderr,"ERROR: Multiple occurences of parameter %s found.\n",name);
	  fprintf(stderr,"\t%s, lines: %d and %d\n",infile,*nline,n);
	  exit(iExit);
	}
	done=1;
	*nline=n;

	num=0;
	istart=0;
	ok=1;
	for (i=1;i<256;i++) { /* Ignore first character, as it makes conditional well-defined */
	  /* printf("%d ",line[i]); */ 
	  if (ok) {
	    if (line[i] == 35) {
	      /* Pound sign! */
	      ok=0;
	      num++;
	    }
	    if (isspace(line[i]) && !isspace(line[i-1]))
	      num++;
	  }
	}
      }
    }
    n++;
    for (i=0;i<256;i++) 
      line[i]=0;
  }
  /* Lose the input parameter */
  num--;
  return num;
}

int AddOutputOrder(char infile[], char name[], int iLen,char param[NUMOUT][24],int *nline,OUTPUT *output,int iVerbose,int iExit) {
  FILE *fp;
  char line[256],word[24],cTmp[24],opt[NUMOUT][24],input[NUMOUT][24],base[NUMOUT][24],out[NUMOUT][24];
  int i=0,count=0,n=0,istart=0,j=0,nl=0,k=0,bNeg=0,iOut=0;
  int foo;

  fp=fopen(infile,"r");
  if (fp == NULL) {
    fprintf(stderr,"Unable to open %s.\n",infile);
    exit(iExit);
  }
  rewind(fp); 

  /* Variables:
     n = Counter of Output; at end is total #
     count = Number of matches to output.cParam
     word = string to search for option in input file
     name = name of option in input file
     input[n] = Verbatim string from input stream
     base[n] = Dummy copy of input, to allow for capitalization manipulation
     output.cParam = Output option string
     cTmp = Dummy copy of output.cParam, to allow for capitalization manipulation
     opt = array of possible output options
     param = Output option with capitalization
     out = Dummy of param
     bNeg = Negative sign in front of option?
     iOut = ID of output struct that matches an output parameter
  */

  while(fgets(line,256,fp) != NULL) {
    /* Check for # sign */
    foo = strlen(line);
    if (memcmp(line,"#",1) != 0) {
      /* Check for desired parameter */
      sprintf(word,"null");
      sscanf(line,"%s",word);
      if (memcmp(word,name,iLen) == 0) { 
        /* Parameter Found! */
        n=0;
        *nline=nl;
        istart=0;
        for (i=0;i<256;i++) {
          /* printf("%d ",line[i]); */ 
          if (isspace(line[i]) || line[i] == 35 || i == strlen(line)-1 ) {
            for (j=0;j<24;j++) 
              input[n][j] = 0;
            
            for (j=0;j<(i-istart);j++) {
              /* printf("%d ",line[istart+j]); */
	      if (line[istart+j] == 45) {
		bNeg = 1;
	      } else if (!iscntrl(line[istart+j]) && line[istart+j] != 35) 
                /* if (line[istart+j] != 10) */ 
                input[n][j-bNeg] = line[istart+j];
            }
	    /* Don't include name of parameter, e.g. OutputOrder */
            if (memcmp(input[n],name,iLen)) {
	      /* Dump string into buffer. This is to maintain
		 capitalization in original input. */
	      for (k=0;k<24;k++) 
		base[n][k]=0;
	      for (k=0;k<24;k++) 
		base[n][k]=input[n][k];
	      
	      /* First check redundancy */
	      for (k=0;k<n;k++) {
		if (memcmp(lower(base[n]),lower(out[k]),(i-istart)) == 0) {
		  /* Match */
		  if (iVerbose >= VERBERR)
		    fprintf(stderr,"ERROR: Same option listed multiple times in %s: %s.\n",name,param[k]);
		  LineExit(infile,nl,iExit,iVerbose);
		}
	      }
	      
	      /* Check for ambiguity */ 
              count=0; /* How many possibilities? */
              for (j=0;j<NUMOUT;j++) {
                for (k=0;k<24;k++)
		  /* Maintain capitalization in output.cParam */
                  cTmp[k]=output->cParam[j][k];
		
                if (memcmp(lower(base[n]),lower(cTmp),(i-istart)) == 0) {
		  /* Valid output option found */
		  for (k=0;k<24;k++) 
		    opt[count][k]=output->cParam[j][k];
		  count++;
		  iOut=j;
                }
              }
	      if (count > 1) {
		/* More than one possibility */
		if (iVerbose >= VERBERR) {
		  fprintf(stderr,"ERROR: Output option \"%s\" is ambiguous. Options are ",input[n]);
		  for (k=0;k<count;k++) {
		    fprintf(stderr,"%s",opt[k]);
		    if (k < count-1) 
		      fprintf(stderr,", ");
		  }
		  fprintf(stderr,".\n");
		}
		LineExit(infile,nl,iExit,iVerbose);
	      }
              if (!count) {
		/* Option not found */
		if (iVerbose >= VERBERR)
		  fprintf(stderr,"ERROR: Unknown option %s listed in %s.\n",input[n],name);
		LineExit(infile,nl,iExit,iVerbose);
              }
	      if (count == 1) {
		/* Unique option */
		for (k=0;k<24;k++) {
		  /* n-1 because n=0 is name */
		  param[n-1][k]=opt[0][k];
		  out[n-1][k]=param[n-1][k];
		}
		output->iNeg[iOut]=bNeg;
		if (bNeg && iVerbose >= VERBUNITS) 
		  fprintf(stderr,"WARNING: Output option %s is negative, output units will be %s\n",param[n-1],output->cNeg[iOut]);
		bNeg=0;
              } 	
            }
            while (isspace(line[i])) i++;
            istart=i;
	    n++; /*?*/
          }
        }
      }
    }
    for (j=0;j<256;j++) line[j]=0;
    nl++;
  }
  
  // Return number of output columns. If no output requested, returns -1
  return n-1;
}

int iGetNumLines(char infile[],int iExit) {
  int n=0;
  FILE *fp;
  char line[256];

  fp=fopen(infile,"r");
  if (fp == NULL) {
    fprintf(stderr,"Unable to open %s.\n",infile);
    exit(iExit);
  }
  while(fgets(line,256,fp) != NULL) {
    n++;
  }
  return n;
}

void InitializeInput(char infile[],int iNumLines,int iLineOK[],int iExit) {
  int n,i,bad;
  FILE *fp;
  char line[256];

  fp=fopen(infile,"r");
  if (fp == NULL) {
    fprintf(stderr,"Unable to open %s.\n",infile);
    exit(iExit);
  }

  for (n=0;n<iNumLines;n++) {
    /* Initialize iLineOK */
    iLineOK[n] = 0;
    
    for (i=0;i<256;i++) line[i]=32;
    fgets(line,256,fp);
    /* Check for # sign or blank line */
    if (!memcmp(line,"#",1)) {
      /* Line is OK */
      iLineOK[n] = 1;
    } else {
      bad=0;
      for (i=0;i<256;i++) {
	if (!isspace(line[i]) && line[i] != '\0') {
	  bad=1;	
	}
      }	
      if (!bad) iLineOK[n] = 1;
    }
  }
}

void InitializeOptions(OPTIONS *options) {
  int i;
  
  for (i=0;i<NUMOPT;i++) 
    sprintf(options->cParam[i],"null");
  
  /* Do System Name first as it is used by other options. */
  
  sprintf(options->cParam[OPT_SYSTEMNAME],"sSystemName");
  sprintf(options->cDescr[OPT_SYSTEMNAME],"System Name");
  sprintf(options->cDefault[OPT_SYSTEMNAME],"None - must be supplied");
  options->iType[OPT_SYSTEMNAME] = 3;
  
  /*
   *
   *   A
   *
   */
  
  sprintf(options->cParam[OPT_AGE],"dAge");
  sprintf(options->cDescr[OPT_AGE],"System Age");
  sprintf(options->cDefault[OPT_AGE],"0");
  options->iType[OPT_AGE] = 2;
  
  /*
   *
   *   B
   *
   */
  
  sprintf(options->cParam[OPT_BACK],"bDoBackward");
  sprintf(options->cDescr[OPT_BACK],"Do Backward Integration?");
  sprintf(options->cDefault[OPT_BACK],"No");
  options->iType[OPT_BACK] = 0;
  
  sprintf(options->cParam[OPT_BACKFILE],"sBackwardFile");
  sprintf(options->cDescr[OPT_BACKFILE],"Name of Backward Integration Output File");
  sprintf(options->cDefault[OPT_BACKFILE],"%s.backward",options->cParam[OPT_SYSTEMNAME]);
  options->iType[OPT_BACKFILE] = 3;
  
  sprintf(options->cParam[OPT_BACKOUTPUTTIME],"dBackwardOutputTime");
  sprintf(options->cDescr[OPT_BACKOUTPUTTIME],"Output Interval for Backward Intgeration");
  sprintf(options->cDefault[OPT_BACKOUTPUTTIME],"1");
  options->iType[OPT_BACKOUTPUTTIME] = 2;
  
  sprintf(options->cParam[OPT_BACKSTOPTIME],"dBackwardStopTime");
  sprintf(options->cDescr[OPT_BACKSTOPTIME],"Stop Time for Backward Integration");
  sprintf(options->cDefault[OPT_BACKSTOPTIME],"10");
  options->iType[OPT_BACKSTOPTIME] = 2;

  sprintf(options->cParam[OPT_BACKTIMESTEP],"dBackwardTimeStep");
  sprintf(options->cDescr[OPT_BACKTIMESTEP],"Timestep for Backward Integration");
  sprintf(options->cDefault[OPT_BACKTIMESTEP],"1");
  options->iType[OPT_BACKTIMESTEP] = 2;
  
  /*
   *
   *   D
   *
   */
  
  sprintf(options->cParam[OPT_DISCRETEROT],"bDiscreteRot");
  sprintf(options->cDescr[OPT_DISCRETEROT],"Use Discrete Rotation Model (Phase lag only)");
  sprintf(options->cDefault[OPT_DISCRETEROT],"1");
  options->iType[OPT_DISCRETEROT] = 0;
  
  /*
   *   FORWARD
   *
   */
  
  sprintf(options->cParam[OPT_FORW],"bDoForward");
  sprintf(options->cDescr[OPT_FORW],"Do Forward Integration?");
  sprintf(options->cDefault[OPT_FORW],"0");
  options->iType[OPT_FORW] = 0;
  
  sprintf(options->cParam[OPT_FORWFILE],"sForwardFile");
  sprintf(options->cDescr[OPT_FORWFILE],"Name of Forward Integration Output File");
  sprintf(options->cDefault[OPT_FORWFILE],"%s.forward",options->cParam[OPT_SYSTEMNAME]);
  options->iType[OPT_FORWFILE] = 3;
  
  sprintf(options->cParam[OPT_FORWOUTPUTTIME],"dForwardOutputTime");
  sprintf(options->cDescr[OPT_FORWOUTPUTTIME],"Output Interval for Forward Integration");
  sprintf(options->cDefault[OPT_FORWOUTPUTTIME],"1");
  options->iType[OPT_FORWOUTPUTTIME] = 2;
  
  sprintf(options->cParam[OPT_FORWSTOPTIME],"dForwardStopTime");
  sprintf(options->cDescr[OPT_FORWSTOPTIME],"Stop Time for Forward Integration");
  sprintf(options->cDefault[OPT_FORWSTOPTIME],"10");
  options->iType[OPT_FORWSTOPTIME] = 2;
  
  sprintf(options->cParam[OPT_FORWTIMESTEP],"dForwardTimeStep");
  sprintf(options->cDescr[OPT_FORWTIMESTEP],"Timestep for Forward Integration");
  sprintf(options->cDefault[OPT_FORWTIMESTEP],"1");
  options->iType[OPT_FORWTIMESTEP] = 2;
  
  /* 
   *
   *   HALT
   *
   */
  
  sprintf(options->cParam[OPT_HALTDBLSYNC],"bHaltDblSync");
  sprintf(options->cDescr[OPT_HALTDBLSYNC],"Halt at Double Synchronous State?");
  sprintf(options->cDefault[OPT_HALTDBLSYNC],"0");
  options->iType[OPT_HALTDBLSYNC] = 0;
  
  sprintf(options->cParam[OPT_HALTMAXECC],"dHaltMaxEcc");
  sprintf(options->cDescr[OPT_HALTMAXECC],"Maximum Eccentricity Value that Halts Integration");
  sprintf(options->cDefault[OPT_HALTMAXECC],"1");
  options->iType[OPT_HALTMAXECC] = 2;
  
  sprintf(options->cParam[OPT_HALTMERGE],"bHaltMerge");
  sprintf(options->cDescr[OPT_HALTMERGE],"Halt at Merge");
  sprintf(options->cDefault[OPT_HALTMERGE],"1");
  options->iType[OPT_HALTMERGE] = 0;
  
  sprintf(options->cParam[OPT_HALTMINECC],"dHaltMinEcc");
  sprintf(options->cDescr[OPT_HALTMINECC],"Minimum Eccentricity Value that Halts Integration");
  sprintf(options->cDefault[OPT_HALTMINECC],"0");
  options->iType[OPT_HALTMINECC] = 2;
  
  sprintf(options->cParam[OPT_HALTMINPRIOBL],"dHaltMinPriObl");
  sprintf(options->cDescr[OPT_HALTMINPRIOBL],"Primary's Minimum Obliquity Value that Halts Integration");
  sprintf(options->cDefault[OPT_HALTMINPRIOBL],"0");
  options->iType[OPT_HALTMINPRIOBL] = 2;
  
  sprintf(options->cParam[OPT_HALTMINSECOBL],"dHaltMinSecObl");
  sprintf(options->cDescr[OPT_HALTMINSECOBL],"Secondary's Minimum Obliquity Value that Halts Integration");
  sprintf(options->cDefault[OPT_HALTMINSECOBL],"0");
  options->iType[OPT_HALTMINSECOBL] = 2;
  
  sprintf(options->cParam[OPT_HALTMINSEMI],"dHaltMinSemi");
  sprintf(options->cDescr[OPT_HALTMINSEMI],"Minimum Semi-Major Axis Value that Halts Integration");
  sprintf(options->cDefault[OPT_HALTMINSEMI],"0");
  options->iType[OPT_HALTMINSEMI] = 2;
  
  sprintf(options->cParam[OPT_HALTPOSDEDT],"bHaltPosDeDt");
  sprintf(options->cDescr[OPT_HALTPOSDEDT],"Halt if de/dt > 0?");
  sprintf(options->cDefault[OPT_HALTPOSDEDT],"0");
  options->iType[OPT_HALTPOSDEDT] = 0;
  
  sprintf(options->cParam[OPT_HALTPRILOCK],"bHaltPriLock");
  sprintf(options->cDescr[OPT_HALTPRILOCK],"Halt if Primary Becomes Tide-Locked?");
  sprintf(options->cDefault[OPT_HALTPRILOCK],"0");
  options->iType[OPT_HALTPRILOCK] = 0;
  
  sprintf(options->cParam[OPT_HALTSECLOCK],"bHaltSecLock");
  sprintf(options->cDescr[OPT_HALTSECLOCK],"Halt if Secondary Becomes Tide-Locked?");
  sprintf(options->cDefault[OPT_HALTSECLOCK],"0");
  options->iType[OPT_HALTSECLOCK] = 0;
  
  sprintf(options->cParam[OPT_HALTSECSYNC],"bHaltSecSync");
  sprintf(options->cDescr[OPT_HALTSECSYNC],"Halt if Secondary's rotation becomes syncrhonous?");
  sprintf(options->cDefault[OPT_HALTSECSYNC],"0");
  options->iType[OPT_HALTSECSYNC] = 0;
  
  /*
   *
   * I
   *
   */

  sprintf(options->cParam[OPT_INTEGRATION],"sIntegration");
  sprintf(options->cDescr[OPT_INTEGRATION],"Integration method (Euler or RungeKutta4");
  sprintf(options->cDefault[OPT_INTEGRATION],"RungeKutta4");
  options->iType[OPT_INTEGRATION] = 3;
  
  /*
   *
   *   L
   *
   */
  
  sprintf(options->cParam[OPT_LOG],"bDoLog");
  sprintf(options->cDescr[OPT_LOG],"Write Log File?");
  sprintf(options->cDefault[OPT_LOG],"0");
  options->iType[OPT_LOG] = 0;
  
  sprintf(options->cParam[OPT_LOGFILE],"sLogFile");
  sprintf(options->cDescr[OPT_LOGFILE],"Log File Name");
  sprintf(options->cDefault[OPT_LOGFILE],"tide.log");
  options->iType[OPT_LOGFILE] = 3;
  
  /*
   *
   *   M
   *
   */
  
  sprintf(options->cParam[OPT_MINVALUE],"dMinValue");
  sprintf(options->cDescr[OPT_MINVALUE],"Minimum Non-Zero Value of Eccentricity and Obliquities");
  sprintf(options->cDefault[OPT_MINVALUE],"0");
  options->iType[OPT_MINVALUE] = 2;
  
  /*
   *
   *   O
   *
   */
  
  sprintf(options->cParam[OPT_OUTDIGITS],"iDigits");
  sprintf(options->cDescr[OPT_OUTDIGITS],"Number of Digits After Decimal Point");
  sprintf(options->cDefault[OPT_OUTDIGITS],"4");
  options->iType[OPT_OUTDIGITS] = 1;
  
  sprintf(options->cParam[OPT_OUTPUTORDER],"sOutputOrder");
  sprintf(options->cDescr[OPT_OUTPUTORDER],"Output Parameter(s)");
  sprintf(options->cDefault[OPT_OUTPUTORDER],"None");
  options->iType[OPT_OUTPUTORDER] = 4;
  
  sprintf(options->cParam[OPT_OUTSCINOT],"iSciNot");
  sprintf(options->cDescr[OPT_OUTSCINOT],"Logarithm to Change from Standard to Scientific Notation");
  sprintf(options->cDefault[OPT_OUTSCINOT],"4");
  options->iType[OPT_OUTSCINOT] = 1;
  
  /*
   *
   *   ORBIT
   *
   */
  
  sprintf(options->cParam[OPT_ORBECC],"dEcc");
  sprintf(options->cDescr[OPT_ORBECC],"Orbital Eccentricity");
  sprintf(options->cDefault[OPT_ORBECC],"0");
  options->iType[OPT_ORBECC] = 2;
  
  sprintf(options->cParam[OPT_ORBMEANMOTION],"dMeanMotion");
  sprintf(options->cDescr[OPT_ORBMEANMOTION],"Orbital Mean Motion");
  sprintf(options->cDefault[OPT_ORBMEANMOTION],"1");
  options->iType[OPT_ORBMEANMOTION] = 2;
  
  sprintf(options->cParam[OPT_ORBPER],"dPeriod");
  sprintf(options->cDescr[OPT_ORBPER],"Orbital Period");
  sprintf(options->cDefault[OPT_ORBPER],"1");
  options->iType[OPT_ORBPER] = 2;
  
  sprintf(options->cParam[OPT_ORBSEMI],"dSemi");
  sprintf(options->cDescr[OPT_ORBSEMI],"Semi-Major Axis");
  sprintf(options->cDefault[OPT_ORBSEMI],"1");
  options->iType[OPT_ORBSEMI] = 2;
  
  /*
   *
   *   P
   *
   */
  
  sprintf(options->cParam[OPT_PRIFORCEEQSPIN],"bPrimaryForceEqSpin");
  sprintf(options->cDescr[OPT_PRIFORCEEQSPIN],"Force Primary's Spin Rate to Equilibrium");
  sprintf(options->cDefault[OPT_PRIFORCEEQSPIN],"0");
  options->iType[OPT_PRIFORCEEQSPIN] = 0;
  
  sprintf(options->cParam[OPT_PRIK2],"dPrimaryK2");
  sprintf(options->cDescr[OPT_PRIK2],"Love Number of Degree 2 of Primary");
  sprintf(options->cDefault[OPT_PRIK2],"1");
  options->iType[OPT_PRIK2] = 2;
  
  sprintf(options->cParam[OPT_PRIMASS],"dPrimaryMass");
  sprintf(options->cDescr[OPT_PRIMASS],"Mass of Primary");
  sprintf(options->cDefault[OPT_PRIMASS],"1");
  options->iType[OPT_PRIMASS] = 2;
  
  sprintf(options->cParam[OPT_PRIMASSRAD],"sPrimaryMassRad");
  sprintf(options->cDescr[OPT_PRIMASSRAD],"Mass-Radius Relationship for Central Body: GS99 RH00 BO06 Sotin07 ");
  sprintf(options->cDefault[OPT_PRIMASSRAD],"None");
  options->iType[OPT_PRIMASSRAD] = 3;
  
  sprintf(options->cParam[OPT_PRIMAXLOCKDIFF],"dPrimaryMaxLockDiff");
  sprintf(options->cDescr[OPT_PRIMAXLOCKDIFF],"Maximum relative difference between primary's spin and equilibrium spin rates to force equilibrium spin rate");
  sprintf(options->cDefault[OPT_PRIMAXLOCKDIFF],"0");
  options->iType[OPT_PRIMAXLOCKDIFF] = 3;
  
  sprintf(options->cParam[OPT_PRIOBL],"dPrimaryObliquity");
  sprintf(options->cDescr[OPT_PRIOBL],"Obliquity of Primary");
  sprintf(options->cDefault[OPT_PRIOBL],"0");
  options->iType[OPT_PRIOBL] = 2;
  
  sprintf(options->cParam[OPT_PRIQ],"dPrimaryQ");
  sprintf(options->cDescr[OPT_PRIQ],"Tidal Quality Factor of Primary");
  sprintf(options->cDefault[OPT_PRIQ],"1e6");
  options->iType[OPT_PRIQ] = 2;
  
  sprintf(options->cParam[OPT_PRIRAD],"dPrimaryRadius");
  sprintf(options->cDescr[OPT_PRIRAD],"Radius of Primary");
  sprintf(options->cDefault[OPT_PRIRAD],"1");
  options->iType[OPT_PRIRAD] = 2;
  
  sprintf(options->cParam[OPT_PRIRG],"dPrimaryRadGyra");
  sprintf(options->cDescr[OPT_PRIRG],"Radius of Gyration of Primary");
  sprintf(options->cDefault[OPT_PRIRG],"0.5");
  options->iType[OPT_PRIRG] = 2;
  
  sprintf(options->cParam[OPT_PRISPINPER],"dPrimarySpinPeriod");
  sprintf(options->cDescr[OPT_PRISPINPER],"Rotation Period of Primary");
  sprintf(options->cDefault[OPT_PRISPINPER],"1");
  options->iType[OPT_PRISPINPER] = 2;
  
  sprintf(options->cParam[OPT_PRISPINRATE],"dPrimarySpinRate");
  sprintf(options->cDescr[OPT_PRISPINRATE],"Rotational Angular Frequency of Primary");
  sprintf(options->cDefault[OPT_PRISPINRATE],"1");
  options->iType[OPT_PRISPINRATE] = 2;
  
  sprintf(options->cParam[OPT_PRITAU],"dPrimaryTau");
  sprintf(options->cDescr[OPT_PRITAU],"Time Lag of Primary");
  sprintf(options->cDefault[OPT_PRITAU],"1");
  options->iType[OPT_PRITAU] = 2;
  
  sprintf(options->cParam[OPT_PRIVROT],"dPrimaryRotVel");
  sprintf(options->cDescr[OPT_PRIVROT],"Rotational Velocity of Primary");
  sprintf(options->cDefault[OPT_PRIVROT],"None");
  options->iType[OPT_PRIVROT] = 2;
  
  /*
   *
   *   S
   *
   */
  
  sprintf(options->cParam[OPT_SECK2],"dSecondaryK2");
  sprintf(options->cDescr[OPT_SECK2],"Love Number of Degree 2 of Secondary");
  sprintf(options->cDefault[OPT_SECK2],"1");
  options->iType[OPT_SECK2] = 2;
  
  sprintf(options->cParam[OPT_SECK2],"dSecondaryK2");
  sprintf(options->cDescr[OPT_SECK2],"Love Number of Degree 2 of Secondary");
  sprintf(options->cDefault[OPT_SECK2],"1");
  options->iType[OPT_SECK2] = 2;
  
  sprintf(options->cParam[OPT_SECFORCEEQSPIN],"bSecondaryForceEqSpin");
  sprintf(options->cDescr[OPT_SECFORCEEQSPIN],"Force Secondary's Spin Rate to Equilibrium");
  sprintf(options->cDefault[OPT_SECFORCEEQSPIN],"0");
  options->iType[OPT_SECFORCEEQSPIN] = 0;
  
  sprintf(options->cParam[OPT_SECMASS],"dSecondaryMass");
  sprintf(options->cDescr[OPT_SECMASS],"Mass of Secondary");
  sprintf(options->cDefault[OPT_SECMASS],"1");
  options->iType[OPT_SECMASS] = 2;
  
  sprintf(options->cParam[OPT_SECMASSRAD],"sSecondaryMassRad");
  sprintf(options->cDescr[OPT_SECMASSRAD],"Mass-Radius Relationship for Secondary: GS99 RH00 BO06 Sotin07");
  sprintf(options->cDefault[OPT_SECMASSRAD],"None");
  options->iType[OPT_SECMASSRAD] = 3;
  
  sprintf(options->cParam[OPT_SECMAXLOCKDIFF],"dSecondaryMaxLockDiff");
  sprintf(options->cDescr[OPT_SECMAXLOCKDIFF],"Maximum relative difference between secondary's spin and equilibrium spin rates to force equilibrium spin rate");
  sprintf(options->cDefault[OPT_SECMAXLOCKDIFF],"0");
  options->iType[OPT_SECMAXLOCKDIFF] = 3;
  
  sprintf(options->cParam[OPT_SECOBL],"dSecondaryObl");
  sprintf(options->cDescr[OPT_SECOBL],"Obliquity of Secondary");
  sprintf(options->cDefault[OPT_SECOBL],"0");
  options->iType[OPT_SECOBL] = 2;
  
  sprintf(options->cParam[OPT_SECQ],"dSecondaryQ");
  sprintf(options->cDescr[OPT_SECQ],"Tidal Quality Factor of Secondary");
  sprintf(options->cDefault[OPT_SECQ],"100");
  options->iType[OPT_SECQ] = 2;

  sprintf(options->cParam[OPT_SECRAD],"dSecondaryRadius");
  sprintf(options->cDescr[OPT_SECRAD],"Radius of Secondary");
  sprintf(options->cDefault[OPT_SECRAD],"1");
  options->iType[OPT_SECRAD] = 2;
  
  sprintf(options->cParam[OPT_SECRG],"dSecondaryRadGyra");
  sprintf(options->cDescr[OPT_SECRG],"Radius of Gyration of Secondary");
  sprintf(options->cDefault[OPT_SECRG],"0.5");
  options->iType[OPT_SECRG] = 2;
  
  sprintf(options->cParam[OPT_SECSPINPER],"dSecondarySpinPer");
  sprintf(options->cDescr[OPT_SECSPINPER],"Rotational Period of Secondary");
  sprintf(options->cDefault[OPT_SECSPINPER],"0");
  options->iType[OPT_SECSPINPER] = 2;
  
  sprintf(options->cParam[OPT_SECSPINRATE],"dSecondarySpinRate");
  sprintf(options->cDescr[OPT_SECSPINRATE],"Rotational Angular Frequency of Secondary");
  sprintf(options->cDefault[OPT_SECSPINRATE],"0");
  options->iType[OPT_SECSPINRATE] = 2;
  
  sprintf(options->cParam[OPT_SECSYNCECC],"dSecondarySyncEcc");
  sprintf(options->cDescr[OPT_SECSYNCECC],"Secondary's Rotation is synchronous below this value");
  sprintf(options->cDefault[OPT_SECSYNCECC],"0");
  options->iType[OPT_SECSYNCECC] = 2;
  
  sprintf(options->cParam[OPT_SECTAU],"dSecondaryTau");
  sprintf(options->cDescr[OPT_SECTAU],"Time Lag for Secondary");
  sprintf(options->cDefault[OPT_SECTAU],"1");
  options->iType[OPT_SECTAU] = 2;
  
  sprintf(options->cParam[OPT_SECVROT],"dSecondaryRotVel");
  sprintf(options->cDescr[OPT_SECVROT],"Rotational Velocity of Secondary");
  sprintf(options->cDefault[OPT_SECVROT],"1");
  options->iType[OPT_SECVROT] = 2;
  
  /*
   *
   *   T
   *
   */
  
  sprintf(options->cParam[OPT_TIMESTEPCOEFF],"dTimestepCoeff");
  sprintf(options->cDescr[OPT_TIMESTEPCOEFF],"Coefficient to scale the timestep");
  sprintf(options->cDefault[OPT_TIMESTEPCOEFF],"1");
  options->iType[OPT_TIMESTEPCOEFF] = 2;
  
  sprintf(options->cParam[OPT_TIDEMODEL],"sTideModel");
  sprintf(options->cDescr[OPT_TIDEMODEL],"Tidal Model: p2 [constant-phase-lag, 2nd order] t8 [constant-time-lag, 8th order]");
  sprintf(options->cDefault[OPT_TIDEMODEL],"p2");
  options->iType[OPT_TIDEMODEL] = 3;
  
  /*
   *
   *   UNITS
   *
   */
  
  sprintf(options->cParam[OPT_UNITANGLE],"sUnitAngle");
  sprintf(options->cDescr[OPT_UNITANGLE],"Angle Units: Degrees Radians");
  sprintf(options->cDefault[OPT_UNITANGLE],"Radians");
  options->iType[OPT_UNITANGLE] = 3;
  
  sprintf(options->cParam[OPT_UNITLENGTH],"sUnitLength");
  sprintf(options->cDescr[OPT_UNITLENGTH],"Length Units: cm m km Solar Earth Jupiter AU");
  sprintf(options->cDefault[OPT_UNITLENGTH],"cm");
  options->iType[OPT_UNITLENGTH] = 3;
  
  sprintf(options->cParam[OPT_UNITMASS],"sUnitMass");
  sprintf(options->cDescr[OPT_UNITMASS],"Mass Units: Grams Kilograms Solar Earth Jupiter Saturn");
  sprintf(options->cDefault[OPT_UNITMASS],"grams");
  options->iType[OPT_UNITMASS] = 3;
  
  sprintf(options->cParam[OPT_UNITTIME],"sUnitTime");
  sprintf(options->cDescr[OPT_UNITTIME],"Time Units: Seconds, Days Years Myr Gyr");
  sprintf(options->cDefault[OPT_UNITTIME],"Seconds");
  options->iType[OPT_UNITTIME] = 3;
  
  /*
   *
   *   V
   *
   */
  
  
  sprintf(options->cParam[OPT_VARDT],"bVarDt");
  sprintf(options->cDescr[OPT_VARDT],"Use variable timestep for Backward Integration?");
  sprintf(options->cDefault[OPT_VARDT],"No");
  options->iType[OPT_VARDT] = 0;

  sprintf(options->cParam[OPT_VERBOSE],"iVerbose");
  sprintf(options->cDescr[OPT_VERBOSE],"Verbosity Level: 1-5");
  sprintf(options->cDefault[OPT_VERBOSE],"3");
  options->iType[OPT_VERBOSE] = 1;
  
}


void Unrecognized(char infile[],int iNumLines,int *iLineOK,int iExit) {
  FILE *fp;
  char line[256],word[16];
   /* foo is a placeholder for calls to EqSpinRate_CTL */
  int i,iBad=0,foo; /* Assume don't exit */

  fp=fopen(infile,"r");

  for (i=0;i<iNumLines;i++) {
    fgets(line,256,fp);	  
    if (!iLineOK[i]) {
      /* Bad line */
      sscanf(line,"%s",word);	
      fprintf(stderr,"ERROR: Unrecognized parameter \"%s\" in %s, line %d.\n",word,infile,i+1);
      iBad++;
    }
  }
  if (iBad) {
    fprintf(stderr,"%d bad line(s) in %s.\n",iBad,infile);
    exit(iExit);
  }
}

void VerifyOptions(PARAM *param,PRIMARY *pri,SECONDARY *sec,INPUT input,OPTIONS options,LINE line,char infile[],IO *io) {
   /* foo is a placeholder for calls to EqSpinRate_CTL */
  int foo;

  if (sec->dSemi == 0 && sec->dMeanMotion == 0 && input.dPeriod == 0) {
    fprintf(stderr,"ERROR: dSemi, dMeanMotion, or dPeriod are not set.\n");
    exit(io->exit_param);
  }
  if (sec->dSemi != 0 && sec->dMeanMotion != 0 && input.dPeriod != 0) {
    fprintf(stderr,"ERROR: dSemi (line %d), dMeanMotion (line %d) and dPeriod (line %d) are all set! Only one is allowed.\n",line.lSemi,line.lMeanMotion,line.lPeriod);
    exit(io->exit_param);
  }
  if (sec->dSemi != 0 && sec->dMeanMotion != 0) {
    fprintf(stderr,"ERROR: dSemi (line %d), dMeanMotion (line %d) are both set! Only one is allowed.\n",line.lSemi,line.lMeanMotion);
    exit(io->exit_param);
  }
  if (sec->dSemi != 0 && input.dPeriod != 0) {
    fprintf(stderr,"ERROR: dSemi (line %d) and dPeriod (line %d) are both set! Only one is allowed.\n",line.lSemi,line.lPeriod);
    exit(io->exit_param);
  }
  if (sec->dMeanMotion != 0 && input.dPeriod != 0) {
    fprintf(stderr,"ERROR: dMeanMotion (line %d) and dPeriod (line %d) are all set! Only one is allowed.\n",line.lMeanMotion,line.lPeriod);
    exit(io->exit_param);
  }

  /* 
   * Only one of semi-major axis, period and mean motion are set, now 
   * assign dSemi and dMeanMotion. 
   */

  if (sec->dSemi != 0) {
    input.dPeriod=a2p(sec->dSemi,(sec->dMass + pri->dMass));
    sec->dMeanMotion=dPerToFreq(input.dPeriod);
  } else if (sec->dMeanMotion != 0) {
    input.dPeriod=dFreqToPer(sec->dMeanMotion);
    sec->dSemi=p2a(input.dPeriod,(sec->dMass + pri->dMass));
  } else if (input.dPeriod != 0) {
    sec->dMeanMotion = dPerToFreq(input.dPeriod);
    sec->dSemi = p2a(input.dPeriod,(pri->dMass+sec->dMass));
  }

  /*
   * Primary Mass and Radius
   */
  
  if (pri->dMass == 0) {
    if (pri->dRadius > 0 && pri->iMassRad > 0) {
      if (pri->iMassRad == 1)
	pri->dMass=dReidHawley_RadMass(pri->dRadius);
      if (pri->iMassRad == 2)
	pri->dMass=dGordaSvech99_RadMass(pri->dRadius);
      if (pri->iMassRad == 3)
	pri->dMass=dBaylessOrosz06_RadMass(pri->dRadius);
      if (pri->iMassRad == 4)
	pri->dMass=dSotin07_RadMass(pri->dRadius);
    } else {
      fprintf(stderr,"ERROR: %s not set, so must set %s and %s.\n",options.cParam[OPT_PRIMASS],options.cParam[OPT_PRIRAD],options.cParam[OPT_PRIMASSRAD]);
      exit(EXIT_PARAM);
    }
  }
      
  if (pri->dRadius == 0) {
    if (pri->dMass > 0 && pri->iMassRad > 0) {
      if (pri->iMassRad == 1)
	pri->dRadius=dReidHawley_MassRad(pri->dMass);
      if (pri->iMassRad == 2)
	pri->dRadius=dGordaSvech99_MassRad(pri->dMass);
      if (pri->iMassRad == 3)
	pri->dRadius=dBaylessOrosz06_MassRad(pri->dMass);
      if (pri->iMassRad == 4)
	pri->dRadius=dSotin07_MassRad(pri->dMass);
      if (pri->iMassRad == 5)
	pri->dRadius=dBaraffe15_MassRad(pri->dMass);
    } else {
      fprintf(stderr,"ERROR: %s not set, so must set %s and %s.\n",options.cParam[OPT_PRIMASS],options.cParam[OPT_PRIRAD],options.cParam[OPT_PRIMASSRAD]);
      exit(EXIT_PARAM);
    }
  }
    
  /*
   * Secondary Mass and Radius
   */
  
  if (sec->dMass == 0) {
    if (sec->dRadius > 0 && sec->iMassRad > 0) {
      if (sec->iMassRad == 1)
	sec->dMass=dReidHawley_RadMass(sec->dRadius);
      if (sec->iMassRad == 2)
	sec->dMass=dGordaSvech99_RadMass(sec->dRadius);
      if (sec->iMassRad == 3)
	sec->dMass=dBaylessOrosz06_RadMass(sec->dRadius);
      if (sec->iMassRad == 4)
	sec->dMass=dSotin07_RadMass(sec->dRadius);
    } else {
      fprintf(stderr,"ERROR: %s not set, so must set %s and %s.\n",options.cParam[OPT_SECMASS],options.cParam[OPT_SECRAD],options.cParam[OPT_SECMASSRAD]);
      exit(EXIT_PARAM);
    }
  }
      
  if (sec->dRadius == 0) {
    if (sec->dMass > 0 && sec->iMassRad > 0) {
      if (sec->iMassRad == 1)
	sec->dRadius=dReidHawley_MassRad(sec->dMass);
      if (sec->iMassRad == 2)
	sec->dRadius=dGordaSvech99_MassRad(sec->dMass);
      if (sec->iMassRad == 3)
	sec->dRadius=dBaylessOrosz06_MassRad(sec->dMass);
      if (sec->iMassRad == 4)
	sec->dRadius=dSotin07_MassRad(sec->dMass);
    } else {
      fprintf(stderr,"ERROR: %s not set, so must set %s and %s.\n",options.cParam[OPT_SECMASS],options.cParam[OPT_SECRAD],options.cParam[OPT_SECMASSRAD]);
      exit(EXIT_PARAM);
    }
  }
    
  /* 
   * Primary Spin Properties
   */

  /* Was either spin period set to equilibrium AND specified in options? */

  if (pri->bForceEqSpin) {
    if (pri->dSpinRate != 0) 
      fprintf(stderr,"WARNING: %s and %s set, defaulting to equilibrium value.\n",options.cParam[OPT_PRIFORCEEQSPIN],options.cParam[OPT_PRISPINRATE]);
    if (input.dPriSpinPer != 0) 
      fprintf(stderr,"WARNING: %s and %s set, defaulting to equilibrium value.\n",options.cParam[OPT_PRIFORCEEQSPIN],options.cParam[OPT_PRISPINPER]);
    if (input.dPriVRot != 0) 
      fprintf(stderr,"WARNING: %s and %s set, defaulting to equilibrium value.\n",options.cParam[OPT_PRIFORCEEQSPIN],options.cParam[OPT_PRIVROT]);

    if (param->iTideModel == CPL) {
      pri->dSpinRate = EqSpinRate_CPL2(sec->dMeanMotion,sec->dEcc,sec->dObliquity,param->bDiscreteRot);
    } else if (param->iTideModel == CTL) {
      pri->dSpinRate = EqSpinRate_CTL8(sec->dMeanMotion,sec->dEcc,pri->dObliquity,param->bDiscreteRot);
    }
  } else {

    /* Do consistency checks */

    if (pri->dSpinRate != 0 && input.dPriSpinPer != 0 && input.dPriVRot != 0) {
      fprintf(stderr,"ERROR: %s (line %d), %s (line %d) and %s (line %d) are set. Only one is allowed.\n",options.cParam[OPT_PRISPINPER],line.lPriSpinPer,options.cParam[OPT_PRISPINRATE],line.lPriSpinRate,options.cParam[OPT_PRIVROT],line.lPriVRot);
      exit(io->exit_param);
    }

    if (pri->dSpinRate != 0 && input.dPriSpinPer != 0) {
      fprintf(stderr,"ERROR: %s (line %d), and %s (line %d) are set. Only one is allowed.\n",options.cParam[OPT_PRISPINPER],line.lPriSpinPer,options.cParam[OPT_PRISPINRATE],line.lPriSpinRate);
      exit(io->exit_param);
    }

    if (pri->dSpinRate != 0 && input.dPriVRot != 0) {
      fprintf(stderr,"ERROR: %s (line %d) and %s (line %d) are set. Only one is allowed.\n",options.cParam[OPT_PRISPINRATE],line.lPriSpinRate,options.cParam[OPT_PRIVROT],line.lPriVRot);
      exit(io->exit_param);
    }

    if (input.dPriSpinPer != 0 && input.dPriVRot != 0) {
       fprintf(stderr,"ERROR: %s (line %d) and %s (line %d) are set. Only one is allowed.\n",options.cParam[OPT_PRISPINPER],line.lPriSpinPer,options.cParam[OPT_PRIVROT],line.lPriVRot);
      exit(io->exit_param);
    }

    if (input.dPriSpinPer == 0 && pri->dSpinRate == 0 && input.dPriVRot == 0) {
      /* If neither set, assign the equilibrium rate */
      if (io->iVerbose > 1)
	fprintf(stderr,"WARNING: Neither %s, %s nor %s set, assigning equilibrium rotation rate.\n",options.cParam[OPT_PRISPINPER],options.cParam[OPT_PRISPINRATE],options.cParam[OPT_PRIVROT]);

      if (param->iTideModel == CPL) {
	pri->dSpinRate=EqSpinRate_CPL2(sec->dMeanMotion,sec->dEcc,pri->dObliquity,param->bDiscreteRot);
      } else if (param->iTideModel == CTL) {
	pri->dSpinRate=EqSpinRate_CTL8(sec->dMeanMotion,sec->dEcc,pri->dObliquity,param->bDiscreteRot);
      }
    } else {
      if (input.dPriSpinPer != 0) 
	pri->dSpinRate = dPerToFreq(input.dPriSpinPer);
      if (input.dPriVRot != 0)
	pri->dSpinRate = input.dPriVRot/(2*PI*pri->dRadius);
    }
  }

  /* 
   * Secondary Spin Properties
   *
   */

  if (sec->bForceEqSpin) {
    if (io->iVerbose >= VERBINPUT) {
      if (sec->dSpinRate != 0) 
	fprintf(stderr,"WARNING: %s and %s set, defaulting to equilibrium value.\n",options.cParam[OPT_SECFORCEEQSPIN],options.cParam[OPT_SECSPINRATE]);
      if (input.dSecSpinPer != 0) 
	fprintf(stderr,"WARNING: %s and %s set, defaulting to equilibrium value.\n",options.cParam[OPT_SECFORCEEQSPIN],options.cParam[OPT_SECSPINPER]);
      if (input.dSecVRot != 0) 
	fprintf(stderr,"WARNING: %s and %s set, defaulting to equilibrium value.\n",options.cParam[OPT_SECFORCEEQSPIN],options.cParam[OPT_SECVROT]);
    }

    if (param->iTideModel == CPL) {
      sec->dSpinRate = EqSpinRate_CPL2(sec->dMeanMotion,sec->dEcc,sec->dObliquity,param->bDiscreteRot);
    } else if (param->iTideModel == CTL) {
      sec->dSpinRate = EqSpinRate_CTL8(sec->dMeanMotion,sec->dEcc,sec->dObliquity,param->bDiscreteRot);
    }
  } else {

    /* Do consistency checks */

    if (sec->dSpinRate != 0 && input.dSecSpinPer != 0 && input.dSecVRot != 0) {
      fprintf(stderr,"ERROR: %s (line %d), %s (line %d) and %s (line %d) are set. Only one is allowed.\n",options.cParam[OPT_SECSPINPER],line.lSecSpinPer,options.cParam[OPT_SECSPINRATE],line.lSecSpinRate,options.cParam[OPT_SECVROT],line.lSecVRot);
      exit(io->exit_param);
    }
    
    if (sec->dSpinRate != 0 && input.dSecSpinPer != 0) {
      fprintf(stderr,"ERROR: %s (line %d), and %s (line %d) are set. Only one is allowed.\n",options.cParam[OPT_SECSPINPER],line.lSecSpinPer,options.cParam[OPT_SECSPINRATE],line.lSecSpinRate);
      exit(io->exit_param);
    }
    
    if (sec->dSpinRate != 0 && input.dSecVRot != 0) {
      fprintf(stderr,"ERROR: %s (line %d) and %s (line %d) are set. Only one is allowed.\n",options.cParam[OPT_SECSPINRATE],line.lSecSpinRate,options.cParam[OPT_SECVROT],line.lSecVRot);
      exit(io->exit_param);
    }
    
    if (input.dSecSpinPer != 0 && input.dSecVRot != 0) {
      fprintf(stderr,"ERROR: %s (line %d) and %s (line %d) are set. Only one is allowed.\n",options.cParam[OPT_SECSPINPER],line.lSecSpinPer,options.cParam[OPT_SECVROT],line.lSecVRot);
      exit(io->exit_param);
    }
    
    if (input.dSecSpinPer == 0 && sec->dSpinRate == 0 && input.dSecVRot == 0) {
      /* If neither set, assign the equilibrium rate */
      if (io->iVerbose > 1)
	fprintf(stderr,"WARNING: Neither %s, %s nor %s set, assigning equilibrium rotation rate.\n",options.cParam[OPT_SECSPINPER],options.cParam[OPT_SECSPINRATE],options.cParam[OPT_SECVROT]);
      
      if (param->iTideModel == CPL) {
	pri->dSpinRate=EqSpinRate_CPL2(sec->dMeanMotion,sec->dEcc,pri->dObliquity,param->bDiscreteRot);
      } else if (param->iTideModel == CTL) {
	pri->dSpinRate=EqSpinRate_CTL8(sec->dMeanMotion,sec->dEcc,pri->dObliquity,param->bDiscreteRot);
      }
    } else {
      if (input.dSecSpinPer != 0) 
	sec->dSpinRate = dPerToFreq(input.dSecSpinPer);
      if (input.dSecVRot != 0)
	sec->dSpinRate = input.dSecVRot/(2*PI*sec->dRadius);
    }
  }

  /* Set model-specific functions */
  if (param->iTideModel == CPL) {
    param->fDerivs = &DerivsCPL;
    param->fEqSpin = &EqSpinRate_CPL2;
  } else if (param->iTideModel == CTL) {
    param->fDerivs = &DerivsCTL;
    param->fEqSpin = &EqSpinRate_CTL8;
  }
}

void VerifyOutput(PARAM *param,OPTIONS options,OUTPUT *output,int iVerbose) {
  int i;

  if (param->iTideModel != CPL) {
    for (i=0;i<NUMOUT;i++) {
      if (!strcmp(param->cOutputOrder[i],output->cParam[OUT_EQSPINPERDISCRETE])) {
	fprintf(stderr,"ERROR: Cannot set both %s to p2 and output option %s.\n",options.cParam[OPT_TIDEMODEL],output->cParam[OUT_EQSPINPERDISCRETE]);
	exit(EXIT_PARAM);
      }
    }
  }

  if (param->bDoForward || param->bDoBackward) {
    if (iVerbose > VERBINPUT && param->iNumCols == -1) 
      fprintf(stderr,"WARNING: Integration requested, but %s not provided.\n",options.cParam[OPT_OUTPUTORDER]);
  }
}
      
void ReadOptions(char infile[],OPTIONS options,PARAM *param,PRIMARY *pri,SECONDARY *sec,FILES *files,OUTPUT *output,IO *io,fdStep *fdOneStep) {
  char cTmp[16],null[8];
  int i,iNumLines,nline,foo;   /* foo is a placeholder for calls to EqSpinRate_CTL */
  int *iLineOK;
  LINE line;
  INPUT input;

  sprintf(null,"null");

  /* Initialize input file */
  iNumLines = iGetNumLines(infile,io->exit_param);
  iLineOK = malloc(iNumLines*sizeof(int));
  InitializeInput(infile,iNumLines,iLineOK,io->exit_param);

  /*
   * Verbosity First!
   */
  
  /* Verbosity Level, currently only supports <5 */
  nline=-1;  
  if (io->iVerbose == -1) {
      io->iVerbose = atoi(options.cDefault[OPT_VERBOSE]); /* Default is normal output */
    AddOptionInt(infile,options.cParam[OPT_VERBOSE],&io->iVerbose,&nline,io->exit_param);
    if (io->iVerbose < 0 || io->iVerbose > VERBALL) {
      fprintf(stderr,"ERROR: %s must be in the range [0,%d]\n",options.cParam[OPT_VERBOSE],VERBALL);
      LineExit(infile,nline,io->exit_param,io->iVerbose);
    }	
  } else {
    /* Still find it in order to verify input file is OK */
    AddOptionInt(infile,options.cParam[OPT_VERBOSE],&i,&nline,io->exit_param);
    if (io->iVerbose >= VERBINPUT && nline != -1) 
      fprintf(stderr,"WARNING: -v set at command line, but %s option set. Output will be verbose.\n",options.cParam[OPT_VERBOSE]);
  }    
  if (nline != -1)       
    iLineOK[nline] = 1;

  /*
   * Next Comes Units
   */

  /* Mass Unit 
     0=gm
     1=kg
     2=solar
     3=Earth
     4=Jup
     5=Sat */

  param->iUnitMass = 0; /* Default is gm */
  nline=-1;
  sprintf(cTmp,"null");
  AddOptionString(infile,options.cParam[OPT_UNITMASS],cTmp,&nline,io->exit_param);
  /* Now assign the integer value */
  if (memcmp(lower(cTmp),"g",1) == 0) {
    param->iUnitMass = 0;
  } else if (memcmp(lower(cTmp),"k",1) == 0) {
    param->iUnitMass = 1;
  } else if (memcmp(lower(cTmp),"so",2) == 0) {
    param->iUnitMass = 2;
  } else if (memcmp(lower(cTmp),"e",1) == 0) {
    param->iUnitMass = 3;
  } else if (memcmp(lower(cTmp),"j",1) == 0) {
    param->iUnitMass = 4;
  } else if (memcmp(lower(cTmp),"sa",2) == 0) {
    param->iUnitMass = 5;
  } else if (memcmp(cTmp,"null",4) == 0) {
      if (io->iVerbose > 1)
        fprintf(stderr,"WARNING: %s not set, defaulting to grams.\n",options.cParam[OPT_UNITMASS]);
  } else {
    if (io->iVerbose >= VERBERR)
      fprintf(stderr,"ERROR: Unknown argument to %s: %s. Options are: gm, kg, solar, Earth, Jupiter, Saturn.\n",options.cParam[OPT_UNITMASS],cTmp);
    LineExit(infile,nline,io->exit_param, io->iVerbose);
  }
  if (nline != -1)
      iLineOK[nline] = 1;

  /* Time Unit 
     0=s
     1=d
     2=yr
     3=Myr
     4=Gyr */

  nline=-1;
  sprintf(cTmp,"null");
  param->iUnitTime = 0; /* Default is seconds */
  AddOptionString(infile,options.cParam[OPT_UNITTIME],cTmp,&nline,io->exit_param);
  if (memcmp(lower(cTmp),"s",1) == 0) {
    param->iUnitTime = 0;
  } else if (memcmp(lower(cTmp),"d",1) == 0) {
    param->iUnitTime = 1;
  } else if (memcmp(lower(cTmp),"y",1) == 0) {
    param->iUnitTime = 2;
  } else if (memcmp(lower(cTmp),"m",1) == 0) {
    param->iUnitTime = 3;
  } else if (memcmp(lower(cTmp),"g",1) == 0) {
    param->iUnitTime = 4;
  } else if (memcmp(cTmp,"null",4) == 0) {
      if (io->iVerbose >= VERBUNITS) 
	fprintf(stderr,"WARNING: %s not set, defaulting to seconds.\n",options.cParam[OPT_UNITTIME]);
  } else {
    if (io->iVerbose >= VERBERR)
      fprintf(stderr,"ERROR: Unknown argument to %s: %s. Options are seconds, days, years, Myr, or Gyr.\n",options.cParam[OPT_UNITTIME],cTmp);
    LineExit(infile,nline,io->exit_param,io->iVerbose);
  }
  if (nline != -1)
    iLineOK[nline] = 1;

  /* Angle Unit 
     0=rad
     1=deg */

  param->iUnitAngle = 0; /* Assume radians */
  nline = -1;
  sprintf(cTmp,"null");
  AddOptionString(infile,options.cParam[OPT_UNITANGLE],cTmp,&nline,io->exit_param);
  if (memcmp(lower(cTmp),"r",1) == 0) {
    param->iUnitAngle=0;
  } else if (memcmp(lower(cTmp),"d",1) == 0) {
    param->iUnitAngle=1;
  } else if (memcmp(cTmp,"null",4) == 0) {
      if (io->iVerbose >= VERBUNITS) 
	fprintf(stderr,"WARNING: %s not set, defaulting to radians.\n",options.cParam[OPT_UNITANGLE]);
      param->iUnitAngle=0;
  } else {
    if (io->iVerbose >= VERBERR)
      fprintf(stderr,"ERROR: Unknown argument to %s: %s. Options are radians or degrees.\n",options.cParam[OPT_UNITANGLE],cTmp);
    LineExit(infile,nline,io->exit_param,io->iVerbose);
  }
  if (nline != -1)
    iLineOK[nline] = 1;

  /* Length Unit
     0=cm
     1=m
     2=km
     3=R_sun
     4=R_earth
     5=R_Jup
     6=AU */

  param->iUnitLength=0; /* Assume cm */
  nline=-1;
  sprintf(cTmp,"null");
  AddOptionString(infile,options.cParam[OPT_UNITLENGTH],cTmp,&nline,io->exit_param);
  if (memcmp(lower(cTmp),"c",1) == 0) {
    param->iUnitLength=0;
  } else if (memcmp(lower(cTmp),"m",1) == 0) {
    param->iUnitLength=1;
  } else if (memcmp(lower(cTmp),"k",1) == 0) {
    param->iUnitLength=2;
  } else if (memcmp(lower(cTmp),"s",1) == 0) {
    param->iUnitLength=3;
  } else if (memcmp(lower(cTmp),"e",1) == 0) {
    param->iUnitLength=4;
  } else if (memcmp(lower(cTmp),"j",1) == 0) {
    param->iUnitLength=5;
  } else if (memcmp(lower(cTmp),"a",1) == 0) {
    param->iUnitLength=6;
  } else if (memcmp(cTmp,"null",4) == 0) {
    if (io->iVerbose >= VERBUNITS)
      fprintf(stderr,"WARNING: %s not set, defaulting to cm.\n",options.cParam[OPT_UNITLENGTH]);
  } else {
    if (io->iVerbose >= VERBERR)
      fprintf(stderr,"ERROR: Unknown argument to %s %s. Options are cm, m, km, solar, Earth, Jupiter, AU.\n",options.cParam[OPT_UNITLENGTH],cTmp);
    LineExit(infile,nline,io->exit_param,io->iVerbose);
  }
  if (nline != -1)
    iLineOK[nline] = 1;

  /* System Name */
  nline = -1;
  sprintf(param->cSystemName,"null");
  AddOptionString(infile,options.cParam[OPT_SYSTEMNAME],param->cSystemName,&nline,io->exit_param);
  if (memcmp(param->cSystemName,"null",4) == 0) {
    if (io->iVerbose >= VERBERR)
      fprintf(stderr,"ERROR: %s not found in %s!\n",options.cParam[OPT_SYSTEMNAME],infile);
    exit(io->exit_param);
  }
  if (nline != -1)
    iLineOK[nline] = 1;

  /* Tide Model */
  param->iTideModel=CPL; /* Default is CPL2 */
  sprintf(cTmp,"null");
  AddOptionString(infile,options.cParam[OPT_TIDEMODEL],cTmp,&nline,io->exit_param);
  if (memcmp(cTmp,"null",4) == 0) {
     if (io->iVerbose >= VERBERR) 
       fprintf(stderr,"WARNING: %s not set, assuming second-order constant phase lag.\n",options.cParam[OPT_TIDEMODEL]);
  } else {
    if (memcmp(lower(cTmp),"cp",2) == 0) {
      param->iTideModel = CPL;
    } else if (memcmp(lower(cTmp),"ct",2) == 0) {
      param->iTideModel = CTL;
    } else {
      if (io->iVerbose >= VERBERR)
	fprintf(stderr,"ERROR: Unknown argument to %s: %s. Options are cpl or ctl.\n",options.cParam[OPT_TIDEMODEL],cTmp);
      LineExit(infile,nline,io->exit_param,io->iVerbose);
    }
  }
  if (nline != -1)
    iLineOK[nline] = 1;


  /*
   *
   * Now ready to read in other options. Insert new options below.
   *
   */

  /*
   *
   * A
   *
   */

  /* Age */
  sec->dAge = 0;
  nline = -1;
  AddOptionDouble(infile,options.cParam[OPT_AGE],&sec->dAge,&nline,io->exit_param);
  if (sec->dAge < 0) {
    if (io->iVerbose >= VERBINPUT)
      fprintf(stderr,"WARNING: %s is negative, assuming Gyr.\n",options.cParam[OPT_AGE]);
    sec->dAge *= -1e9*YEARSEC;
  } else {
    sec->dAge *= dTimeUnit(param->iUnitTime,io->exit_param);
  }
  if (nline != -1)
    iLineOK[nline] = 1;

  /*
   *
   *   BACKWARD
   *
   */

  /*
   * Backward Integration Parameters
   */

  /* Do Backward Integration? */

  param->bDoBackward = 0; /* Default is no */
  nline = -1;
  AddOptionInt(infile,options.cParam[OPT_BACK],&param->bDoBackward,&nline,io->exit_param);
  if (param->bDoBackward < 0 || param->bDoBackward > 1){
    if (io->iVerbose >= VERBERR)
      fprintf(stderr,"ERROR: Argument to %s must be either 0 or 1.\n",options.cParam[OPT_BACK]);
    LineExit(infile,nline,io->exit_param,io->iVerbose);
  }
  if (nline != -1)
    iLineOK[nline] = 1;
  
  /* Backward file name */

  sprintf(files->cBack,"null");
  nline=-1;
  AddOptionString(infile,options.cParam[OPT_BACKFILE],files->cBack,&nline,io->exit_param);
  if (memcmp(files->cBack,"null",4) != 0) {
    if (param->bDoBackward == 0 && io->iVerbose >= VERBINPUT) {
      fprintf(stderr,"WARNING: %s found, but %s is set to 0.\n",options.cParam[OPT_BACKFILE],options.cParam[OPT_BACK]);
      fprintf(stderr,"\t%s: Line %d.\n",infile,nline);
    }
  } else {
    if (param->bDoBackward == 1) {
      sprintf(files->cBack,"%s.backward",param->cSystemName);
      if (io->iVerbose >= VERBINPUT)
	fprintf(stderr,"WARNING: %s is set, but %s not found: Defaulting to \"%s\".\n",options.cParam[OPT_BACKFILE],options.cParam[OPT_BACKFILE],files->cBack);
    }
  }
  if (nline != -1)
    iLineOK[nline] = 1;

  /* Backward integration output interval */

  param->dBackOutputTime = 0;
  nline = -1;
  AddOptionDouble(infile,options.cParam[OPT_BACKOUTPUTTIME],&param->dBackOutputTime,&nline,io->exit_param);
  if (param->dBackOutputTime != 0 && !param->bDoBackward && io->iVerbose >= VERBINPUT) 
    fprintf(stderr,"WARNING: %s initialized, but %s = 0.\n",options.cParam[OPT_BACKOUTPUTTIME],options.cParam[OPT_BACK]);
  if (param->dBackOutputTime < 0) {
    if (io->iVerbose >= VERBERR)
      fprintf(stderr,"ERROR: %s must be greater than 0.\n",options.cParam[OPT_BACKOUTPUTTIME]);
    LineExit(infile,nline,io->exit_param,io->iVerbose);
  }
  /* Convert timestep to cgs */
  param->dBackOutputTime *= dTimeUnit(param->iUnitTime,io->exit_units);
  if (nline != -1)
    iLineOK[nline] = 1;

  /* Backward integration stop time */

  param->dBackStopTime = 0;
  nline = -1;
  AddOptionDouble(infile,options.cParam[OPT_BACKSTOPTIME],&param->dBackStopTime,&nline,io->exit_param);
  if (param->dBackStopTime != 0 && !param->bDoBackward && io->iVerbose >= VERBINPUT)
    fprintf(stderr,"WARNING: %s set, but %s = 0.\n",options.cParam[OPT_BACKSTOPTIME],options.cParam[OPT_BACK]);
  if (param->dBackStopTime < 0) {
    if (io->iVerbose >= VERBERR)
      fprintf(stderr,"ERROR: %s must be greater than 0.\n",options.cParam[OPT_BACKSTOPTIME]);
    LineExit(infile,nline,io->exit_param,io->iVerbose);
  }
  /* Convert timestep to cgs */
  param->dBackStopTime *= dTimeUnit(param->iUnitTime,io->exit_units);
  if (nline != -1)
    iLineOK[nline] = 1;

  /* Backward integration timestep */

  param->dBackTimeStep = 0; 
  nline = -1;
  AddOptionDouble(infile,options.cParam[OPT_BACKTIMESTEP],&param->dBackTimeStep,&nline,io->exit_param);
  if (param->dBackTimeStep != 0 && !param->bDoBackward && io->iVerbose >= VERBINPUT)
    fprintf(stderr,"WARNING: %s initialized, but %s = 0.\n",options.cParam[OPT_BACKTIMESTEP],options.cParam[OPT_BACK]);
  if (param->dBackTimeStep < 0) {
    if (io->iVerbose >= VERBERR)
      fprintf(stderr,"ERROR: %s must be greater than 0.\n",options.cParam[OPT_BACKTIMESTEP]);
    LineExit(infile,nline,io->exit_param,io->iVerbose);
  }
  /* Convert timestep to cgs */
  param->dBackTimeStep *= dTimeUnit(param->iUnitTime,io->exit_units);
  if (nline != -1)
    iLineOK[nline] = 1;
  
  /*
   *
   * D
   *
   */

  param->bDiscreteRot = atoi(options.cDefault[OPT_DISCRETEROT]);   
  AddOptionBool(infile,options.cParam[OPT_DISCRETEROT],&param->bDiscreteRot,&nline,io->exit_param,io->iVerbose);
  if (param->bDiscreteRot != 0 && param->bDiscreteRot != 1) {
    if (io->iVerbose >= VERBERR) 
      fprintf(stderr,"ERROR: %s must be either 0 or 1.\n",options.cParam[OPT_DISCRETEROT]);
    LineExit(infile,nline,io->exit_param,io->iVerbose);
  }
  if (nline != -1 && param->iTideModel != CPL) {
    if (io->iVerbose >= VERBERR) 
      fprintf(stderr,"WARNING: %s is set, but time lag model selected. Ignoring.\n",options.cParam[OPT_DISCRETEROT]);
  }
  if (nline != -1)
    iLineOK[nline] = 1;

  /*
   * 
   *   FORWARD
   *
   */
  
  /* Forward integration? */

  param->bDoForward = 0; 	/* 0 or 1 */
  AddOptionInt(infile,options.cParam[OPT_FORW],&param->bDoForward,&nline,io->exit_param);
  if (param->bDoForward != 0 && param->bDoForward != 1) {
    if (io->iVerbose >= VERBERR) 
      fprintf(stderr,"ERROR: %s must be either 0 or 1.\n",options.cParam[OPT_FORW]);
    LineExit(infile,nline,io->exit_param,io->iVerbose);
  }
  if (nline != -1)
    iLineOK[nline] = 1;

  /* Print warning if no time integrations */

  if (!param->bDoBackward && !param->bDoForward && io->iVerbose >= VERBINPUT) 
    fprintf(stderr,"WARNING: No time integrations.\n");

  /* Forward file name */

  sprintf(files->cFor,"null");
  nline=-1;
  AddOptionString(infile,options.cParam[OPT_FORWFILE],files->cFor,&nline,io->exit_param);
  if (memcmp(files->cFor,"null",4) != 0) {
    if (param->bDoForward == 0 && io->iVerbose >= VERBINPUT) {
      fprintf(stderr,"WARNING: %s found, but %s is set to 0.\n",options.cParam[OPT_FORWFILE],options.cParam[OPT_FORW]);
      fprintf(stderr,"\t%s: Line %d.\n",infile,nline);
    }
  } else {
    if (param->bDoForward == 1) {
      sprintf(files->cFor,"%s.forward",param->cSystemName);
      if (io->iVerbose >= VERBINPUT)
	fprintf(stderr,"WARNING: %s is set, but %s not found. Defaulting to \"%s\".\n",options.cParam[OPT_FORWFILE],options.cParam[OPT_FORW],files->cFor);
    }
  }
  if (nline != -1)
      iLineOK[nline] = 1;
  
  /* Forward integration output interval */

  param->dForwOutputTime = 0;
  nline = -1;
  AddOptionDouble(infile,options.cParam[OPT_FORWOUTPUTTIME],&param->dForwOutputTime,&nline,io->exit_param);
  if (param->dForwOutputTime != 0 && !param->bDoForward && io->iVerbose >= VERBINPUT) {
    fprintf(stderr,"WARNING: %s initialized, but %s = 0.\n",options.cParam[OPT_FORWOUTPUTTIME],options.cParam[OPT_FORW]);
  }
  if (param->dForwOutputTime < 0) {
    if (io->iVerbose >= VERBERR)
      fprintf(stderr,"%s must be greater than 0.\n",options.cParam[OPT_FORWOUTPUTTIME]);
    LineExit(infile,nline,io->exit_param,io->iVerbose);
  }
  /* Convert timestep to cgs */
  param->dForwOutputTime *= dTimeUnit(param->iUnitTime,io->exit_units);
  if (nline != -1)
    iLineOK[nline] = 1;

  /* Forward integration stop time */

  nline = -1;
  param->dForwStopTime = 0;
  
  AddOptionDouble(infile,options.cParam[OPT_FORWSTOPTIME],&param->dForwStopTime,&nline,io->exit_param);
  if (param->dForwStopTime != 0 && !param->bDoForward) {
    if (io->iVerbose >= VERBERR)
      fprintf(stderr,"%s initialized, but %s = 0.\n",options.cParam[OPT_FORWSTOPTIME],options.cParam[OPT_FORW]);
    LineExit(infile,nline,io->exit_param,io->iVerbose);
  }
  if (param->dForwStopTime < 0) {
    if (io->iVerbose >= VERBERR)
      fprintf(stderr,"ERROR: %s must be greater than 0.\n",options.cParam[OPT_FORWSTOPTIME]);
    LineExit(infile,nline,io->exit_param,io->iVerbose);
  }
  /* Convert timestep to cgs */
  param->dForwStopTime *= dTimeUnit(param->iUnitTime,io->exit_units);
  if (nline != -1)
    iLineOK[nline] = 1;

  /* Forward integration timestep */

  param->dForwTimeStep = 0; 
  nline = -1;
  AddOptionDouble(infile,options.cParam[OPT_FORWTIMESTEP],&param->dForwTimeStep,&nline,io->exit_param);
  if (param->dForwTimeStep != 0 && !param->bDoForward && io->iVerbose >= VERBINPUT) {
    fprintf(stderr,"WARNING: %s initialized, but %s is 0.\n",options.cParam[OPT_FORWTIMESTEP],options.cParam[OPT_FORW]);
    
  }
  if (param->dForwTimeStep < 0) {
    if (io->iVerbose >= VERBERR)
      fprintf(stderr,"ERROR: %s must be greater than 0.\n",options.cParam[OPT_FORWTIMESTEP]);
    LineExit(infile,nline,io->exit_param,io->iVerbose);
  }
  /* Convert timestep to cgs */
  param->dForwTimeStep *= dTimeUnit(param->iUnitTime,io->exit_units);
  if (nline != -1)
    iLineOK[nline] = 1;
  
  /*
   *
   * I
   *
   */

  /* Integration Method */
  sprintf(cTmp,"null");
  AddOptionString(infile,options.cParam[OPT_INTEGRATION],cTmp,&nline,io->exit_param);
  if (memcmp(cTmp,"null",4) == 0) {
     if (io->iVerbose >= VERBERR) 
       fprintf(stderr,"WARNING: %s not set, integration method will be %s.\n",options.cParam[OPT_INTEGRATION],options.cDefault[OPT_INTEGRATION]);
     param->iIntegration = RK4; /* Default is Runge-Kutta 4 */
     *fdOneStep = &RK4Step;
  } else {
    if (memcmp(lower(cTmp),"e",1) == 0) {
      param->iIntegration = EULER;
      *fdOneStep = &EulerStep;
    } else if (memcmp(lower(cTmp),"r",1) == 0) {
      param->iIntegration = RK4;
      *fdOneStep = &RK4Step;
    } else {
      if (io->iVerbose >= VERBERR)
	fprintf(stderr,"ERROR: Unknown argument to %s: %s. Options are Euler or RungeKutta4.\n",options.cParam[OPT_TIDEMODEL],cTmp);
      LineExit(infile,nline,io->exit_param,io->iVerbose);
    }
  }
  if (nline != -1)
    iLineOK[nline] = 1;


  /*
   *
   *   HALT
   *
   */

  param->halt.bHalt = 0; /* Start by assuming no halts */

  /* Double Synchronous */

  param->halt.bDblSync = atoi(options.cDefault[OPT_HALTDBLSYNC]);
  nline = -1;
  AddOptionBool(infile,options.cParam[OPT_HALTDBLSYNC],&param->halt.bDblSync,&nline,io->exit_param,io->iVerbose);
  if (nline != -1)
    iLineOK[nline] = 1;

  /* Maximum Eccentricity */
  
  param->halt.dMaxEcc = 1; 
  nline = -1;
  AddOptionDouble(infile,options.cParam[OPT_HALTMAXECC],&param->halt.dMaxEcc,&nline,io->exit_param);
  if (param->halt.dMaxEcc < 0 || param->halt.dMaxEcc > 1) {
    if (io->iVerbose >= VERBERR)
      fprintf(stderr,"ERROR: %s must be in the range (0,1).\n",options.cParam[OPT_HALTMAXECC]);
    LineExit(infile,nline,io->exit_param,io->iVerbose);
  }
  if (param->halt.dMaxEcc < 1)
    param->halt.bHalt = 1;
  if (nline != -1) 
    iLineOK[nline] = 1;
  
  /* Merge */

  param->halt.bMerge = 1;
  AddOptionBool(infile,options.cParam[OPT_HALTMERGE],&param->halt.bMerge,&nline,io->exit_param,io->iVerbose);
  if (param->halt.bMerge == 1)
    param->halt.bHalt = 1;
  if (nline != -1)
    iLineOK[nline] = 1;

  /* Minimum Eccentricity */
  
  param->halt.dMinEcc = -1; 
  nline = -1;
  AddOptionDouble(infile,options.cParam[OPT_HALTMINECC],&param->halt.dMinEcc,&nline,io->exit_param);
  if (nline != -1) {
    if (param->halt.dMinEcc < 0 || param->halt.dMinEcc > 1) {
      if (io->iVerbose >= VERBERR)
	fprintf(stderr,"ERROR: %s must be in the range (0,1).\n",options.cParam[OPT_HALTMINECC]);
      LineExit(infile,nline,io->exit_param,io->iVerbose);
    }
    // Min ecc OK
    param->halt.bHalt = 1;
  }
  if (nline != -1)
    iLineOK[nline] = 1;

   /* Minimum obliquity */
  
  param->halt.dMinPriObl = -1; /* Assume no minimum */
  nline = -1;
  AddOptionDouble(infile,options.cParam[OPT_HALTMINPRIOBL],&param->halt.dMinPriObl,&nline,io->exit_param);
  if (param->iUnitAngle == 0) {
    if (nline != -1 && (param->halt.dMinPriObl < 0 || param->halt.dMinPriObl > PI) ) {
      if (io->iVerbose >= VERBERR) {
	fprintf(stderr,"ERROR: %s must be in the range [0,PI].\n",options.cParam[OPT_HALTMINPRIOBL]);
	LineExit(infile,nline,io->exit_param,io->iVerbose);
      } 
    } else {
      if (nline != -1 && (param->halt.dMinPriObl < 0 || param->halt.dMinPriObl > 180) ) {
	fprintf(stderr,"ERROR: %s must be in the range [0,180].\n",options.cParam[OPT_HALTMINPRIOBL]);
	LineExit(infile,nline,io->exit_param,io->iVerbose);
      } 
      /* Change to radians */
      param->dMinObliquity *= DEGRAD;
    }
  }
  if (param->halt.dMinPriObl > 0)
    param->halt.bHalt = 1;
  if (nline != -1)
      iLineOK[nline] = 1;

  /* Secondary's Minimum Obliquity */

  param->halt.dMinSecObl = -1; /* Assume no minimum */
  nline = -1;
  AddOptionDouble(infile,options.cParam[OPT_HALTMINSECOBL],&param->halt.dMinSecObl,&nline,io->exit_param);
  if (param->iUnitAngle == 0) {
    if (nline != -1 && (param->halt.dMinSecObl < 0 || param->halt.dMinSecObl > PI) ) {
      if (io->iVerbose >= VERBERR) {
	fprintf(stderr,"ERROR: %s must be in the range [0,PI].\n",options.cParam[OPT_HALTMINSECOBL]);
	LineExit(infile,nline,io->exit_param,io->iVerbose);
      } 
    } else {
      if (nline != -1 && (param->halt.dMinSecObl < 0 || param->halt.dMinSecObl > 180) ) {
	fprintf(stderr,"ERROR: %s must be in the range [0,180].\n",options.cParam[OPT_HALTMINSECOBL]);
	LineExit(infile,nline,io->exit_param,io->iVerbose);
      } 
      /* Change to radians */
      param->dMinObliquity *= DEGRAD;
    }
  }
  if (param->halt.dMinSecObl > 0)
    param->halt.bHalt = 1;
  if (nline != -1)
    iLineOK[nline] = 1;

  /* Minimum Semi-Major Axis */

  param->halt.dMinSemi=0;
  nline = -1;
  AddOptionDouble(infile,options.cParam[OPT_HALTMINSEMI],&param->halt.dMinSemi,&nline,io->exit_param);
  if (param->halt.dMinSemi < 0) {
    if (io->iVerbose >= VERBERR)
      fprintf(stderr,"ERROR: %s must be larger than 0.\n",options.cParam[OPT_HALTMINSEMI]);
    LineExit(infile,nline,io->exit_param,io->iVerbose);
  }
  if (param->halt.dMinSemi > 0) {
    /* Convert to cgs */
    param->halt.dMinSemi *= dLengthUnit(param->iUnitLength,io->exit_units);
    param->halt.bHalt=1;
  }
  if (nline != -1)
    iLineOK[nline] = 1;

  /* Positive de/dt */

  param->halt.bPosDeDt = atoi(options.cDefault[OPT_HALTPOSDEDT]);
  nline = -1;
  AddOptionBool(infile,options.cParam[OPT_HALTPOSDEDT],&param->halt.bPosDeDt,&nline,io->exit_param,io->iVerbose);
  if (nline != -1)
    iLineOK[nline] = 1;
  if (param->halt.bPosDeDt == 1)
    param->halt.bHalt = 1;

  /* Primary Tide-locked */

  param->halt.bPriLock = atoi(options.cDefault[OPT_HALTPRILOCK]);
  nline = -1;
  AddOptionBool(infile,options.cParam[OPT_HALTPRILOCK],&param->halt.bPriLock,&nline,io->exit_param,io->iVerbose);
  if (nline != -1)
    iLineOK[nline] = 1;
  if (param->halt.bPriLock == 1)
    param->halt.bHalt = 1;

  /* Secondary Tide-locked */

  param->halt.bSecLock = atoi(options.cDefault[OPT_HALTSECLOCK]);
  nline = -1;
  AddOptionBool(infile,options.cParam[OPT_HALTSECLOCK],&param->halt.bSecLock,&nline,io->exit_param,io->iVerbose);
  if (nline != -1)
    iLineOK[nline] = 1;
  if (param->halt.bSecLock == 1)
    param->halt.bHalt = 1;
  
  /* Secondary Synchronously Rotating */

  param->halt.bSecSync = atoi(options.cDefault[OPT_HALTSECSYNC]);
  nline = -1;
  AddOptionBool(infile,options.cParam[OPT_HALTSECSYNC],&param->halt.bSecSync,&nline,io->exit_param,io->iVerbose);
  if (nline != -1)
    iLineOK[nline] = 1;
  if (param->halt.bSecSync == 1)
    param->halt.bHalt = 1;

  /*
   *
   *   LOG
   *
   */

  /* Do log file? */
  nline = -1;
  param->bLog = 0; /* Default is no logfile */
  AddOptionInt(infile,options.cParam[OPT_LOG],&param->bLog,&nline,io->exit_param);
  if (param->bLog != 0 && param->bLog != 1) {
    if (io->iVerbose >= VERBERR)
      fprintf(stderr,"ERROR: %s must be either 0 or 1.\n",options.cParam[OPT_LOG]);
    LineExit(infile,nline,io->exit_param,io->iVerbose);
  }
  if (nline != -1)
    iLineOK[nline] = 1;
  
  files->cLog[0] = 0;
  nline = -1;
  AddOptionString(infile,options.cParam[OPT_LOGFILE],files->cLog,&nline,io->exit_param);
  if (files->cLog[0] == 0 && param->bLog) {
    sprintf(files->cLog,"%s.log",param->cSystemName);
    if (io->iVerbose >= VERBINPUT)
      fprintf(stderr,"WARNING: %s is True, but %s not found. Defaulting to \"%s\".\n",options.cParam[OPT_LOG],options.cParam[OPT_LOGFILE],files->cLog);
  }
  if (nline != -1)
    iLineOK[nline] = 1;
 
  /*
   *
   * M
   *
   */

  param->dMinValue = atof(options.cDefault[OPT_MINVALUE]);
  nline = -1;
  AddOptionDouble(infile,options.cParam[OPT_MINVALUE],&param->dMinValue,&nline,io->exit_param);
  if (param->dMinValue < 0) {
    if (io->iVerbose >= VERBERR)
      fprintf(stderr,"ERROR: %s must be larger than 0.\n",options.cParam[OPT_MINVALUE]);
    LineExit(infile,nline,io->exit_param,io->iVerbose);
  }
  if (nline != -1)
    iLineOK[nline] = 1;
  
  /*
   *
   *   ORBIT
   *
   */

  /* Eccentricity */

  sec->dEcc = -1;
  nline = -1;
  AddOptionDouble(infile,options.cParam[OPT_ORBECC],&sec->dEcc,&nline,io->exit_param);
  if (nline == -1) {
    if (io->iVerbose >= VERBERR)
      fprintf(stderr,"ERROR: %s must be set.\n",options.cParam[OPT_ORBECC]);
    exit(io->exit_param);
  }
  if (sec->dEcc < 0) {
    if (io->iVerbose >= VERBERR)
      fprintf(stderr,"ERROR: %s must be non-negative.\n",options.cParam[OPT_ORBECC]);
    LineExit(infile,nline,io->exit_param,io->iVerbose);
  }
  if (sec->dEcc > pow(2./19,0.5) && param->bDiscreteRot) {
    if (io->iVerbose >= VERBINPUT)
      fprintf(stderr,"WARNING: Setting %s to 1 is not advised for eccentricities larger than %.3lf\n",options.cParam[OPT_DISCRETEROT],pow(2./19,0.5));
  }

  if (nline != -1)
    iLineOK[nline] = 1;
			     
  /* Mean Motion */

  sec->dMeanMotion = 0;
  nline = -1;
  AddOptionDouble(infile,options.cParam[OPT_ORBMEANMOTION],&sec->dMeanMotion,&nline,io->exit_param);
  if (sec->dMeanMotion <= 0 && nline != -1) {
    if (io->iVerbose >= VERBERR)
      fprintf(stderr,"ERROR: %s must be greater than 0.\n",options.cParam[OPT_ORBMEANMOTION]);
      LineExit(infile,nline,io->exit_param,io->iVerbose);
  } 
  if (nline != -1) {
    sec->dMeanMotion /= dTimeUnit(param->iUnitTime,io->exit_units);
    iLineOK[nline] = 1;
  }
  line.lMeanMotion=nline;

  /* Orbital Period */

  input.dPeriod = 0;
  nline = -1;
  AddOptionDouble(infile,options.cParam[OPT_ORBPER],&input.dPeriod,&nline,io->exit_param);
  if (input.dPeriod < 0) {
    input.dPeriod = -input.dPeriod*DAYSEC;
    if (io->iVerbose >= VERBUNITS)
      fprintf(stderr,"WARNING: %s is less than 0, assuming days.\n",options.cParam[OPT_ORBPER]);
    iLineOK[nline] = 1;
  } else if (input.dPeriod > 0) {
    input.dPeriod *= dTimeUnit(param->iUnitTime,io->exit_units);
    iLineOK[nline] = 1;
  }
  line.lPeriod=nline;

  /* Semi-Major Axis */

  sec->dSemi = 0;
  nline = -1;
  AddOptionDouble(infile,options.cParam[OPT_ORBSEMI],&sec->dSemi,&nline,io->exit_param);
  if (sec->dSemi < 0) {
    sec->dSemi = -sec->dSemi*AUCM;
    if (io->iVerbose >= VERBUNITS)
      fprintf(stderr,"WARNING: %s is less than zero, assuming AU.\n",options.cParam[OPT_ORBSEMI]);
    iLineOK[nline] = 1;
  } else if (sec->dSemi > 0) {
    sec->dSemi *= dLengthUnit(param->iUnitLength,io->exit_units);
    iLineOK[nline] = 1;
  }
  line.lSemi=nline;

  /*
   *
   *   OUT
   *
   */

  io->iDigits = 4;
  nline = -1;
  AddOptionInt(infile,options.cParam[OPT_OUTDIGITS],&io->iDigits,&nline,io->exit_param);
  if (nline == -1) {
    if (io->iVerbose >= VERBINPUT)
      fprintf(stderr,"WARNING: %s not set, defaulting to %s.\n",options.cParam[OPT_OUTDIGITS],options.cDefault[OPT_OUTDIGITS]);
  }
  if (io->iDigits < 0) {
    if (io->iVerbose >= VERBERR)
      fprintf(stderr,"ERROR: %s must be non-negative.\n",options.cParam[OPT_OUTDIGITS]);
    LineExit(infile,nline,io->exit_param,io->iVerbose);
  }
  if (io->iDigits > 16) {
    if (io->iVerbose >= VERBERR)
      fprintf(stderr,"ERROR: %s must be less than 16.\n",options.cParam[OPT_OUTDIGITS]);
    LineExit(infile,nline,io->exit_param,io->iVerbose);
  }
  if (nline != -1)
    iLineOK[nline] = 1;
			     
  io->iSciNot = 4;
  nline = -1;
  AddOptionInt(infile,options.cParam[OPT_OUTSCINOT],&io->iSciNot,&nline,io->exit_param);
  if (nline == -1) {
    if (io->iVerbose >= VERBINPUT)
      fprintf(stderr,"WARNING: %s not set, defaulting to %s.\n",options.cParam[OPT_OUTSCINOT],options.cDefault[OPT_OUTSCINOT]);
  }
  if (io->iSciNot < 0) {
    if (io->iVerbose >= VERBERR)
      fprintf(stderr,"ERROR: %s must be non-negative.\n",options.cParam[OPT_OUTSCINOT]);
    LineExit(infile,nline,io->exit_param,io->iVerbose);
  }
  if (io->iSciNot > 16) {
    if (io->iVerbose >= VERBERR)
      fprintf(stderr,"ERROR: %s must be less than 16.\n",options.cParam[OPT_OUTSCINOT]);
    LineExit(infile,nline,io->exit_param,io->iVerbose);
  }
  if (nline != -1)
    iLineOK[nline] = 1;
  
  
  /*
   *
   * PRIMARY
   *
   */
  
  /* Force Equilibrium Spin Rate? */

  nline = -1;
  pri->bForceEqSpin = 0;
  AddOptionBool(infile,options.cParam[OPT_PRIFORCEEQSPIN],&pri->bForceEqSpin,&nline,io->exit_param,io->iVerbose);
  if (nline != -1) 
    iLineOK[nline] = 1;

  /* Primary k2 */

  nline = -1;
  AddOptionDouble(infile,options.cParam[OPT_PRIK2],&pri->dK2,&nline,io->exit_param);
  if (nline == -1) {
    if (io->iVerbose >= VERBERR)
      fprintf(stderr,"ERROR: %s must be set.\n",options.cParam[OPT_PRIK2]);
    exit(io->exit_param);
  }
  if (pri->dK2 < 0) {
    if (io->iVerbose >= VERBERR)
      fprintf(stderr,"ERROR: %s must be greater than zero.\n",options.cParam[OPT_PRIK2]);
    LineExit(infile,nline,io->exit_param,io->iVerbose);
  } 
  if (nline != -1) 
    iLineOK[nline] = 1;
  
  /* Primary Mass */

  /* Negative values defaults to solar units */
  /* If undefined, defaults to value predicted by param->cPrimaryMassRad */
  pri->dMass = 0;
  nline = -1;
  AddOptionDouble(infile,options.cParam[OPT_PRIMASS],&pri->dMass,&nline,io->exit_param);
  /* Convert to cgs */
  if (pri->dMass > 0) 
    pri->dMass *= dMassUnit(param->iUnitMass,io->exit_units);
  else {
    if (io->iVerbose >= VERBUNITS) 
      fprintf(stderr,"WARNING: %s is negative, assuming solar units.\n",options.cParam[OPT_PRIMASS]);
    pri->dMass *= -MSUN;
  }
  if (nline != -1)
    iLineOK[nline] = 1;
  
  /* Primary's Mass-Radius relationship */

  /* Checks are performed in Verify(); */
  sprintf(cTmp,"null");
  pri->iMassRad = 0;
  nline = -1;
  AddOptionString(infile,options.cParam[OPT_PRIMASSRAD],cTmp,&nline,io->exit_param);
  if (memcmp(cTmp,"null",4) != 0) {
    if (memcmp(lower(cTmp),"r",1) == 0) {
      /* Reid & Hawley 2000 */	
      pri->iMassRad=1;
    } else if (memcmp(lower(cTmp),"g",1) == 0) {
      /* Gorda and Svenchnikov 1999 */
      pri->iMassRad=2;
    } else if (memcmp(lower(cTmp),"bay",3) == 0) {
      /* Bayless & Orosz 2006 */
      pri->iMassRad=3;
    } else if (memcmp(lower(cTmp),"s",1) == 0) {
      /* Sotin et al 2007 */
      pri->iMassRad=4;
    } else if (memcmp(lower(cTmp),"bar",3) == 0) {
      pri->iMassRad=5;
    } else {
      if (io->iVerbose >= VERBERR) {
	fprintf(stderr,"ERROR: Unknown argument to %s: %s.\n",options.cParam[OPT_PRIMASSRAD],cTmp);
	fprintf(stderr,"Options are GS99 (Gorda & Svechnikov 1999), BO06 (Bayless & Orosz 2006), Sotin07 (Sotin et al. 2007), or RH00 (Reid & Hawley 2000).\n");
      }
      LineExit(infile,nline,io->exit_param,io->iVerbose);
    }
  } 
  if (nline != -1)
    iLineOK[nline] = 1;

  /* Maximum allowable offset between primary's spin period and its
     equilibrium spin period. */

  pri->dMaxLockDiff = atof(options.cDefault[OPT_PRIMAXLOCKDIFF]);
  nline = -1;
  AddOptionDouble(infile,options.cParam[OPT_PRIMAXLOCKDIFF],&pri->dMaxLockDiff,&nline,io->exit_param);
  if (pri->dMaxLockDiff < 0 && io->iVerbose > VERBERR) {
    fprintf(stderr,"ERROR: %s must be > 0.\n",options.cParam[OPT_PRIMAXLOCKDIFF]);
    LineExit(infile,nline,io->exit_param,io->iVerbose);
  }
  if (nline != -1)
    iLineOK[nline] = 1;

  /* Obliquity */

  pri->dObliquity = 0; /* Default is no obliquity */
  nline = -1;
  AddOptionDouble(infile,options.cParam[OPT_PRIOBL],&pri->dObliquity,&nline,io->exit_param);
  if (param->iUnitAngle == 0) {
    if (pri->dObliquity < 0 || pri->dObliquity > PI) {
	if (io->iVerbose >= VERBERR)
	  fprintf(stderr,"ERROR: %s must be in the range [0,PI].\n",options.cParam[OPT_PRIOBL]);
	LineExit(infile,nline,io->exit_param,io->iVerbose);
    }
  } else {
    if (pri->dObliquity < 0 || pri->dObliquity > 180) {
      if (io->iVerbose >= VERBERR)
	fprintf(stderr,"ERROR: %s must be in the range [0,180].\n",options.cParam[OPT_PRIOBL]);
      LineExit(infile,nline,io->exit_param,io->iVerbose);	
    }
    /* Change to radians */
    pri->dObliquity *= DEGRAD;
  }
  if (nline != -1)
    iLineOK[nline] = 1;
  
  /* Q */
  
  pri->dQ = 0;
  nline = -1;
  AddOptionDouble(infile,options.cParam[OPT_PRIQ],&pri->dQ,&nline,io->exit_param);
  if (pri->dQ < 0) {
    if (io->iVerbose >= VERBERR)
      fprintf(stderr,"ERROR: %s must be greater than 0.\n",options.cParam[OPT_PRIQ]);
    LineExit(infile,nline,io->exit_param,io->iVerbose);
  }
  if (param->iTideModel == CPL && pri->dQ == 0) {
    if (io->iVerbose >= VERBERR)
      fprintf(stderr,"ERROR: Phase lag model selected, but %s not set.\n",options.cParam[OPT_PRIQ]);
    exit(io->exit_param);
  }
  if (param->iTideModel == CTL && pri->dQ != 0 && io->iVerbose >= VERBINPUT) 
    fprintf(stderr,"WARNING: CTL model selected, but %s set.\n",options.cParam[OPT_PRIQ]);
  if (nline != -1)
    iLineOK[nline] = 1;
  
  /* Primary Radius */
  
  /* Negative value defaults to solar units */
  pri->dRadius = 0; /* Default is use relationship */
  nline = -1;
  AddOptionDouble(infile,options.cParam[OPT_PRIRAD],&pri->dRadius,&nline,io->exit_param);
  if (pri->dRadius > 0)
    pri->dRadius *= dLengthUnit(param->iUnitLength,io->exit_units);
  else {
    if (io->iVerbose >= VERBUNITS)
      fprintf(stderr,"WARNING: %s is less than 0, assuming solar units.\n",options.cParam[OPT_PRIRAD]);
    pri->dRadius *= -RSUN;
  }  
  if (nline != -1)
    iLineOK[nline] = 1;
  
  /* Radius of Gyration */

  pri->dRG = 0.5;
  nline = -1;
  AddOptionDouble(infile,options.cParam[OPT_PRIRG],&pri->dRG,&nline,io->exit_param);
  if (pri->dRG < 0) {
    if (io->iVerbose >= VERBERR)
      fprintf(stderr,"ERROR: %s must be greater than zero.\n",options.cParam[OPT_PRIRG]);
    LineExit(infile,nline,io->exit_param,io->iVerbose);
  }
  if (nline != -1)
    iLineOK[nline] = 1;
  
  /* Primary Rotation Period */

  /* Checked with PriSpinRate and PriVRot in Verify() */
  input.dPriSpinPer = 0;
  nline = -1;
  AddOptionDouble(infile,options.cParam[OPT_PRISPINPER],&input.dPriSpinPer,&nline,io->exit_param);
  if (input.dPriSpinPer < 0) {
    if (io->iVerbose >= VERBUNITS) 
      fprintf(stderr,"WARNING: %s is less than 0, assuming days.\n",options.cParam[OPT_PRISPINPER]);
    input.dPriSpinPer *= -DAYSEC;
  } else
    input.dPriSpinPer *= dTimeUnit(param->iUnitTime,io->exit_units);
  
  if (nline != -1) 
    iLineOK[nline] = 1;
  line.lPriSpinPer = nline;
  
  /* Primary Rotation Frequency */

  /* Checked with PriSpinPer and PriVRot in Verify() */
  pri->dSpinRate = 0; 
  nline=-1;
  AddOptionDouble(infile,options.cParam[OPT_PRISPINRATE],&pri->dSpinRate,&nline,io->exit_param);
  if (pri->dSpinRate < 0 && nline != -1) {
    if (io->iVerbose >= VERBERR)
      fprintf(stderr,"ERROR: %s must be greater than 0.\n",options.cParam[OPT_PRISPINRATE]);
    LineExit(infile,nline,io->exit_param,io->iVerbose);
  }
  if (nline != -1) {
    pri->dSpinRate /= dTimeUnit(param->iUnitTime,io->exit_units);
    iLineOK[nline] = 1;
  }
  line.lPriSpinRate = nline;

  /* Time lag */

  nline = -1;
  pri->dTau = 0;
  AddOptionDouble(infile,options.cParam[OPT_PRITAU],&pri->dTau,&nline,io->exit_param);
  if (param->iTideModel == CPL && pri->dTau > 0 && io->iVerbose >= VERBINPUT) 
    fprintf(stderr,"WARNING: Phase lag model selected and %s set.\n",options.cParam[OPT_PRITAU]);
  if (pri->dTau < 0) {
    if (io->iVerbose >= VERBUNITS)
      fprintf(stderr,"WARNING: %s < 0, assuming seconds.\n",options.cParam[OPT_PRITAU]);
    pri->dTau = -pri->dTau;
  } else if (pri->dTau > 0) {
    /* Convert timestep to cgs */
    pri->dTau *= dTimeUnit(param->iUnitTime,io->exit_units);
  }
  if (param->iTideModel == CPL && pri->dTau != 0 && io->iVerbose >= VERBINPUT) 
    fprintf(stderr,"WARNING: CPL model selected, but %s set.\n",options.cParam[OPT_PRITAU]);
  if (nline != -1)
    iLineOK[nline] = 1;
   
  /* Rotational Velocity */

  /* Checked with PriSpinRate and PriVRot in Verify() */
  input.dPriVRot = 0;
  nline = -1;
  AddOptionDouble(infile,options.cParam[OPT_PRIVROT],&input.dPriVRot,&nline,io->exit_param);
  if (input.dPriVRot < 0) {
    if (io->iVerbose >= VERBUNITS) 
      fprintf(stderr,"WARNING: %s is less than 0, assuming km/s.\n",options.cParam[OPT_PRIVROT]);
    input.dPriVRot = - input.dPriVRot*1e5;
  } else 
    input.dPriVRot *= dTimeUnit(param->iUnitTime,io->exit_units)/dLengthUnit(param->iUnitLength,io->exit_units);
  
  if (nline != -1) {
    iLineOK[nline] = 1;
  }
  line.lPriVRot = nline;
  
  /*
   * 
   *   SECONDARY
   *
   */
  
  /* Force Equilibrium Spin Rate? */

  nline = -1;
  sec->bForceEqSpin = 0;
  AddOptionBool(infile,options.cParam[OPT_SECFORCEEQSPIN],&sec->bForceEqSpin,&nline,io->exit_param,io->iVerbose);
  if (nline != -1) 
    iLineOK[nline] = 1;
  
  /* Secondary's K2 */

  sec->dK2=-1;
  AddOptionDouble(infile,options.cParam[OPT_SECK2],&sec->dK2,&nline,io->exit_param);
  if (sec->dK2 <= 0 && nline != -1) {
    if (io->iVerbose >= VERBERR)
      fprintf(stderr,"ERROR: %s must be greater than zero.\n",options.cParam[OPT_SECK2]);
    LineExit(infile,nline,io->exit_param,io->iVerbose);
  }
  if (nline == -1) {
    if (io->iVerbose >= VERBERR)
      fprintf(stderr,"ERROR: %s must be set.\n",options.cParam[OPT_SECK2]);
    exit(io->exit_param);
  }
  if (nline != -1) 
    iLineOK[nline] = 1;
  
  /* Secondary Mass */

  /* Negative values defaults to Earth units */
  nline = -1;
  sec->dMass = 0;
  AddOptionDouble(infile,options.cParam[OPT_SECMASS],&sec->dMass,&nline,io->exit_param);
  if (sec->dMass > 0) {
    /* Convert to cgs */
    sec->dMass *= dMassUnit(param->iUnitMass,io->exit_units);
  } else {
    if (io->iVerbose >= VERBUNITS) 
      fprintf(stderr,"WARNING: %s is less than zero, using Earth units.\n",options.cParam[OPT_SECMASS]);
    sec->dMass *= -MEARTH;
  }
  if (nline != -1)
    iLineOK[nline] = 1;
  
  /* Secondary Mass-Radius relationship */
  
  sec->iMassRad = 0;
  sprintf(cTmp,"null");
  nline=-1;
  AddOptionString(infile,options.cParam[OPT_SECMASSRAD],cTmp,&nline,io->exit_param);
  if (memcmp(cTmp,null,4) != 0) {
    if (memcmp(lower(cTmp),"r",1) == 0) {
      /* NLDS */
      sec->iMassRad=1;
    } else if (memcmp(cTmp,"g",1) == 0) {
      /* Gorda and Svenchnikov 1999 */
      sec->iMassRad=2;
    } else if (memcmp(cTmp,"b",1) == 0) {
      /* Bayless & Orosz 2006 */
      sec->iMassRad=3;
    } else if (memcmp(cTmp,"s",1) == 0) {
      /* Sotin et al 2007 */
      sec->iMassRad=4;
    } else {
      if (io->iVerbose >= VERBERR) {
	fprintf(stderr,"ERROR: Unknown argument to %s: %s.\n",options.cParam[OPT_SECMASSRAD],cTmp);
	fprintf(stderr,"Options are GS99, BO06, Sotin07, or RH00.\n");
      }
      LineExit(infile,nline,io->exit_param,io->iVerbose);
    }
  } 
  if (nline != -1)	
    iLineOK[nline] = 1;

  /* Maximum allowable offset between secondary's spin period and its
     equilibrium spin period. */

  sec->dMaxLockDiff = atof(options.cDefault[OPT_SECMAXLOCKDIFF]);
  nline = -1;
  AddOptionDouble(infile,options.cParam[OPT_SECMAXLOCKDIFF],&sec->dMaxLockDiff,&nline,io->exit_param);
  if (sec->dMaxLockDiff < 0 && io->iVerbose > VERBERR) {
    fprintf(stderr,"ERROR: %s must be > 0.\n",options.cParam[OPT_SECMAXLOCKDIFF]);
    LineExit(infile,nline,io->exit_param,io->iVerbose);
  }
  if (nline != -1)
    iLineOK[nline] = 1;
  
  /* Secondary's Obliquity */

  sec->dObliquity = 0; /* Default is no obliquity */
  nline = -1;
  AddOptionDouble(infile,options.cParam[OPT_SECOBL],&sec->dObliquity,&nline,io->exit_param);
  if (param->iUnitAngle == 0) {
    if (sec->dObliquity < 0 || sec->dObliquity > PI) {
      if (io->iVerbose >= VERBERR)
	fprintf(stderr,"ERROR: %s must be in the range [0,PI].\n",options.cParam[OPT_SECOBL]);
      LineExit(infile,nline,io->exit_param,io->iVerbose);
    } 
  } else {
    if (sec->dObliquity < 0 || sec->dObliquity > 180) {
      if (io->iVerbose >= VERBERR)
	fprintf(stderr,"ERROR: %s must be in the range [0,180].\n",options.cParam[OPT_SECOBL]);
      LineExit(infile,nline,io->exit_param,io->iVerbose);
    }
    /* Change to radians */
    sec->dObliquity *= DEGRAD;
  }
  if (nline != -1)
    iLineOK[nline] = 1;
  
  /* Secondary Q */

  sec->dQ = atof(options.cDefault[OPT_SECQ]);
  nline = -1;
  AddOptionDouble(infile,options.cParam[OPT_SECQ],&sec->dQ,&nline,io->exit_param);
  if (sec->dQ < 0) {
    if (io->iVerbose >= VERBERR)
      fprintf(stderr,"ERROR: %s must be greater than 0.\n",options.cParam[OPT_SECQ]);
    LineExit(infile,nline,io->exit_param,io->iVerbose);
  }
  if (param->iTideModel == CPL && sec->dQ == 0) {
    if (io->iVerbose >= VERBERR)
      fprintf(stderr,"ERROR: %s model selected, but %s not set.\n",options.cParam[OPT_TIDEMODEL],options.cParam[OPT_SECQ]);
    exit(io->exit_param);
  }
  if (param->iTideModel == CTL && nline != -1 && io->iVerbose >= VERBINPUT) 
    fprintf(stderr,"WARNING: CTL model selected, but %s set.\n",options.cParam[OPT_SECQ]);
  if (nline != -1)
    iLineOK[nline] = 1;
  
  /* Negative value defaults to Earth radii */
  sec->dRadius = 0; /* Default is use relationship */
  nline = -1;
  AddOptionDouble(infile,options.cParam[OPT_SECRAD],&sec->dRadius,&nline,io->exit_param);
  if (sec->dRadius > 0) 
    sec->dRadius *= dLengthUnit(param->iUnitLength,io->exit_units);
  else {
    if (io->iVerbose >= VERBUNITS)
      fprintf(stderr,"WARNING: %s less than zero, using Earth units.\n",options.cParam[OPT_SECRAD]);
    sec->dRadius *= -REARTH;
  }
  if (nline != -1)
    iLineOK[nline] = 1;

  /* Secondary's Radius of Gyration */

  sec->dRG = 0.5;
  nline = -1;
  AddOptionDouble(infile,options.cParam[OPT_SECRG],&sec->dRG,&nline,io->exit_param);
  if (sec->dRG < 0) {
    if (io->iVerbose >= VERBERR)
      fprintf(stderr,"ERROR: %s must be greater than zero.\n",options.cParam[OPT_SECRG]);
    LineExit(infile,nline,io->exit_param,io->iVerbose);
  }
  if (nline != -1)
    iLineOK[nline] = 1;
  
  /* Secondary's Spin Period */

  input.dSecSpinPer = 0;
  nline =-1;
  
  AddOptionDouble(infile,options.cParam[OPT_SECSPINPER],&input.dSecSpinPer,&nline,io->exit_param);
  if (input.dSecSpinPer < 0) {
    if (io->iVerbose >= VERBUNITS) 
      fprintf(stderr,"WARNING: %s is less than 0, assuming days.\n",options.cParam[OPT_SECSPINPER]);
    input.dSecSpinPer = -input.dSecSpinPer*DAYSEC;
  } else if (input.dSecSpinPer > 0) {
    input.dSecSpinPer *= dTimeUnit(param->iUnitTime,io->exit_units);
  }
  if (nline != -1)
    iLineOK[nline] = 1;
  line.lSecSpinPer = nline;

  /* Secondary's Rotation Rate */

  sec->dSpinRate = 0; 
  nline = -1;
  AddOptionDouble(infile,options.cParam[OPT_SECSPINRATE],&sec->dSpinRate,&nline,io->exit_param);
  if (sec->dSpinRate < 0) {
    if (io->iVerbose >= VERBERR)
	fprintf(stderr,"ERROR: %s must be greater than 0.\n",options.cParam[OPT_SECSPINRATE]);
    LineExit(infile,nline,io->exit_param,io->iVerbose);
  }
  if (nline != -1) {
    sec->dSpinRate /= dTimeUnit(param->iUnitTime,io->exit_units);  
    iLineOK[nline] = 1;
  }
  line.lSecSpinRate = nline;

  /* Secondary Syncronous Eccentricity */

  sec->dSyncEcc = atoi(options.cDefault[OPT_SECSYNCECC]);
  nline = -1;
  AddOptionDouble(infile,options.cParam[OPT_SECSYNCECC],&sec->dSyncEcc,&nline,io->exit_param);
  if (sec->dSyncEcc < 0 || sec->dSyncEcc > 1) {
    if (io->iVerbose >= VERBERR) 
      fprintf(stderr,"ERROR: %s must be in the range [0,1].\n",options.cParam[OPT_SECSYNCECC]);
    LineExit(infile,nline,io->exit_param,io->iVerbose);
  }
  if (nline != -1)
    iLineOK[nline] = 1;
    
  /* Secondary Time lag */

  sec->dTau = 0;
  nline = -1;
  AddOptionDouble(infile,options.cParam[OPT_SECTAU],&sec->dTau,&nline,io->exit_param);
  if (param->iTideModel == CPL && sec->dTau > 0 && io->iVerbose >= VERBINPUT) 
    fprintf(stderr,"WARNING: Phase lag model selected, but %s set.\n",options.cParam[OPT_SECTAU]);
  if (sec->dTau < 0) {
    if (io->iVerbose >= VERBUNITS)
      fprintf(stderr,"WARNING: %s < 0, assuming seconds.\n",options.cParam[OPT_SECTAU]);
    sec->dTau = -sec->dTau;
  } else {
    /* Convert timestep to cgs */
    sec->dTau *= dTimeUnit(param->iUnitTime,io->exit_units);
  }
  if (param->iTideModel == CPL && nline != -1 && io->iVerbose >= VERBINPUT) 
    fprintf(stderr,"WARNING: CPL model selected, but %s set.\n",options.cParam[OPT_SECTAU]);
  if (nline != -1)
    iLineOK[nline] = 1;
  
  /* Rotational Velocity */

  /* Checked with SecSpinRate and SecVRot in Verify() */
  input.dSecVRot = 0;
  nline = -1;
  AddOptionDouble(infile,options.cParam[OPT_SECVROT],&input.dSecVRot,&nline,io->exit_param);
  if (input.dSecVRot < 0) {
    if (io->iVerbose >= VERBERR) 
      fprintf(stderr,"WARNING: %s is less than 0, assuming km/s.\n",options.cParam[OPT_SECVROT]);
    input.dSecVRot = - input.dSecVRot*DAYSEC/1e5;
  } else if (input.dSecVRot > 0) {
    input.dSecVRot *= dTimeUnit(param->iUnitTime,io->exit_units)/dLengthUnit(param->iUnitLength,io->exit_units);
  }
  if (nline != -1) 
    iLineOK[nline] = 1;
  line.lSecVRot = nline;
  
  /* Do variable timestep? */

  param->bVarDt = 0; 
  nline = -1;
  AddOptionInt(infile,options.cParam[OPT_VARDT],&param->bVarDt,&nline,io->exit_param);
  if (param->bVarDt != 0) {
    if ((!param->bDoForward && !param->bDoBackward) && io->iVerbose >= VERBINPUT) 
      fprintf(stderr,"WARNING: %s found, but no integration requested.\n",options.cParam[OPT_VARDT]);
  }
  if (param->bVarDt != 0 && param->bVarDt != 1) {
    if (io->iVerbose >= VERBERR)
      fprintf(stderr,"ERROR: %s must be either 0 or 1.\n",options.cParam[OPT_VARDT]);
    LineExit(infile,nline,io->exit_param,io->iVerbose);
  }
  if (nline != -1)
    iLineOK[nline] = 1;
  
  /* variable timestep coefficient */

  param->dTimestepCoeff = 0; 
  nline = -1;
  AddOptionDouble(infile,options.cParam[OPT_TIMESTEPCOEFF],&param->dTimestepCoeff,&nline,io->exit_param);
  if (param->dTimestepCoeff != 0)
    if ( (!param->bDoBackward && !param->bDoForward) && io->iVerbose >= VERBINPUT) {
    fprintf(stderr,"WARNING: %s found, but no integration requested.\n",options.cParam[OPT_TIMESTEPCOEFF]);
  }
  if (param->dTimestepCoeff < 0 && io->iVerbose >= VERBERR) {
    fprintf(stderr,"ERROR: %s must be > 0.\n",options.cParam[OPT_TIMESTEPCOEFF]);
    LineExit(infile,nline,io->exit_param,io->iVerbose);
  }
  if (nline != -1)
    iLineOK[nline] = 1;
  
  /* 
   * Get output order
   */

  nline = -1;
  param->iNumCols = AddOutputOrder(infile,options.cParam[OPT_OUTPUTORDER],12,param->cOutputOrder,&nline,output,io->iVerbose,io->exit_param);
  if (nline != -1)
    iLineOK[nline] = 1;
  
  /*
   * Now check for unrecognized commands 
   */

  Unrecognized(infile,iNumLines,iLineOK,io->exit_param);

  if (io->iVerbose >= VERBINPUT) 
    printf("Input file %s successfully read in.\n",infile);

  /* 
   * Now check for duplicate or incompatible things.
   */

  VerifyOptions(param,pri,sec,input,options,line,infile,io);

  if (io->iVerbose >= VERBINPUT) 
    printf("Input file %s successfully verified.\n",infile);
  
  VerifyOutput(param,options,output,io->iVerbose);
  
  if (io->iVerbose >= VERBINPUT) 
    printf("Output file %s successfully verified.\n",files->cLog);
  
  free(iLineOK);
}

