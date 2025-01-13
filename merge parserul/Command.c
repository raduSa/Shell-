#include "Command.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    if (command->_numberOfSimpleCommands == 0) return;

    // Get the first simple command and the first argument
    SimpleCommand *simpleCommand = command->_simpleCommands[0];
    char *commandName = simpleCommand->_arguments[0];

    // Handle built-in commands
    if (strcmp(commandName, "help") == 0) {
        printf("Shell Help:\n");
        printf("Available commands:\n");
        printf("- help: Show available commands\n");
        printf("- exit: Exit the shell\n");
        printf("- Any other Linux command (e.g., pwd, ls)\n");
        return;
    }

    if (strcmp(commandName, "exit") == 0) {
        printf("Exiting shell.\n");
        exit(0);
    }

    // Fork a new process
    pid_t pid = fork();
    if (pid == 0) {
        // Child process
        printf("[DEBUG] Executing: %s\n", commandName);
        execvp(commandName, simpleCommand->_arguments);
        perror("Error executing command");  // If execvp fails
        exit(1);
    } else if (pid < 0) {
        perror("Fork failed");
        return;
    }

    // Parent process: wait for the child to complete
    if (!command->_background) {
        int status;
        waitpid(pid, &status, 0);
        printf("[DEBUG] Command finished with status: %d\n", status);
    } else {
        printf("[DEBUG] Command running in background (PID: %d).\n", pid);
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

