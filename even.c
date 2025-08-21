#include <stdio.h>
#include <unistd.h>
#include <sys/types.h> 
#include <signal.h> 
void hup_handler(int sig){
    printf("\nOuch!\n");
}

void int_handler(int sig){
    printf("\nYeah!\n");
}

int main(void) {
    int n;
    
    printf("Enter a number: ");
    scanf("%d", &n);

    signal(SIGHUP, hup_handler);
    signal(SIGINT, int_handler);

    for (int i = 0; i < n; i++) {
        if (i % 2 == 0) {
            printf("%d\n", i);
            sleep(5);
        }
    }

    return 0;
}

