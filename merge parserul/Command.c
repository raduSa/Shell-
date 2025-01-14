#include "Command.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>  // For execvp, fork
#include <sys/wait.h>  // For wait
// Create a new Command
Command *createCommand() {
    Command *cmd = malloc(sizeof(Command));
    if (!cmd) {
        fprintf(stderr, "Failed to allocate memory for Command\n");
        return NULL;
    }
    cmd->_numberOfAvailableSimpleCommands = 10;
    cmd->_numberOfSimpleCommands = 0;
    cmd->_simpleCommands = calloc(cmd->_numberOfAvailableSimpleCommands, sizeof(SimpleCommand*));
    cmd->_outFile = NULL;
    cmd->_inputFile = NULL;
    cmd->_errFile = NULL;
    cmd->_background = 0;
    return cmd;
}

// Create a new SimpleCommand
SimpleCommand *createSimpleCommand() {
    SimpleCommand *simpleCommand = malloc(sizeof(SimpleCommand));
    if (!simpleCommand) {
        fprintf(stderr, "Memory allocation failed for SimpleCommand\n");
        return NULL;
    }
    simpleCommand->_numberOfAvailableArguments = 5;
    simpleCommand->_numberOfArguments = 0;
    simpleCommand->_arguments = calloc(simpleCommand->_numberOfAvailableArguments, sizeof(char*));
    return simpleCommand;
}

// Insert an argument into a SimpleCommand
void insertArgument(SimpleCommand *simpleCommand, char *argument) {
    if (!simpleCommand || !argument) {
        fprintf(stderr, "Null pointer encountered in insertArgument\n");
        return;
    }
    if (simpleCommand->_numberOfArguments >= simpleCommand->_numberOfAvailableArguments) {
        simpleCommand->_numberOfAvailableArguments *= 2;
        simpleCommand->_arguments = realloc(simpleCommand->_arguments, simpleCommand->_numberOfAvailableArguments * sizeof(char*));
    }
    simpleCommand->_arguments[simpleCommand->_numberOfArguments++] = strdup(argument);
}

// Insert a SimpleCommand into a Command
void insertSimpleCommand(Command *command, SimpleCommand *simpleCommand) {
    if (!command || !simpleCommand) return;
    if (command->_numberOfSimpleCommands >= command->_numberOfAvailableSimpleCommands) {
        command->_numberOfAvailableSimpleCommands *= 2;
        command->_simpleCommands = realloc(command->_simpleCommands, command->_numberOfAvailableSimpleCommands * sizeof(SimpleCommand*));
    }
    command->_simpleCommands[command->_numberOfSimpleCommands++] = simpleCommand;
}

// Set output file for Command
void setOutputFile(Command *command, const char *filename, const char *mode) {
    free(command->_outFile);
    command->_outFile = strdup(filename);
}

// Set input file for Command
void setInputFile(Command *command, const char *filename) {
    free(command->_inputFile);
    command->_inputFile = strdup(filename);
}

// Set error file for Command
void setErrorFile(Command *command, const char *filename) {
    free(command->_errFile);
    command->_errFile = strdup(filename);
}

// Execute the Command
void executeCommand(Command *command) {
    if (command->_numberOfSimpleCommands == 0) {
        fprintf(stderr, "[ERROR] No command to execute.\n");
        return;
    }

    // Save original stdin and stdout
    int tmpin = dup(0);
    int tmpout = dup(1);

    // Set up input redirection
    int fdin;
    if (command->_inputFile) {
        fdin = open(command->_inputFile, O_RDONLY);
        if (fdin < 0) {
            perror("Error opening input file");
            return;
        }
    } else {
        fdin = dup(tmpin);
    }

    int ret;
    int fdout;

    // Loop through each simple command
    for (int i = 0; i < command->_numberOfSimpleCommands; i++) {
        // Redirect input
        dup2(fdin, 0);
        close(fdin);

        // Setup output
        if (i == command->_numberOfSimpleCommands - 1) {
            // Last command
            if (command->_outFile) {
                fdout = open(command->_outFile, O_WRONLY | O_CREAT | O_TRUNC, 0666);
                if (fdout < 0) {
                    perror("Error opening output file");
                    return;
                }
            } else {
                fdout = dup(tmpout);
            }
        } else {
            // Not the last command, create a pipe
            int fdpipe[2];
            if (pipe(fdpipe) == -1) {
                perror("Pipe creation failed");
                return;
            }
            fdout = fdpipe[1];
            fdin = fdpipe[0];
        }

        // Redirect output
        dup2(fdout, 1);
        close(fdout);

        // Get the current simple command
        SimpleCommand *simpleCommand = command->_simpleCommands[i];

        // Prepare arguments for execvp
        char **args = malloc((simpleCommand->_numberOfArguments + 1) * sizeof(char *));
        args[0] = simpleCommand->_arguments[simpleCommand->_numberOfArguments - 1];
        for (int j = 0; j < simpleCommand->_numberOfArguments - 1; j++) {
            args[j + 1] = simpleCommand->_arguments[j];
        }
        args[simpleCommand->_numberOfArguments] = NULL; // Null-terminate the arguments array
        
        // Handle built-in commands
	if (strcmp(args[0], "help") == 0) {
		printf("Shell Help:\n");
		printf("Available commands:\n");
		printf("- help: Show available commands\n");
		printf("- exit: Exit the shell\n");
		printf("- Any other Linux command (e.g., pwd, ls)\n");
		free(args);
		return;
	}

	if (strcmp(args[0], "exit") == 0) {
		printf("Exiting shell.\n");
		free(args);
		exit(0);
	}

        // Create the child process
        ret = fork();
        if (ret == 0) {
            // Child process: Execute the current command
            
            char buffer[1024];
	    read(fdin, buffer, sizeof(buffer));
	    printf("Received from pipe: %s\n", buffer);
	    
            execvp(args[0], args);
            perror("execvp failed");
            exit(1);
        } else if (ret < 0) {
            perror("Fork failed");
            free(args);
            return;
        }

        // Free arguments array
        free(args);
    }

    // Restore original stdin and stdout
    dup2(tmpin, 0);
    dup2(tmpout, 1);
    close(tmpin);
    close(tmpout);

    // Wait for the last command if not running in background
    if (!command->_background) {
        waitpid(ret, NULL, 0);
    }
}

// Clear the Command
void clearCommand(Command *command) {
    for (int i = 0; i < command->_numberOfSimpleCommands; i++) {
        SimpleCommand *simpleCommand = command->_simpleCommands[i];
        for (int j = 0; j < simpleCommand->_numberOfArguments; j++) {
            free(simpleCommand->_arguments[j]);
        }
        free(simpleCommand->_arguments);
        free(simpleCommand);
    }
    free(command->_simpleCommands);
    command->_simpleCommands = NULL;
    free(command->_outFile);
    free(command->_inputFile);
    free(command->_errFile);
    command->_outFile = NULL;
    command->_inputFile = NULL;
    command->_errFile = NULL;
    command->_numberOfSimpleCommands = 0;
    command->_background = 0;
}

// Free the Command
void freeCommand(Command *command) {
    if (command) {
        clearCommand(command);
        free(command);
    }
}

