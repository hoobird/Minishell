# include "minishell.h"

// check file type directory or file
int	check_file_type(char *path)
{
	struct stat fileStat;

	if (stat(path, &fileStat) == -1) {
		perror("stat failed");
		return (-1);
	}
	if ((fileStat.st_mode & S_IFMT) == S_IFREG)
		return (1); // regular file
	else if ((fileStat.st_mode & S_IFMT) == S_IFDIR)
		return (2); // directory
	else
		return (0); // other type of file
}

// check for read / write permissions
// possible mode are R_OK, W_OK, X_OK, F_OK
// R_OK - check for read permission
// W_OK - check for write permission
// X_OK - check for execute permission (DONT NEED THIS FOR NOW)
// F_OK - check for file existence (DONT NEED THIS FOR NOW)
// return 0 if permission denied
// return 1 if permission granted
int	check_file_permissions(char *filename, int mode)
{
	if (access(filename, mode) == -1)
	{
		if (mode == R_OK)
			printerror("minishell: %s: No such file or directory\n");
		else if (mode == W_OK)
			printerror("minishell: %s: Permission denied\n");
		return (0);
	}
	return (1);
}