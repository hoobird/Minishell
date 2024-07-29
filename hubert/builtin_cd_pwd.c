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
void	builtin_pwd(char **arg, char ***envpc)
{
	(void)arg;
	char	buf[PATH_MAX];
	
	if (getcwd(buf, PATH_MAX) == NULL)
		return ;
	printf("%s\n", buf); // else if getcwd() success, print pathname to STDOUT
}

/*
function relies on chdir()

chdir(), accepts one argument, a char *, the  the DST dir
On success, will change the current process's CWD to the DST dir, retVal 0
On error, retVal -1, and errno is set

NOTE #1
chdir() looks like it will fail with an invalid arg, eg. 
when arg is:
file, and not dir
empty str
randomstr that does not refer to dir

-> no need to check if arg for chdir() is indeed a dir!

NOTE #2
chdir() accepts relative paths, ie. args that start with a .
*/
void	builtin_cd(char **arg, char ***envpc)
{
	char	*dst_dir;
	char	*tmp;
	
	dst_dir = arg[0];
	if (chdir(dst_dir) == 0)
		return ;
	/*
	{
		tmp = envpc_get_value(*envpc, "PWD");
		envpc_add(envpc, "OLDPWD", tmp);
		envpc_add(envpc, "PWD", dst_dir);
	}
	*/
}

int	main(int ac, char *av[], char **env)
{
	char	*test_abs;
	char	*test_rel_minus_1;
	char	*test_rel_plus_1;
	char	*test_rel_plus_2;
	char	*test_fail_1;
	char	*test_fail_2;
	char	*test_fail_3;
	char	*root;
	char	*oldpwd;
	
	printf("bef:\n");
	builtin_pwd(NULL, &env);
	
	test_abs = "/usr/games"; // absolute path
	test_rel_minus_1 = "./learn"; // relative path, directory nested in current directory
	test_rel_plus_1 = ".."; // relative path, one level up
	test_rel_plus_2 = ".."; // relative path, TWO levels up
	test_fail_1 = "/etc/passwd"; // expect fail, because arg is file, not dir
	test_fail_2 = ""; // empty pathname
	test_fail_3 = "123helloworld"; // some random str
	root = "/";
	oldpwd = envpc_get_value(env, "OLDPWD");

// relative path, directory nested in current directory, ie. ONE level below current directory 
	printf("\nchdir() w/ rel path, ONE level BELOW current directory:\n");
	builtin_cd(&test_rel_minus_1, &env);
	builtin_pwd(NULL, &env);

// relative path, ONE level up
	printf("\nchdir() w/ rel path, ONE level up:\n");
	builtin_cd(&test_rel_plus_1, &env);
	builtin_pwd(NULL, &env);
	
// relative path, TWO levels up
	printf("\nchdir() w/ rel path, TWO levels up:\n");
	builtin_cd(&test_rel_plus_2, &env);
	builtin_pwd(NULL, &env);

// abs path	
	printf("\nchdir() w/ abs path:\n");
	builtin_cd(&test_abs, &env);
	builtin_pwd(NULL, &env);

// file as arg
	printf("\nchdir() w/ an existing file as arg:\n");
	builtin_cd(&test_fail_1, &env);
	builtin_pwd(NULL, &env);
	
// 	empty str as arg
	printf("\nchdir() w/ empty str as arg:\n");
	builtin_cd(&test_fail_2, &env);
	builtin_pwd(NULL, &env);
	
// some random str as arg
	printf("\nchdir() w/ some random str as arg:\n");
	builtin_cd(&test_fail_3, &env);
	builtin_pwd(NULL, &env);
	
// abs path, root
	printf("\nchdir() to root:\n");
	builtin_cd(&root, &env);
	builtin_pwd(NULL, &env);
	
// abs path, $OLDPWD
	printf("\nchdir() to OLDPWD, from env:\n");
	builtin_cd(&oldpwd, &env);
	builtin_pwd(NULL, &env);
	
	return (0);
}
