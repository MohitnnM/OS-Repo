#include <stdio.h>
#include <unistd.h>
#include <sys/types.h> 
#include <signal.h> 
void hup_handler(int sig){
    printf("Ouch!\n");
}

void int_handler(int sig){
    printf("\nYeah!\n");
}

int main(int arg, char *argv[]) {
    signal(SIGHUP, hup_handler);
    signal(SIGINT, int_handler);

    int n = atoi(argv[1]);

    for (int i = 0; i < n; i++) {
        if (i % 2 == 0) {
            printf("%d\n", i);
            sleep(5);
        }
    }

    return 0;
}

