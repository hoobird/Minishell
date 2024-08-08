#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

// int main() {
//     char *input;
//     while ((input = readline("Enter command: ")) != NULL) {
//         if (*input) {
//             add_history(input);
//         }
//         printf("You entered: %s\n", input);
//         free(input);
//     }
//     return 0;
// }

void handle_signal(int signal) {
    write(1, "\n", 1);
    rl_on_new_line();
    // rl_replace_line("Enter command: ",0);
    rl_redisplay();
}

int main() {
    // Set up the signal handler
    signal(SIGINT, handle_signal);
    // signal(SIGQUIT, handle_signal);
    // signal(SIG, handle_signal);
    char* input;
    signal(SIGINT, handle_signal);
    while ((input = readline("Enter command: ")) != NULL) 
    {
        if (*input) {
            add_history(input);
        }
        printf("You entered: %s\n", input);
        free(input);
    }

    return 0;
}
