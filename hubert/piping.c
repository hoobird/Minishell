#include "minishell.h"

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