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
    if (i == 0)
    {
        printf("No pipes generated\n");
    }
}

void    printcommandlist(t_command_args **command_arg_list)
{
    int i;

    i = 0;
    while (command_arg_list[i])
    {
        printf("Command %d:\n", i);
        print_tokenlist(command_arg_list[i]->tokenlist);
        printf("readfd: %d\n", command_arg_list[i]->readfd);
        printf("writefd: %d\n", command_arg_list[i]->writefd);
        printf("cancelexec: %d\n", command_arg_list[i]->cancelexec);
        i++;
    }
    if (i == 0)
    {
        printf("No commands generated\n");
    }
}        
t_command_args  **upgrade_struct_to_list(t_token **tokenlistlist)
{
    t_command_args  **output;
    int             i;

    output = ft_calloc(sizeof(t_command_args *), tokenlistlist_len(tokenlistlist) + 1);
    if (!output)
    {
        printerror("Upgrade struct failed\n");
        return (NULL);
    }
    i = 0;
    while (i < tokenlistlist_len(tokenlistlist))
    {
        output[i] = ft_calloc(sizeof(t_command_args), 1);
        if (!output[i])
        {
            printerror("Upgrade struct failed\n");
            return (NULL);
        }
        output[i]->tokenlist = tokenlistlist[i];
        output[i]->readfd = STDIN_FILENO;
        output[i]->writefd = STDOUT_FILENO;
        output[i]->cancelexec = 0;
        output[i]->pid = 0;
        i++;
    }
    return (output);
}

void    generate_pipes(t_command_args **command_args_list, int no_pipes)
{
    int i;
    int **pipefd;

    if (no_pipes <= 0)
        return ;
    pipefd = ft_calloc(sizeof(int *), no_pipes);
    if (!pipefd)
    {
        printerror("Generate pipes failed\n");
        return ;
    }
    i = 0;
    while (i < no_pipes)
    {
        pipefd[i] = ft_calloc(sizeof(int), 2);
        if (!pipefd[i])
        {
            printerror("Generate pipes (Calloc error) failed\n");
            return ;
        }
        if (pipe(pipefd[i]) == -1)
        {
            printerror("Generate pipes failed\n");
            return ;
        }
        command_args_list[i]->writefd = pipefd[i][1];
        command_args_list[i + 1]->readfd = pipefd[i][0];
        i++;
    }
    while (i > 0)
    {
        i--;
        free(pipefd[i]);
    }
    free(pipefd);
    return ;
}

t_command_args	**upgrade_struct_generate_pipes(t_token **tokenlistlist)
{
    t_command_args  **output;
    int        no_of_pipes;

    output = upgrade_struct_to_list(tokenlistlist);
    if (!output)
        return (NULL);
    no_of_pipes = tokenlistlist_len(tokenlistlist) - 1;
    if (no_of_pipes > 0)
        generate_pipes(output, no_of_pipes);
    return (output);
}



