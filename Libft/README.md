# Libft - Your Very First Own Library

Libft is a C programming project focused on creating a personal library of general-purpose functions. It serves as a foundation for future C projects by providing essential functionalities commonly found in the standard C library (libc).

## Functions

### Part 1 - Libc Functions

| Function       | Description                                       |
|----------------|---------------------------------------------------|
| ft_isalpha     | Checks if a character is an alphabetic character. |
| ft_isdigit     | Checks if a character is a digit.                 |
| ft_isalnum     | Checks if a character is alphanumeric.            |
| ft_isascii     | Checks if a character is a 7-bit ASCII character. |
| ft_isprint     | Checks if a character is printable.               |
| ft_strlen      | Computes the length of a string.                  |
| ft_memset      | Fills memory with a constant byte.                |
| ft_bzero       | Sets a buffer to zero.                            |
| ft_memcpy      | Copies memory area.                               |
| ft_memmove     | Copies memory area, allowing overlap.             |
| ft_strlcpy     | Copies a string to a buffer with a size limit.    |
| ft_strlcat     | Appends a string to a buffer with a size limit.   |
| ft_toupper     | Converts a lowercase letter to uppercase.         |
| ft_tolower     | Converts an uppercase letter to lowercase.       |
| ft_strchr      | Locates the first occurrence of a character.      |
| ft_strrchr     | Locates the last occurrence of a character.       |
| ft_strncmp     | Compares two strings.                             |
| ft_memchr      | Locates the first occurrence of a character.      |
| ft_memcmp      | Compares memory areas.                            |
| ft_strnstr     | Locates a substring in a string.                  |
| ft_atoi        | Converts a string to an integer.                  |

### Part 2 - Additional Functions

| Function       | Description                                       |
|----------------|---------------------------------------------------|
| ft_substr      | Allocates and returns a substring of a string.    |
| ft_strjoin     | Concatenates two strings.                         |
| ft_strtrim     | Removes specified characters from a string.       |
| ft_split       | Splits a string into an array of substrings.      |
| ft_itoa        | Converts an integer to a string.                  |
| ft_strmapi     | Applies a function to each character of a string. |
| ft_striteri    | Applies a function to each character of a string. |
| ft_putchar_fd  | Outputs a character to a file descriptor.         |
| ft_putstr_fd   | Outputs a string to a file descriptor.            |
| ft_putendl_fd  | Outputs a string followed by a newline.           |
| ft_putnbr_fd   | Outputs an integer to a file descriptor.          |

## Usage

- Clone the repository.
- Compile the library using the provided Makefile. (libft.a)
- Include `libft.h` in your C projects to access the functions.
