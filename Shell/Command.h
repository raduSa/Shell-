#ifndef COMMAND_H
#define COMMAND_H

#include <stdlib.h>

typedef struct SimpleCommand {
    int _numberOfAvailableArguments;  // Preallocated space for arguments
    int _numberOfArguments;           // Number of arguments
    char **_arguments;                // Array of arguments
} SimpleCommand;

typedef struct Command {
    int _numberOfAvailableSimpleCommands;  // Preallocated space for simple commands
    int _numberOfSimpleCommands;           // Number of simple commands
    SimpleCommand **_simpleCommands;       // Array of simple commands
    char *_outFile;                        // Output file
    char *_inputFile;                      // Input file
    char *_errFile;                        // Error file
    int _background;                       // Background execution flag
} Command;

typedef struct History {
    char **_commands;     // Array of command strings
    int _count;           // Number of commands in history
    int _size;            // Allocated size for history
} History;

// Function prototypes for SimpleCommand operations
SimpleCommand *createSimpleCommand();
void insertArgument(SimpleCommand *simpleCommand, char *argument);
void freeSimpleCommand(SimpleCommand *simpleCommand);

// Function prototypes for Command operations
Command *createCommand();
void insertSimpleCommand(Command *command, SimpleCommand *simpleCommand);
void setOutputFile(Command *command, const char *filename, const char *mode);
void setInputFile(Command *command, const char *filename);
void setErrorFile(Command *command, const char *filename);
void executeCommand(Command *command);
void clearCommand(Command *command);
void freeCommand(Command *command);

// Function prototypes for History operations
History *createHistory();
void addToHistory(History *history, const char *cmd);
void showHistoryN(History *history, int n);
void showHistory(History *history);
void clearHistory(History *history);
void freeHistory(History *history);

#endif

