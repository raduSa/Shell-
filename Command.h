#ifndef COMMAND_H
#define COMMAND_H

#include <stdlib.h>

// Define the SimpleCommand structure
typedef struct SimpleCommand {
    int _numberOfAvailableArguments; // Preallocated space for arguments
    int _numberOfArguments;          // Number of arguments
    char **_arguments;               // Array of arguments
} SimpleCommand;

// Define the Command structure
typedef struct Command {
    int _numberOfAvailableSimpleCommands; // Preallocated space for simple commands
    int _numberOfSimpleCommands;          // Number of simple commands
    SimpleCommand **_simpleCommands;      // Array of simple commands
    char *_outFile;                       // Output file
    char *_inputFile;                     // Input file
    char *_errFile;                       // Error file
    int _background;                      // Background execution flag
} Command;

// Function prototypes for SimpleCommand operations
SimpleCommand *createSimpleCommand();
void insertArgument(SimpleCommand *simpleCommand, char *argument);
void freeSimpleCommand(SimpleCommand *simpleCommand);

// Function prototypes for Command operations
Command *createCommand();
void insertSimpleCommand(Command *command, SimpleCommand *simpleCommand);
void setOutputFile(Command *command, const char *filename);
void setInputFile(Command *command, const char *filename);
void setErrorFile(Command *command, const char *filename);
void executeCommand(Command *command);
void clearCommand(Command *command);
void freeCommand(Command *command);

#endif

