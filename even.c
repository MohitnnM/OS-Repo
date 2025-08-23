#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
/* Signal handler function to handle signals accordingly */
void signal_handler(int sig) {
  /* If HUP signal received then print "Ouch!" in terminal */
  if (sig == SIGHUP) {
    write(1, "Ouch!\n", sizeof("Ouch!\n") - 1);
  } /* if statement for HUP signal*/
  /* If INT signal received then print "Yeah!" in terminal */
  if (sig == SIGINT) {
    write(1, "Yeah!\n", sizeof("Yeah!\n") - 1);
  } /* if statement for INT signal*/
} /* signal_handler */

/* Main function tasked with handling signals and printing even numbers */
int main(int argc, char *argv[]) {
  /* Set up the signal handlers before inputs */
  signal(SIGHUP, signal_handler);
  signal(SIGINT, signal_handler);
  
  int n = atoi(argv[1]); /* Convert command line argument to int */
  /* Check and print first n even numbers */
  for (int i = 0; i < n*2; i +=2) {
    if (i % 2 == 0) {
      printf("%d\n", i); /* Print the even number */
      sleep(5); /* Pause to give program enough time to receive signals */
    } /* if statement */
  } /* for loop */

  return 0;
} /*main*/ 