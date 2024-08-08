#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void handle_signal(int signal) {
    printf("Caught signal %d\n", signal);
}

int main() {
    // Set up the signal handler
    signal(SIGINT, handle_signal);
    signal(SIGQUIT, handle_signal);
    signal(EOF, handle_signal);
    // signal(SIG, handle_signal);
    
    // Loop indefinitely
    while (1) {
        printf("Running...\n");
        sleep(1);
    }
    
    return 0;
}
