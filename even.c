#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h> 
#include <signal.h> 
void signal_handler(int sig){
    if (sig == SIGHUP) {
        printf("\nSIGHUP received!\n");
    } else if (sig == SIGINT) {
        printf("\nSIGINT received!\n");
    }
    printf("\nOuch!\n");
    printf("\nYeah!\n");
}

int main(int arg, char *argv[]) {
    signal(SIGHUP, signal_handler);
    signal(SIGINT, signal_handler);

    int n = atoi(argv[1]);

    for (int i = 0; i < n; i++) {
        if (i % 2 == 0) {
            printf("%d\n", i);
            sleep(5);
        }
    }

    return 0;
}

