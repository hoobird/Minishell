#include "minishell.h"

void	builtin_exit(int status)
{
	unsigned char	exit_status;

	exit_status = (unsigned char)status;
	exit(exit_status);
}