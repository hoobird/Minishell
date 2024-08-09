#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>

// int main() {
//     pid_t pid;
//     int fd[2];
//     char inbuf[100];

//     if (pipe(fd) == -1){
//         printf("An error occured with the opening of the pipe");
//         return 1;
//     }
//     // created pipe, now trying to fork
//     pid = fork();
//     if (pid == -1) {
//         // Error occurred while forking
//         perror("fork fucked");
//         return 1;
//     } else if (pid == 0) {
//         // Child process
//         printf("Hello from the child process!\n");
//         write(fd[1], "Child process writes things", 28);
//     } else {
//         // Parent process
//         printf("Hello from the parent process!\n");
//         read(fd[0], inbuf, 28); 
//         printf("% s\n", inbuf); 
//     }

//     return 0;
// }

int main()
{
    pid_t   pid;
    char    *line;
    int status;

    pid = fork();
    if (pid == 0)
    {
        while (1)
        {line = readline("> ");
        printf("%s\n",line);
        }
        exit(0);
    }
    waitpid(pid, &status, 0);
    printf("status: %d\n", status);
    return (0);
}