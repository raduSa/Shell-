#include "Command.h"
#include <stdio.h>
#include <string.h>

// Create a new SimpleCommand
SimpleCommand *createSimpleCommand() {
    /*SimpleCommand *simpleCommand = (SimpleCommand *)malloc(sizeof(SimpleCommand));
    simpleCommand->_numberOfAvailableArguments = 10;
    simpleCommand->_numberOfArguments = 0;
    simpleCommand->_arguments = (char **)malloc(simpleCommand->_numberOfAvailableArguments * sizeof(char *));
    return simpleCommand;*/
    printf("Simple Command created\n");
}

// Insert an argument into a SimpleCommand
void insertArgument(SimpleCommand *simpleCommand, char *argument) {
    /*if (simpleCommand->_numberOfArguments == simpleCommand->_numberOfAvailableArguments) {
        simpleCommand->_numberOfAvailableArguments *= 2;
        simpleCommand->_arguments = (char **)realloc(simpleCommand->_arguments, simpleCommand->_numberOfAvailableArguments * sizeof(char *));
    }
    simpleCommand->_arguments[simpleCommand->_numberOfArguments] = strdup(argument);
    simpleCommand->_numberOfArguments++;*/
    printf("Arg inserted.\n");
}

// Free a SimpleCommand
void freeSimpleCommand(SimpleCommand *simpleCommand) {
    /*for (int i = 0; i < simpleCommand->_numberOfArguments; i++) {
        free(simpleCommand->_arguments[i]);
    }
    free(simpleCommand->_arguments);
    free(simpleCommand);*/
    printf("Freed Command\n");
}

// Create a new Command
Command *createCommand() {
    /*Command *command = (Command *)malloc(sizeof(Command));
    command->_numberOfAvailableSimpleCommands = 10;
    command->_numberOfSimpleCommands = 0;
    command->_simpleCommands = (SimpleCommand **)malloc(command->_numberOfAvailableSimpleCommands * sizeof(SimpleCommand *));
    command->_outFile = NULL;
    command->_inputFile = NULL;
    command->_errFile = NULL;
    command->_background = 0;
    return command;*/
    printf("Command created\n");
}

// Insert a SimpleCommand into a Command
void insertSimpleCommand(Command *command, SimpleCommand *simpleCommand) {
    /*if (command->_numberOfSimpleCommands == command->_numberOfAvailableSimpleCommands) {
        command->_numberOfAvailableSimpleCommands *= 2;
        command->_simpleCommands = (SimpleCommand **)realloc(command->_simpleCommands, command->_numberOfAvailableSimpleCommands * sizeof(SimpleCommand *));
    }
    command->_simpleCommands[command->_numberOfSimpleCommands] = simpleCommand;
    command->_numberOfSimpleCommands++;*/
    printf("Simple Command inserted.\n");
}

// Set output file for the Command
void setOutputFile(Command *command, const char *filename) {
    /*if (command->_outFile) free(command->_outFile);
    command->_outFile = strdup(filename);*/
    printf("Set output file.\n");
}

// Set input file for the Command
void setInputFile(Command *command, const char *filename) {
    /*if (command->_inputFile) free(command->_inputFile);
    command->_inputFile = strdup(filename);*/
}

// Set error file for the Command
void setErrorFile(Command *command, const char *filename) {
    /*if (command->_errFile) free(command->_errFile);
    command->_errFile = strdup(filename);*/
}

// Execute the Command (dummy implementation)
void executeCommand(Command *command) {
    printf("Executing command\n");
    // Implementation for actual execution goes here
}

// Clear the Command
void clearCommand(Command *command) {
    /*for (int i = 0; i < command->_numberOfSimpleCommands; i++) {
        freeSimpleCommand(command->_simpleCommands[i]);
    }
    free(command->_simpleCommands);
    if (command->_outFile) free(command->_outFile);
    if (command->_inputFile) free(command->_inputFile);
    if (command->_errFile) free(command->_errFile);
    command->_numberOfSimpleCommands = 0;
    command->_outFile = NULL;
    command->_inputFile = NULL;
    command->_errFile = NULL;
    command->_background = 0;*/
}

// Free the Command
void freeCommand(Command *command) {
    clearCommand(command);
    free(command);
}

