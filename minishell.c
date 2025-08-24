/*********************************************************************
Program : miniShell Version : 1.3
--------------------------------------------------------------------
skeleton code for linix/unix/minix command line interpreter
--------------------------------------------------------------------
File : minishell.c
Compiler/System : gcc/linux
********************************************************************/
/*
To complete part 2 of Assignment 1 there were a variety of changes that I needed to make, particularly concering the application of background tasks. Intially I decided to 
that using the struct to save information about the jobs and then when the jobs where being forked, I would check if the job was a background or a foreground task using the
flag 'background_tasks'.In the parent process, if it was background then I would add the 'job_id', 'pid_num', and the command  to the 'bg_jobs' array. otherwise I would simply 
wait until the job was complete. once the I would have a while loop that would keep going until there were no finished jobs left, and in that loop after each job was forked
the program would check to see if any jobs were finished. If they were finished then their status would be changed to finish and they would be removed from the array. Unfortunately
this raised the error that the done message would not be printed until a new command was entered. Instead I decided to implement a signal handler as used in part 1. It completed
the same function as it was before in the while loop, only difference being that it would print the message whenever any background task was finished instead of waiting for other
foreground tasks.

*/
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

/* a basic structure for background job information, consisting of the job_id (what number job it is), the pid_num (what the PID number of the job is), cmd (what the command actually is), and 
job_status (is the job still running) */
struct background_jobs {
  int job_id;
  int pid_num;
  char cmd[NL];
  int job_status;
};

struct background_jobs bg_jobs[NV]; /* an array to store all background jobs until done */
int num_jobs = 0; /* number of background jobs */

/* a signal handler function to recieve signals and constantly check if any background jobs have been completed yet*/
void sig_handler(int sig)
{
  int stat;
  pid_t job_finished;
  /* while loop that keeps going until waitpid return a number less than 0 to indicate there are no finished child processes remaining */
  while ((job_finished = waitpid(-1, &stat, WNOHANG)) > 0) {
    for (int j = 0; j < num_jobs; ++j) {
      if (bg_jobs[j].pid_num == job_finished) {
        bg_jobs[j].job_status = 0; /* change finished job's status to finished*/
        printf("[%d]+ Done                 %s\n", bg_jobs[j].job_id, bg_jobs[j].cmd);
        fflush(stdout);
        /* loop throught the bg_jobs array, starting at the current job, to remove the finished job*/
        for (int k = j; k < num_jobs - 1; ++k) {
          bg_jobs[k] = bg_jobs[k + 1];
        }
        num_jobs--;
        break;
      }
    }
  }
}  
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
  int frkRtnVal; /* value returned by fork sys call */
  int background_tasks; /* number of background tasks*/
  char *v[NV];          /* array of pointers to command line tokens
                         */
  char *sep = " \t\n";  /* command line token separators */
  int i;                /* parse index */
  signal(SIGCHLD, sig_handler);
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
        /* when there is argument, then check previous if argument is &, if so
         * then increment background_tasks */
        if (strcmp(v[i - 1], "&") == 0) {
          background_tasks = 1; /* set flag to true*/
          v[i - 1] = NULL; /* remove the & from the argument list */
        }
        break;
      }
    }
    /* assert i is number of tokens + 1 */
    /* implement cd to change to current directory */
    if (strcmp(v[0], "cd") == 0) {
      /* error check to see if the location is specified*/
      if (v[1] == NULL) {
        perror("cd: argument missing");
      } else {
        /* run cd is valid otherwise return error if directory is not found*/
        if (chdir(v[1]) != 0) {
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
        int stat;
        /* if statement runs waitpid if it is a foreground task */
        if (background_tasks == 0) {
          waitpid(frkRtnVal, &stat, 0);
        } else if (background_tasks == 1) { /* otherwise if it is a background task then it adds the neccasary information to the bg_jobs */
          bg_jobs[num_jobs].job_id = num_jobs + 1; /* increase number of jobs*/
          bg_jobs[num_jobs].pid_num = frkRtnVal; /* save the job PID number*/
          bg_jobs[num_jobs].cmd[0] = '\0';  /* reset it to begin with an empty string */
          /* loop through the command line input until there is no more commands and concatenate it into the string */
          for (int k = 0; v[k] != NULL; ++k) {
            strcat(bg_jobs[num_jobs].cmd, v[k]);
          }
          bg_jobs[num_jobs].job_status = 1; /* set the job_status flag to true*/
          printf("[%d] %d\n", bg_jobs[num_jobs].job_id, frkRtnVal); /* print the job_id and pid_num to indicate this job is now running in the background*/
          num_jobs++; /* increase number of background jobs */
        }
        // REMOVE PRINTF STATEMENT BEFORE SUBMISSION
        // printf("%s done \n", v[0]);
        break;
      }
    } /* switch */
  } /* while */
} /* main */