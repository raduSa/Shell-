/*
 * shell.l: simple lexical analyzer for the shell.
 */

%{
#include <string.h>
#include "y.tab.h"  /* Ensure this matches the yacc/bison header file */

char *input_buffer;  // Buffer to store the entire input
int input_len;          // Length of the current input buffer
%}

/* Token definitions */
%%
\n              { 
                    // End of line, nothing to append here
                    return NEWLINE; 
                }
[ \t]           { /* Ignore spaces and tabs */ }

">>"            { 
		    if (input_buffer == NULL) input_len = 0;
                    input_buffer = realloc(input_buffer, input_len + 4);
                    strcpy(input_buffer + input_len, ">> ");
                    input_len += 3;
                    return GREATGREAT;
                }
">&"            { 
		    if (input_buffer == NULL) input_len = 0;
                    input_buffer = realloc(input_buffer, input_len + 4);
                    strcpy(input_buffer + input_len, ">& ");
                    input_len += 3;
                    return GREATAMPERSAND;
                }
">"             { 
		    if (input_buffer == NULL) input_len = 0;
                    input_buffer = realloc(input_buffer, input_len + 3);
                    strcpy(input_buffer + input_len, "> ");
                    input_len += 2;
                    return GREAT;
                }
"<"             { 
  	 	    if (input_buffer == NULL) input_len = 0;
                    input_buffer = realloc(input_buffer, input_len + 3);
                    strcpy(input_buffer + input_len, "< ");
                    input_len += 2;
                    return LESS;
                }
"|"             { 
		    if (input_buffer == NULL) input_len = 0;
                    input_buffer = realloc(input_buffer, input_len + 3);
                    strcpy(input_buffer + input_len, "| ");
                    input_len += 2;
                    return PIPE;
                }
"&"             { 
		    if (input_buffer == NULL) input_len = 0;
                    input_buffer = realloc(input_buffer, input_len + 3);
                    strcpy(input_buffer + input_len, "& ");
                    input_len += 2;
                    return AMPERSAND;
                }

[^ \t\n][^ \t\n]* {
                  /* Match words or filenames */
                  yylval.string_val = strdup(yytext);  /* strdup allocates memory */
                  
                  if (input_buffer == NULL) input_len = 0;
                  int len = strlen(yytext);              
                  input_buffer = realloc(input_buffer, input_len + len + 2);
                  strcpy(input_buffer + input_len, yytext);     
                  input_len += len;
                  strcpy(input_buffer + input_len, " ");
                  input_len++;

                  return WORD;
                }
%%
int yywrap(void) {
    return 1;  // Required by Lex for EOF handling
}

