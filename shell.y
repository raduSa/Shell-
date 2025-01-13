%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Command.h" // Include the header defining the data structures

/* Declare the external `Command` and `SimpleCommand` objects */
Command *_currentCommand = NULL;
SimpleCommand *_currentSimpleCommand = NULL;

void yyerror(const char *s);
int yylex();
%}

%union {
    char *string_val;
}

/* Token definitions */
%token <string_val> WORD
%token GREAT LESS GREATGREAT GREATAMPERSAND GREATGREATAMPERSAND PIPE AMPERSAND NEWLINE
%type <string_val> arg_list cmd_and_args pipe_list io_modifier io_modifier_list background_optional command_line command_list

%%

goal:
    command_list
;

arg_list:
    arg_list WORD                  {
                                      // Insert argument into the current simple command
                                      insertArgument(_currentSimpleCommand, $2);
                                  }
  | /* empty */                    { /* No arguments */ }
;

cmd_and_args:
    WORD arg_list                  {
                                      // Initialize a new simple command and set its initial argument
                                      _currentSimpleCommand = createSimpleCommand();
                                      insertArgument(_currentSimpleCommand, $1);

                                      // Insert the simple command into the current command
                                      insertSimpleCommand(_currentCommand, _currentSimpleCommand);
                                  }
;

pipe_list:
    pipe_list PIPE cmd_and_args    {
                                      // Handle pipelines by sequentially adding commands
                                      printf("Pipeline created.\n");
                                  }
  | cmd_and_args
;

io_modifier:
    GREATGREAT WORD                {
                                      // Set the append output file for the current command
                                      setOutputFile(_currentCommand, $2);
                                  }
  | GREAT WORD                     {
                                      // Set the output file for the current command
                                      setOutputFile(_currentCommand, $2);
                                  }
  | GREATGREATAMPERSAND WORD       {
                                      // Set the append output file for both stdout and stderr
                                      setOutputFile(_currentCommand, $2);
                                      setErrorFile(_currentCommand, $2);
                                  }
  | GREATAMPERSAND WORD            {
                                      // Set the output file for both stdout and stderr
                                      setOutputFile(_currentCommand, $2);
                                      setErrorFile(_currentCommand, $2);
                                  }
  | LESS WORD                      {
                                      // Set the input file for the current command
                                      setInputFile(_currentCommand, $2);
                                  }
;

io_modifier_list:
    io_modifier_list io_modifier   { /* Handle multiple redirections */ }
  | /* empty */                    { /* No redirections */ }
;

background_optional:
    AMPERSAND                      {
                                      // Set the background flag for the current command
                                      _currentCommand->_background = 1;
                                  }
  | /* empty */                    {
                                      // Default: no background execution
                                      _currentCommand->_background = 0;
                                  }
;

command_line:
    pipe_list io_modifier_list background_optional NEWLINE {
                                      // Execute the command after parsing the entire line
                                      executeCommand(_currentCommand);

                                      // Clear the current command for the next input
                                      clearCommand(_currentCommand);
                                  }
  | NEWLINE                        { /* Empty command line */ }
  | error NEWLINE                  {
                                      yyerrok;
                                      printf("Syntax error, skipping to next command.\n");
                                  }
;

command_list:
    command_list command_line      { /* Multiple commands */ }
  | /* empty */                    { /* No commands */ }
;

%%

/* Error handling */
void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}

int main() {
    printf("Welcome to the shell! Type your commands.\n");

    // Initialize the current command
    //_currentCommand = createCommand();

    while (1) {
        printf("shell> ");
        yyparse();
    }

    // Free the current command when the shell exits
    //freeCommand(_currentCommand);

    return 0;
}

