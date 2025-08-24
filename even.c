/*
Module: Even.c
Purpose:
This program takes in an integer input, 'n' from the command line, and print the first n even numbers, with a 5 second pause between each output to alow the time for signals to be recieved. 
If a SIGHUP is recivedm typically given when a program is to be terminated then the module will modify the signal so that it instead prints "Ouch!". SImilarly if a SIGINT is recieved, typically
when the copy keyboard shortcut Ctrl+C is used to interrupt the program, the module will modify the signal so that it instead prints "Yeah!".
Changes:
The only major changes required for this was to change all printf statements to write functions instead as write is a low-level function and it is async-signal safe.
Author: Mohit Mittal (a1885648)
Date: August 2025
*/

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