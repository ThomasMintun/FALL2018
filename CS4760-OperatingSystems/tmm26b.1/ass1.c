#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/wait.h> // for wait
#include <sys/types.h> //for perror

void helpOptions(){
         printf("\n        Help Menu        \n");
         printf("-n     Requires an integer argument after '-n ' to be valid.\n");
         printf("-h     View Help Menu.\n");
         printf("-p     Displays an error message with perror.\n");
}

void simpleChain(int x){
  pid_t childpid = 0;
  int i;

  for(i = 1; i < x; i++) {
    if((childpid = fork()))
      break;
  }
  //sleep(10);
  //childpid = wait(NULL);
  fprintf(stderr, "i: %d process ID: %ld parent ID: %ld child ID: %ld\n ", i, (long)getpid(), (long)getppid(), (long)childpid);
}

int main(int argc, char *argv[]){
  int x, opt;
  char forPerror[60];

  if (argc !=2){//error message
    fprintf(stderr, "Usage: %s processes\n", argv[0]);
  }

  while((opt = getopt(argc,argv, "n:hp")) != -1){
    switch(opt){
      case 'h':
        helpOptions(); 
        break;
      case 'n':
        x = atoi(optarg);
        simpleChain(x);
        break;
      case ':':
        fprintf(stderr, "%s: option '-%c' requires the number of processes and characters \n", argv[0], optopt);
        break;
      case 'p':
        snprintf(forPerror, sizeof forPerror, "%s: Error: Detailed error message ", argv[0]);
        perror(forPerror);
        break;
      case '?':
        break;
      default:
        fprintf(stderr, "%s: option '-%c' is invalid: ignored\n", argv[0], optopt);
        break;
      }
  }
return 0;
}
