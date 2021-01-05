//include statements
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>

//define statements
#define THE_BUFSIZE 64
#define TOK_DELIM " \t\r\n\a"
#define RL_BUFSIZE 1024

//initilizations
int execute(char **args);
char **split_line(char *line);

int grsh_cd(char **args);
int grsh_exit();
int grsh_path(char **args);
char path[] = "/bin/";

//builtin functions

char error_message[30] = ("An error has occured\n");
char test[30] = ("test message\n");
char *builtin_str[] = {
	"cd",
	"path",
    "exit"
};

int (*builtin_func[]) (char **) = {
	&grsh_cd,
	&grsh_path,
    &grsh_exit
};

int grsh_num_builtins(void) {
	return sizeof(builtin_str) / sizeof(char *);
}

int grsh_cd(char **args)
{
	if (args[1] == NULL) {
		write(STDERR_FILENO, error_message, strlen(error_message));
	} else {
		if (chdir(args[1]) != 0) {
			write(STDERR_FILENO, error_message, strlen(error_message));
		}
	}
	return 1;
}

int grsh_path(char **args)
{
		if(args[1] == NULL)
		memset(path, 0, sizeof(path));

	else{
		int j = 2;
		memset(path, 0, sizeof(path));
		strcpy(path, args[1]);

		while(args[j] != NULL){
			strcpy(path, "\n");
			strcat(path, args[j]);
			j++;
		}
	}

	return 1;
}

int grsh_exit()
{
	exit(0);
}


pid_t child_pid;

void kill_child(int sig)
{
	kill(child_pid,SIGKILL);
}

int mysh_launch(char **args)
{
	
	pid_t pid;
	int status;
	char *paths;
	char *mainpath = NULL;
	mainpath = malloc(THE_BUFSIZE * sizeof(char *));
	paths = strtok(path, TOK_DELIM);

	while(paths != NULL){

		strcpy(mainpath, paths);
		strcat(mainpath, "/");
		strcat(mainpath, args[0]);

		if(access(mainpath, X_OK) == 0)
			break;

		else
			free(mainpath);

		paths = strtok(path, TOK_DELIM);
	}

	pid = fork();

	if (pid == 0)
	{
		if(execv(mainpath, args) == -1){
			write(STDERR_FILENO, error_message, strlen(error_message));
			

		}

		return 0;
	}

	else if (pid < 0)
	{
		write(STDERR_FILENO, error_message, strlen(error_message));
		return 0;
	}

	else
	{
		do
		{
			waitpid(pid, &status, WUNTRACED);
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}

	return 1;
}

int execute(char **args)
{
	int i;

	if (args[0] == NULL) 
	{
		// An empty command was entered.
		return 1;
	}

	for (i = 0; i < grsh_num_builtins(); i++) 
	{
		if(strcmp(args[0], "exit") == 0)
			grsh_exit();
		if (strcmp(args[0], builtin_str[i]) == 0) 
		{
			return (*builtin_func[i])(args);
		}
	}

	return mysh_launch(args);
}

char *read_line(void)
{
  char *line = NULL;
  size_t bufsize = 0; // have getline allocate a buffer for us

  if (getline(&line, &bufsize, stdin) == -1){
    if (feof(stdin)) {
      exit(EXIT_SUCCESS);  // We recieved an EOF
    } else  {
      perror("readline");
      exit(EXIT_FAILURE);
    }
  }

  return line;
}

char **split_line(char *line)
{
	int bufsize = THE_BUFSIZE;
	int position = 0;
	char **tokens = malloc(bufsize * sizeof(char*));
	char *token;

	token = strtok(line, TOK_DELIM);

	while (token != NULL) {
		tokens[position] = token;
		position++;

		token = strtok(NULL, TOK_DELIM);
	}

	tokens[position] = NULL;
	return tokens;
}

void interactive_shell_mode(void)
{

	char *line;
	char **args;
	int status;

	do{
		printf("grsh> ");
		line = read_line();
		args = split_line(line);
		status = execute(args);

		free(line);
		free(args);
	} while (status);
	exit(0);
}


void batch_shell(char *filename){

	size_t len =0;
	ssize_t read;
	char **args;
	char *line = NULL;
	FILE *file = fopen(filename, "r");

	if(!file)
	{
		write(STDERR_FILENO, error_message, strlen(error_message));
         	exit(1);
	}

	while((read = getline(&line, &len, file)) != -1){
		args = split_line(line);
		execute(args);
	}
fclose(file);

}

int main(int argc, char **argv)
{
    if(argc < 2){
		interactive_shell_mode();
	}
	else{
		batch_shell(argv[1]);
	}
	return 0;
}