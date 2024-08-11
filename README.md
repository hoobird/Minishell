# MINISHELL
![Logo](./README_RESOURCES/Minishell_logo.png)

## Description
This project, written in C, is about creating a simple shell that mimics bash. It is capable of performing the following:
- Execute executables from env PATH.
- Execute self-developed built-ins such as echo, pwd, cd, exit, export, unset.
- Piping.
- Redirections such as output (>), append (>>), input (<), here-doc (<<).
- Signals such as SIGINT, SIGQUIT, EOF (ctrl-C, ctrl-\, ctrl-D)

## Quick Start

### Prerequisites
- Ensure you have `cc` installed on your system.
- You also need
  - `libreadline-dev`
- Clone the repository using Git.

### Clone and Build

```bash
git clone https://github.com/hoobird/minishell.git
cd minishell
make
```

### Running Minishell

```bash
./minishell
```

You should see the minishell prompt appear, indicating that the shell is ready to accept commands.

## Example of Commands and Outputs

### Basic Commands

- **Executing Built-ins:**

```bash
minishell> echo Hello, World!
Hello, World!
minishell> pwd
/home/user/minishell
```

- **Navigating Directories:**

```bash
minishell> cd ..
minishell> pwd
/home/user
```

### Redirections

- **Output Redirection:**

```bash
minishell> echo "This is a test" > output.txt
minishell> cat output.txt
This is a test
```

- **Input Redirection:**

```bash
minishell> cat < input.txt
This is a test
```

- **Appending to Files:**

```bash
minishell> echo "Another line" >> output.txt
minishell> cat output.txt
This is a test
Another line
```

- **Here Document (<<):**

```bash
minishell> cat << EOF
> This is a line
> EOF
This is a line
```

### Piping

```bash
minishell> echo "Hello, World!" | grep "World"
Hello, World!
```

## Documentation
Google Docs Link [![GDocs logo](https://www.gstatic.com/images/branding/product/1x/docs_2020q4_48dp.png)](https://docs.google.com/document/d/110aoK4tEYZb7dgFtXNXJpPI-a2cLozeHmbdc43mXKzU/edit?usp=sharing)

## Team Members:

[Hubert Lim](https://github.com/hoobird)     [(hulim)](https://profile.intra.42.fr/users/hulim)

[Junjie Su](https://github.com/paperandpencil)     [(jsu)](https://profile.intra.42.fr/users/jsu)
