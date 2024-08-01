#include <unistd.h> // for execve()

/*
execve() can be called: 
(1) in a child process
(2) in a parent process 

builtins for minishell, that manipulate the environment, should be:
- called in the parent process, so that the changes to the environment persists
because if you execve the builtins in a child process,
	when the child proc returns, the changes to its environment will be lost
	
eg. it does not make sense to execve(/bin/cd) in a child process

*/



int main(void)
{
	
	return (0);
}
