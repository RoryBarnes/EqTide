#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include "eqtide.h"
#include "options.h"
#include "output.h"

void WriteHelp(OUTPUT output,OPTIONS options,char exe[]) {
    int i;
    char ESC=27;

    printf("\n\t\t\tHelp Message for %s\n",exe);
    printf("\nWritten by Rory Barnes\n");
    printf("\n%s is a general purpose tidal integrator. It takes ",exe);
    printf("an input file consisting of options and initial conditions ");
    printf("and simulates tidal evolution, along with other secondary ");
    printf("parameters, forward and/or backward in time. This help ");
    printf("describes the basics of the input file.\n\n");

    printf("----- Command Line Options -----\n\n");
    printf("%c[1m-v, -verbose%c[0m -- Maximum verbosity, i.e. display all warnings and updates.\n",ESC,ESC);
    printf("%c[1m-q, -quiet%c[0m   -- No verbosity, i.e. nothing printed to device.\n",ESC,ESC);
    printf("%c[1m-h, -help%c[0m    -- Display this message.\n\n",ESC,ESC);

    printf("----- Input File -----\n\n");
    printf("Comments/White Space: Any characters to the right of a # sign ");
    printf("are treated as a comment and are ignored. All white space is ");
    printf("ignored.\n");
    printf("Options must be the first string on any line, and must be ");
    printf("written exactly as listed below.\n");
    printf("Arguments may have any format, and need only be unambiguous.\n\n");

    printf("----- Input Options -----\n\n");
    
    for (i=0;i<NUMOPT;i++) {
	if (memcmp(options.cParam[i],"null",4)) {
	    printf("%c[1m%s%c[0m (",ESC,options.cParam[i],ESC);
	    if (options.iType[i] == 0)
		printf("Bool");
	    else if (options.iType[i] == 1) 
		printf("Int");
	    else if (options.iType[i] == 2)
		printf("Double");
	    else if (options.iType[i] == 3)
		printf("String");
	    else if (options.iType[i] == 4)
		printf("Array");
	    printf(") -- %s (Default = %s).\n",options.cDescr[i],options.cDefault[i]);
	}
    }
    
    printf("\n----- Output Options -----\n\n");
    printf("These options follow the argument %s.\n",options.cParam[OPT_OUTPUTORDER]);

    for (i=0;i<NUMOUT;i++) {
      if (memcmp(output.cParam[i],"null",4)) {
	if (output.iNeg[i] == 0)
	  printf("%c[1m[-]%c[0m",ESC,ESC);
	printf("%c[1m%s%c[0m -- %s.",ESC,output.cParam[i],ESC,output.cDescr[i]);
	if (output.iNeg[i] == 0)
	  printf(" [%s]",output.cNeg[i]);
	printf("\n");
      }
    }
    
}

int main(int argc,char *argv[]) {
  int i,iv,iq;
  double f;
  PARAM param;
  SECONDARY sec;
  PRIMARY pri;
  HZ hz;
  IO io;
  FILES files; 
  OUTPUT output;
  OPTIONS options;

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
  param.iVerbose = -1;
  /* Check for flags */
  for (i=1;i<argc;i++) {
      if (memcmp(argv[i],"-v",2) == 0) {
	  param.iVerbose = 5;
	  iv=i;
      }
      if (memcmp(argv[i],"-q",2) == 0) {
	  param.iVerbose = 0;
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
	ReadOptions(argv[i],options,&param,&pri,&sec,&hz,&files,&output,io);

	  if (param.iVerbose > 0) 
	      printf("Read %s.\n",argv[i]);
	  if (param.bLog) {
	    WriteLog(param,pri,sec,hz,output,files,io,0);
	      if (param.iVerbose > 1)
		  printf("Log file written.\n");
	  }

	  /* Reverse Integration */
	  if (param.bDoBackward) {
	      if (param.iTideModel == 0) 
		TideRewind_CPL2(param,pri,sec,hz,output,files,io);
	      if (param.iTideModel == 9)
		TideRewind_CTL8(param,pri,sec,hz,output,files,io);
	  if (param.bLog) 
	    WriteLog(param,pri,sec,hz,output,files,io,-1);

	  if (param.iVerbose > 0) 
	    printf("Completed reverse integration.\n");
	  }

	  /* Forward Integration */
	  if (param.bDoForward) {
	      if (param.iTideModel == 0)
		TideForward_CPL2(param,pri,sec,hz,output,files,io);
	      if (param.iTideModel == 9)
		TideForward_CTL8(param,pri,sec,hz,output,files,io);
	  if (param.bLog) 
	    WriteLog(param,pri,sec,hz,output,files,io,1);
	  if (param.iVerbose > 0) 
	    printf("Completed forward integration.\n");
	 
	  }
      }
  }

  exit(0);
}
