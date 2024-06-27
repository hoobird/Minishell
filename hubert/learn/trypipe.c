// For read, write, close, dup, dup2, execve, fork, pipe, access
# include <unistd.h>
/* For malloc, free, exit */
# include <stdlib.h>
/* For open */
# include <fcntl.h>
/* For wait, waitpid */
# include <sys/wait.h>
# include <errno.h>
/* For perror */
# include <stdio.h>

// C program to illustrate 
// pipe system call in C 
#include <stdio.h> 
#include <unistd.h> 
#define MSGSIZE 16 
char* msg1 = "hello, world #1"; 
char* msg2 = "hello, world #2"; 
char* msg3 = "hello, world #3"; 

int main() 
{ 
	char inbuf[MSGSIZE]; 
	int p[2], i; 

	if (pipe(p) < 0) 
		exit(1); 

	/* continued */
	/* write pipe */

	write(p[1], msg1, MSGSIZE); 
	write(p[1], msg2, MSGSIZE); 
	write(p[1], msg3, MSGSIZE);
    close(p[1]);

	for (i = 0; i < 4; i++) { 
		/* read pipe */
		if (read(p[0], inbuf, MSGSIZE) > 0) 
		    printf("% s\n", inbuf);
        else
            printf("EOF reached\n");
	} 
	return 0; 
} 
