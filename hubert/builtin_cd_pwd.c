# include "minishell.h"

/* 
function relies on getcwd()

Q: why die die have to use getcwd()?
A: because it is possible to unset $PWD...

ie. 
If $PWD is unset in env, envpc_get_value(envpc, PWD) will return NULL
In comparison, getcwd() should still work, even when $PWD is unset in env

getcwd() requires TWO args: char buf[size], size_t size
PATH_MAX is defined in limits.h, should be 4096

On failure, getcwd() returns NULL, and errno var is set, to indicate the error

On success, getcwd() returns a ptr to a str containing the pathname of the cwd
The buf char array is also set the pathname

NOTE. the calling function will handle any redirect from STDOUT to alternate FD?
*/
int	builtin_pwd(char **arg, char ***envpc)
{
	(void)arg;
	char	buf[PATH_MAX];
	
	if (getcwd(buf, PATH_MAX) == NULL)
		return (1);
	else
	{
		printf("%s\n", buf); // else if getcwd() success, print pathname to STDOUT
		return (0);
	}
}

/*
function relies on chdir()

chdir(), accepts one argument, a char *, the  the DST dir
On success, will change the current process's CWD to the DST dir, retVal 0
On error, retVal -1, and errno is set

NOTE
chdir() seems to accept as an arg, relative paths, eg.
./learn	# change to "child" directory, 1 level down
. 		# change to current directory
../..	# change to "grandparent" directory, 2 levels up

TO DO # 31 July 2024
need to check for 2 conditions:

if arg is directory # check_file_type(arg)
AND 
if arg can be read, permissions # check_file_permissions (arg, R_OK)


cd "" # does nothing, stay in the same/current directory
cd # no arg, change to HOME
cd abc 123, # more than one arg, stay in the same/current directory, print err msg

*/

/*
helper function
to determine if char** is valid for builtin_cd

Returns the number of elements in the char **
NOTE. EXCLUDES the NULL pointer at the end of the char ** array
*/
static int len_ch_star_star(char **css)
{
	int index;

	index = 0;
	while (css && css[index])
		index++;
	return (index);
}

int	builtin_cd(char **arg, char ***envpc)
{
	char	*dst_dir;
	char	*tmp;
	
	if (len_ch_star_star(arg) >= 2)
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		return (1);
	}

	else if (len_ch_star_star(arg) == 0)
	{
		dst_dir = envpc_get_value(*envpc, "HOME");
		if (dst_dir == NULL)
		{
			ft_putstr_fd("minishell: cd: HOME not set\n", 2);		
			return (1);
		}
		else
		{
			if (chdir(dst_dir) == 0)
			{
				tmp = envpc_get_value(*envpc, "PWD");
				envpc_add(envpc, "OLDPWD", tmp);
				envpc_add(envpc, "PWD", dst_dir);		
				return (0);
			}
			else if (chdir(dst_dir) == -1)
			{
				ft_putstr_fd("minishell: cd: ", 2);
				ft_putstr_fd(dst_dir, 2);
				if (access(dst_dir, F_OK) == -1)
				{
					ft_putstr_fd(" no such file or directory\n", 2);			
					return (1);
				}
				else if (access(dst_dir, R_OK) == -1)
				{
					ft_putstr_fd(" permission denied\n", 2);
					return (1);
				}
				else
				{
					ft_putstr_fd(" not a directory\n", 2);
					return (1);
				}
			}
		}
	}

	else if (len_ch_star_star(arg) == 1)
	{
		dst_dir = arg[0];
		if (ft_strncmp(dst_dir, "", 1) == 0)
			return (0);
		else if (chdir(dst_dir) == 0)
		{
			tmp = envpc_get_value(*envpc, "PWD");
			envpc_add(envpc, "OLDPWD", tmp);
			envpc_add(envpc, "PWD", dst_dir);
			return (0);
		}
		else if (chdir(dst_dir) == -1)
		{
			ft_putstr_fd("minishell: cd: ", 2);
			ft_putstr_fd(dst_dir, 2);
			if (access(dst_dir, F_OK) == -1)
			{
				ft_putstr_fd(" no such file or directory\n", 2);			
				return (1);
			}
		    else if (access(dst_dir, R_OK) == -1)
		    {
				ft_putstr_fd(" permission denied\n", 2);
		    	return (1);
		    }
		    else
		    {
				ft_putstr_fd(" not a directory\n", 2);
				return (1);
		    }
		}
	}
	return (0);
}

/*
int	main(int ac, char *av[], char **env)
{
	char	**envpc;
	char	*test_abs;
	char	*test_rel_minus_1;
	char	*test_rel_plus_1;
	char	*test_rel_plus_2;
	char	*test_fail_1;
	char	*test_fail_2;
	char	*test_fail_3;
	char	*root;
	char	*oldpwd;
	char	*no_arg;
	
	char	*arg[3];
	
	envpc = envp_copy(env);
	
	printf("bef:\n");
	builtin_pwd(NULL, &env);
	printf("\n");

	char	*multi_arg[3];
	
	//multi_arg[0] = "hello";
	multi_arg[0] = "./cannotread";
	//multi_arg[1] = "world";
	multi_arg[1] = NULL;
	multi_arg[2] = NULL;
	
	//printf("\nchdir() with more than one arg\n");
	builtin_cd(multi_arg, &envpc);
	printf("\n");
	builtin_pwd(NULL, &envpc);	
	
	//envpc_print(envpc);
	envpc_free(&envpc);
	
	return (0);
}
cc builtin_cd_pwd.c builtin_env.c printerror.c ../Libft/libft.a

TEST CASES:

absolute path, valid AND invalid
relative path, valid AND invalid
"" // empty str
NULL char ** array
valid dir, with no read permissions
*/
