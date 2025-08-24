/*********************************************************************
Program : miniShell Version : 1.3
--------------------------------------------------------------------
skeleton code for linix/unix/minix command line interpreter
--------------------------------------------------------------------
File : minishell.c
Compiler/System : gcc/linux
********************************************************************/
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#define NV 20  /* max number of command tokens */
#define NL 100 /* input buffer size */
char line[NL]; /* command input buffer */

struct background_jobs{
  int job_id;
  int pid_num;
  char cmd[NL];
  int job_status;
};

/* shell prompt */
void prompt(void) {
  // ## REMOVE THIS 'fprintf' STATEMENT BEFORE SUBMISSION
  // fprintf(stdout, "\n msh> ");
  fflush(stdout);
}
/* argk - number of arguments */
/* argv - argument vector from command line */
/* envp - environment pointer */
int main(int argk, char *argv[], char *envp[]) {
  int frkRtnVal;       /* value returned by fork sys call */
  struct background_jobs bg_jobs[NV]; /* an array to store all background jobs until done */
  int num_jobs = 0; /* number of background jobs */
  int stat;
  int background_tasks; /* number of background tasks*/
  char *v[NV];         /* array of pointers to command line tokens
                        */
  char *sep = " \t\n"; /* command line token separators */
  int i;               /* parse index */
  /* prompt for and process one command line at a time */
  while (1) { /* do Forever */
    prompt();
    fgets(line, NL, stdin);
    fflush(stdin);
    // This if() required for gradescope
    if (feof(stdin)) { /* non-zero on EOF */
      exit(0);
    }
    if (line[0] == '#' || line[0] == '\n' || line[0] == '\000') {
      continue; /* to prompt */
    }
    background_tasks = 0; /* set the background_tasks to 0 again before parsing input */
    v[0] = strtok(line, sep);
    for (i = 1; i < NV; i++) {
      v[i] = strtok(NULL, sep);
      if (v[i] == NULL) {
        /* when there is argument, then check previous if argument is &, if so then increment background_tasks */
        if (strcmp(v[i-1],"&") == 0){
          background_tasks = 1;
          v[i-1] = NULL; /* remove the & from the argument list */
        }
        break;
      }
    }
    /* assert i is number of tokens + 1 */
    
    /* implement cd to change to current directory */
    if (strcmp(v[0],"cd") == 0)
    {
      /* error check to see if the location is specified*/
      if(v[1] == NULL){
        perror("cd: argument missing");
      }
      else{
        /* run cd is valid otherwise return error if directory is not found*/
        if(chdir(v[1]) != 0){
          perror("cd: directory not found");
        }
      }
      /* continue to skip fork*/
      continue;
    }
    /* fork a child process to exec the command in v[0] */
    switch (frkRtnVal = fork()) {
      case -1: /* fork returns error to parent process */
      {
        perror("fork");
        continue;
      }
      case 0: /* code executed only by child process */
      {
        execvp(v[0], v);
        perror("execvp");
        exit(1);
      }
      default: /* code executed only by parent process */
      {
        if (background_tasks == 0){
          waitpid(frkRtnVal, &stat, 0);
          perror("waitpid");
        }
        else if (background_tasks == 1){
          bg_jobs[num_jobs].job_id = num_jobs + 1;
          bg_jobs[num_jobs].pid_num = frkRtnVal;
          strcpy(bg_jobs[num_jobs].cmd, v[0]);
          bg_jobs[num_jobs].job_status = 1;
          printf("[%d] %d\n", bg_jobs[num_jobs].job_id, frkRtnVal);
          num_jobs++;
        }
        // REMOVE PRINTF STATEMENT BEFORE SUBMISSION
        // printf("%s done \n", v[0]);
        break;
      }
    } /* switch */
    pid_t job_finished;
    while ((job_finished = waitpid(-1, &stat, WNOHANG)) > 0){
      for(int j = 0; j < num_jobs; ++j){
        if(bg_jobs[j].pid_num == job_finished){
          bg_jobs[j].job_status = 0;
          printf("[%d]+ Done                 %s\n", bg_jobs[j].job_id, bg_jobs[j].cmd);
        }
      }
    }
  } /* while */
} /* main */