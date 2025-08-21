#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
// Signal handler function to handle signals accordingly
void signal_handler(int sig) {
  // If HUP signal received then print "Ouch!" in terminal
  if (sig == SIGHUP) {
    write(1, "Ouch!\n", sizeof("Ouch!\n") - 1);
  }
  // If INT signal received then print "Yeah!" in terminal
  if (sig == SIGINT) {
    write(1, "Yeah!\n", sizeof("Yeah!\n") - 1);
  }
}

// Main function tasked with handling signals and printing even numbers
int main(int argc, char *argv[]) {

  signal(SIGHUP, signal_handler);
  signal(SIGINT, signal_handler);
  // Convert command line input to int 
  int n = atoi(argv[1]);
  //Check and print first n even numbers
  for (int i = 0; i < n*2; i +=2) {
    if (i % 2 == 0) {
      printf("%d\n", i);
      // Pause to give program enough time to receive signals
      sleep(5);
    }
  }

  return 0;
}
