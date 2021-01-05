# Assignment 3: Unix Shell


## Overview
In this assignment, you will implement a command line interpreter (CLI) or, as it is more commonly known, a shell. The shell should operate in this 
basic way: when you type in a command (in response to its prompt), the shell creates a child process that executes the command you entered and then 
prompts for more user input when it has finished. The shells you implement will be similar to, but simpler than, the one you run every day in Unix. 
If you don't know what shell you are running, it's probably `bash`. 

*Built-in Commands*

- Whenever your shell accepts a command, it should check whether the command is a built-in command or not. If it is, it should not be executed 
like other programs. Instead, your shell will invoke your implementation of the built-in command. For example, to implement the `exit` built-in 
command, you simply call `exit(0)`; in your grsh source code, which then will exit the shell.
- In this project, you should implement `exit`, `cd`, and `path` as built-in commands.
- **exit**: When the user types exit, your shell should simply call the exit() system call with 0 as a parameter. It is an error to pass any arguments to exit.
- **cd**: cd always take one argument (0 or >1 args should be signaled as an error). To change directories, use the 
`chdir()` system call with the argument supplied by the user; if chdir fails, that is also an error.
- **path**: The path command takes 0 or more arguments, with each argument separated by a space from the others. 
A typical usage would be like this: `grsh> path /bin /usr/bin`, which would add `/bin` and `/usr/bin` to the search path of the shell. 
If the user sets path to be empty, then the shell should not be able to run any programs (except built-in commands). The path command 
always overwrites the old path with the newly specified path.







### Programming References:
- https://www.gnu.org/software/gnu-c-manual/gnu-c-manual.html
- https://users.ece.utexas.edu/~adnan/c-refcard.pdf
- https://www.cprogramming.com/tutorial/c-tutorial.html?inl=nv
- https://www.tutorialspoint.com/cprogramming/
