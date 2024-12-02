#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

typedef bool (*InternalCommandHandler)(char **parameters);

typedef struct {
    char *command_name;
    InternalCommandHandler handler;
} CommandMapping;

bool handle_exit(char **parameters) {
    exit(0);
}

bool handle_cd(char **parameters) { 
    if (parameters[1] == NULL) {
        fprintf(stderr, "cd: expected argument to \"cd\"\n");
        return true;
    }

    char path[1024] = {0};
    int i = 1;
    while (parameters[i] != NULL) {
        if (i > 1) strcat(path, " ");
        strcat(path, parameters[i]);
        i++;
    }

    if (chdir(path) != 0) {
        perror("cd failed");
        return true;
    }
    return true;
}

bool handle_help(char **parameters) {
    printf("Help: Available commands are 'cd', 'exit', and external binaries in /bin/\n");
    return true;
}

CommandMapping commands_map[] = {
    {"exit", handle_exit},
    {"cd", handle_cd},
    {"help", handle_help},
    {NULL, NULL}  //final
};

bool execute_internal_command(char *command, char *parameters[]) {//AB
    for (int i = 0; commands_map[i].command_name != NULL; i++) {
        if (strcmp(command, commands_map[i].command_name) == 0) {
            return commands_map[i].handler(parameters);
        }
    }
    return false;  // not an intern command
}



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


void read_command(char cmd[], char *par[]) {//SR
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
		
		if (execute_internal_command(command, parameters)) {
                    continue;  // intern command.
                }
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
