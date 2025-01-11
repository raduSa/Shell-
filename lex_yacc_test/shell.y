%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Forward declarations */
void yyerror(const char *s);
int yylex();
%}

/* Define token types */
%union {
    char *string_val; /* For WORD tokens (filenames, arguments, commands) */
}

/* Token definitions */
%token <string_val> WORD
%token GREAT LESS GREATGREAT GREATAMPERSAND PIPE AMPERSAND NEWLINE
%type <string_val> redirection background

%%

/* Grammar rules */
command_list:
    command NEWLINE                { printf("Command executed.\n"); }
  | command_list command NEWLINE   { printf("Multiple commands executed.\n"); }
  ;

command:
    simple_command
  | simple_command PIPE command    { printf("Pipeline created.\n"); }
  | simple_command background      { printf("Background command executed.\n"); }
  ;

simple_command:
    WORD                           { printf("Command: %s\n", $1); free($1); }
  | simple_command WORD            { printf("Argument: %s\n", $2); free($2); }
  | simple_command redirection     { printf("Redirection processed.\n"); }
  ;

redirection:
    GREAT WORD                     { printf("Redirect output to: %s\n", $2); free($2); }
  | LESS WORD                      { printf("Redirect input from: %s\n", $2); free($2); }
  | GREATGREAT WORD                { printf("Append output to: %s\n", $2); free($2); }
  | GREATAMPERSAND WORD            { printf("Redirect stdout and stderr to: %s\n", $2); free($2); }
  ;

background:
    AMPERSAND                      { printf("Command will run in background.\n"); }
  ;

%%

/* Error handling */
void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}

int main() {
    printf("Welcome to the shell! Type your commands.\n");
    while (1) {
        printf("shell> ");
        yyparse();
    }
    return 0;
}

