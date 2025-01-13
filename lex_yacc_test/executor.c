#include "shell.h"
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>

Command *currentCommand = NULL;

void initCommand() {
    currentCommand = malloc(sizeof(Command));
    currentCommand->simpleCommands = malloc(sizeof(SimpleCommand *) * 5);
    currentCommand->numSimpleCommands = 0;
    currentCommand->allocatedSimpleCommands = 5;
    currentCommand->outFile = NULL;
    currentCommand->inputFile = NULL;
    currentCommand->errorFile = NULL;
    currentCommand->background = 0;
}

void insertSimpleCommand(SimpleCommand *cmd) {
    if (currentCommand->numSimpleCommands == currentCommand->allocatedSimpleCommands) {
        currentCommand->allocatedSimpleCommands += 5;
        currentCommand->simpleCommands = realloc(currentCommand->simpleCommands, sizeof(SimpleCommand *) * currentCommand->allocatedSimpleCommands);
    }
    currentCommand->simpleCommands[currentCommand->numSimpleCommands++] = cmd;
}

void addArgument(SimpleCommand *cmd, char *arg) {
    cmd->args = realloc(cmd->args, sizeof(char *) * (cmd->numArgs + 2));
    cmd->args[cmd->numArgs++] = strdup(arg);
    cmd->args[cmd->numArgs] = NULL;
}

void executeCommand() {
    // Logica pentru execuția comenzii
}

void clearCommand() {
    for (int i = 0; i < currentCommand->numSimpleCommands; i++) {
        SimpleCommand *cmd = currentCommand->simpleCommands[i];
        for (int j = 0; j < cmd->numArgs; j++) {
            free(cmd->args[j]);
        }
        free(cmd->args);
        free(cmd);
    }
    free(currentCommand->simpleCommands);
    free(currentCommand->outFile);
    free(currentCommand->inputFile);
    free(currentCommand->errorFile);
    free(currentCommand);
}

