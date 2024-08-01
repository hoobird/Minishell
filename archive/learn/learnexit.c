#include <unistd.h>
#include <stdlib.h>




int	main()
{
	builtin_exit(99);
	write(1, "This should not be printed\n", 28);
	return (0);
}