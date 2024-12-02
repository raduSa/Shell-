#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

void type_prompt() {
	static int first_time = 1;
	// clear screen
	if (first_time) {
		const char* CLEAR_SCREEN_ANSI = " \033[1;1H\033[2J";
		write(STDOUT_FILENO, CLEAR_SCREEN_ANSI, 12);
		first_time = 0;
	}

	// get current working directory
	char cwd[BUFSIZ];
	if (NULL == getcwd(cwd, sizeof(cwd)))
		perror("getcwd FAILED");
	printf("@ %s > ", cwd);
}

void read_command(char cmd[], char *par[]) {
	char line[1024];
	int count = 0, i = 0, j = 0;
	char *token_array[100], *token;

	//read one line
	while (true) {
		// read next character
		int c = fgetc(stdin);

		line[count++] = (char) c;
		if (c == '\n')
			break;	
	}

	// handle empty input
	if (count == 1) 
		return;

	// split line into tokens
	token = strtok(line, " \n");

	while (token != NULL) {
		token_array[i++] = strdup(token);
		token = strtok(NULL, " \n");
	}

	// first word is the command
	strcpy(cmd, token_array[0]);

	// all others tokens are parameters
	for (int j = 0; j < i; j++)
		par[j] = token_array[j];
	par[i] = NULL;
}

int main() {
	char cmd[100], command[100], *parameters[20];
	char *envp[] = { (char *) "PATH=/bin", 0 };
	while (true) {
		type_prompt();
		read_command(command, parameters);
		
		if (strcmp(command, "exit") == 0)
			break;
		
		if (fork() != 0)
			wait( NULL );
		else {
			strcpy(cmd, "/bin/");
			strcat(cmd, command);
			if (execve(cmd, parameters, envp) == -1) {
				perror("Invalid command");
			}
			break;
		}
	}
	return 0;
}
