#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
// #include <sys/types.h>

// man stat(3type)
// struct stat {
// 	dev_t      st_dev;      /* ID of device containing file */
// 	ino_t      st_ino;      /* Inode number */
// 	mode_t     st_mode;     /* File type and mode */
// 	nlink_t    st_nlink;    /* Number of hard links */
// 	uid_t      st_uid;      /* User ID of owner */
// 	gid_t      st_gid;      /* Group ID of owner */
// 	dev_t      st_rdev;     /* Device ID (if special file) */
// 	off_t      st_size;     /* Total size, in bytes */
// 	blksize_t  st_blksize;  /* Block size for filesystem I/O */
// 	blkcnt_t   st_blocks;   /* Number of 512 B blocks allocated */

// 	/* Since POSIX.1-2008, this structure supports nanosecond
// 		precision for the following timestamp fields.
// 		For the details before POSIX.1-2008, see VERSIONS. */

// 	struct timespec  st_atim;  /* Time of last access */
// 	struct timespec  st_mtim;  /* Time of last modification */
// 	struct timespec  st_ctim;  /* Time of last status change */

// #define st_atime  st_atim.tv_sec  /* Backward compatibility */
// #define st_mtime  st_mtim.tv_sec
// #define st_ctime  st_ctim.tv_sec
// };

// man inode(7)

// Regarding the st_mode field, the following macros can be used to check the file type and permissions:
// S_ISUID     04000   set-user-ID bit (see execve(2))
// S_ISGID     02000   set-group-ID bit (see below)
// S_ISVTX     01000   sticky bit (see below)

// S_IRWXU     00700   owner has read, write, and execute
// 					permission
// S_IRUSR     00400   owner has read permission
// S_IWUSR     00200   owner has write permission
// S_IXUSR     00100   owner has execute permission

// S_IRWXG     00070   group has read, write, and execute
// 					permission
// S_IRGRP     00040   group has read permission
// S_IWGRP     00020   group has write permission
// S_IXGRP     00010   group has execute permission

// S_IRWXO     00007   others (not in group) have read, write,
// 					and execute permission
// S_IROTH     00004   others have read permission
// S_IWOTH     00002   others have write permission
// S_IXOTH     00001   others have execute permission

// The following mask values are defined for the file type:
// 	S_IFMT     0170000   bit mask for the file type bit field

// 	S_IFSOCK   0140000   socket
// 	S_IFLNK    0120000   symbolic link
// 	S_IFREG    0100000   regular file
// 	S_IFBLK    0060000   block device
// 	S_IFDIR    0040000   directory
// 	S_IFCHR    0020000   character device
// 	S_IFIFO    0010000   FIFO


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

int main() {
	char filename[] = "./testfolder";

	check_file_type(filename);

	return 0;
}