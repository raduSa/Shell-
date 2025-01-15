%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // getcwd
#include "Command.h"

extern int yylex();
extern void yyerror(const char *s);
extern char *input_buffer;

Command *_currentCommand = NULL;
SimpleCommand *_currentSimpleCommand = NULL;
History *_history = NULL;
%}

%union {
    char *string_val;
}

%token <string_val> WORD GREAT LESS GREATGREAT GREATAMPERSAND GREATGREATAMPERSAND PIPE AMPERSAND NEWLINE

%%

command_list:
    | command_list command_line
    ;

command_line:
    pipe_list io_modifier_list background_optional NEWLINE {
        printf("[DEBUG] Executing command.\n");
        executeCommand(_currentCommand);
        clearCommand(_currentCommand);
        printf("[DEBUG] Command cleared.\n");

        // Reinitialize command structure
        _currentCommand = createCommand();  // Reset the command
        if (_history == NULL) {
            _history = createHistory();
        }
        addToHistory(_history, input_buffer);
        
        // Reset input buffer
        input_buffer = NULL;
        
        YYACCEPT;  // Force exit from `yyparse()`
    }
    | NEWLINE {
        printf("[DEBUG] Empty line. Returning to prompt.\n");
        YYACCEPT;  // Return to prompt on empty line
    }
    | error NEWLINE {
        yyerrok;
        printf("[ERROR] Invalid command.\n");
        YYACCEPT;  // Return to prompt after handling error
    }
    ;

pipe_list:
    cmd_and_args
    | pipe_list PIPE cmd_and_args {
        printf("[DEBUG] Pipe detected.\n");
    }
    ;

cmd_and_args:
    WORD arg_list {
    	if (_currentSimpleCommand == NULL) {
	    printf("[DEBUG] Creating simple command.\n");
            _currentSimpleCommand = createSimpleCommand();
        }
        insertArgument(_currentSimpleCommand, $1);
        printf("[DEBUG] Inserted argument: %s\n", $1);

        if (_currentCommand == NULL) {
            _currentCommand = createCommand();
        }
    
        insertSimpleCommand(_currentCommand, _currentSimpleCommand);
        _currentSimpleCommand = NULL;  // Reset simple command after insertion
        printf("[DEBUG] Simple command inserted.\n");
    }
    ;

arg_list:
    | arg_list WORD {
        printf("[DEBUG] Adding argument to simple command: %s\n", $2);
        if (_currentSimpleCommand == NULL) {
	    printf("[DEBUG] Creating simple command.\n");
            _currentSimpleCommand = createSimpleCommand();
        }
        insertArgument(_currentSimpleCommand, $2);
    }
    ;

io_modifier_list:
    | io_modifier_list io_modifier
    ;

io_modifier:
    GREAT WORD { setOutputFile(_currentCommand, $2, "w"); }
    | LESS WORD { setInputFile(_currentCommand, $2); }
    | GREATGREAT WORD { setOutputFile(_currentCommand, $2, "a"); }
    | GREATAMPERSAND WORD { setErrorFile(_currentCommand, $2); }
    ;

background_optional:
    | AMPERSAND { if (_currentCommand) _currentCommand->_background = 1; }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "[ERROR] %s\n", s);
}

int main(void) {
    printf("Welcome to the shell! Type your commands.\n");

    _currentCommand = createCommand();  // Initialize the first command

    while (1) {
        printf("shell ");
        // get current working directory
	char cwd[BUFSIZ];
	if (NULL == getcwd(cwd, sizeof(cwd)))
		perror("getcwd FAILED");
	printf("@ %s > ", cwd);
        fflush(stdout);  // Ensure prompt is shown immediately

        if (yyparse() == 0) {
            printf("[DEBUG] Command processed successfully.\n");
        } else {
            printf("[ERROR] Parsing error.\n");
        }

        if (_currentCommand == NULL) {
            _currentCommand = createCommand();  // Reset command if NULL
        }
    }

    freeCommand(_currentCommand);  // Clean up memory when exiting the shell
    freeHistory(_history);
    return 0;
}

