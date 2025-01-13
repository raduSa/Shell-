#ifndef SHELL_H
#define SHELL_H

typedef struct SimpleCommand {
    int numArgs;        // Numărul de argumente
    char **args;        // Lista de argumente
} SimpleCommand;

typedef struct Command {
    SimpleCommand **simpleCommands;  // Lista de comenzi simple
    int numSimpleCommands;           // Numărul de comenzi simple
    int allocatedSimpleCommands;     // Comenzi alocate
    char *outFile;                   // Fișier de ieșire pentru redirecționare
    char *inputFile;                 // Fișier de intrare pentru redirecționare
    char *errorFile;                 // Fișier de eroare pentru redirecționare
    int background;                  // Indicator pentru execuție în background (1/0)
} Command;

extern Command *currentCommand;

void initCommand();
void insertSimpleCommand(SimpleCommand *cmd);
void addArgument(SimpleCommand *cmd, char *arg);
void executeCommand();
void clearCommand();

#endif // SHELL_H

