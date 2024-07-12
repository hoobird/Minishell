#include "../includes/minishell.h"

// Test case 
char **commands[4] = {
    (char *[]){"cat", "testfile.txt", NULL},
    (char *[]){"wc", "-l", NULL},
    (char *[]){"cat", "-e", NULL},
    NULL
};

int **generatepipes(int n)
{
    int **pipes;
    int i;

    pipes = (int **)ft_calloc(n + 1, sizeof(int *));
    i = 0;
    while (i < n)
    {
        pipes[i] = (int *)ft_calloc(3, sizeof(int));  // {pipeOUT, pipeIN, NULL}
        pipe(pipes[i]);
        i++;
    }   
    return (pipes);
}

int execute(char ***cmds)
{
    int cmdindex;
    int **pipelist;

    pipelist = generatepipes(2);
    cmdindex = 0;
    while (cmds[cmdindex])
    {
        if (fork() == 0) // child process
        {
            printf("Child process %d\n", cmdindex);
            // prep for piping
            if (cmdindex == 0)
            {
                dup2(pipelist[cmdindex][1], 1); // redirect stdout to pipe write
            }
            else if (cmds[cmdindex + 1] == NULL)
            {
                dup2(pipelist[cmdindex - 1][0], 0); // redirect stdin to pipe read
            }
            else
            {
                dup2(pipelist[cmdindex - 1][0], 0); // redirect stdin to previous pipe read
                dup2(pipelist[cmdindex][1], 1); // redirect stdout to current pipe write
            }
            printf("Executing%d %s\n",cmdindex, cmds[cmdindex][0]);
            execve(ft_strjoin("/bin/", cmds[cmdindex][0]), cmds[cmdindex], NULL);
            exit(1);
        }
        // parent process
        printf("Parent process %d\n", cmdindex);
        // close pipes
        if (cmdindex == 0)
            close(pipelist[cmdindex][1]); // close write end of pipe
        else if (cmds[cmdindex + 1] == NULL)
            close(pipelist[cmdindex - 1][0]); // close read end of pipe
        else
        {
            close(pipelist[cmdindex - 1][0]); // close read end of previous pipe 
            close(pipelist[cmdindex][1]); // close write end of current pipe
        }
        while (waitpid(-1, NULL, 0))
            ;
        cmdindex++;
    }
    return (0);
}

void    printpipelist(int **pipelist)
{
    int i;

    i = 0;
    while (pipelist[i])
    {
        printf("Pipe %d: %d %d\n", i, pipelist[i][0], pipelist[i][1]);
        i++;
    }
}

int main()
{
    // int **pipelist;
    // pipelist = generatepipes(2);
    // printpipelist(pipelist);
    execute(commands);
    return (0);
}