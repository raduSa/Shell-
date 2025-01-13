/*
 * shell.l: simple lexical analyzer for the shell.
 */

%{
#include <string.h>
#include "y.tab.h"  /* Ensure this matches the yacc/bison header file */
%}

/* Token definitions */
%%
\n              { return NEWLINE; }
[ \t]           { /* Ignore spaces and tabs */ }
">>"            { return GREATGREAT; }
">&"            { return GREATAMPERSAND; }
">"             { return GREAT; }
"<"             { return LESS; }
"|"             { return PIPE; }
"&"             { return AMPERSAND; }
[^ \t\n][^ \t\n]* {
                  /* Match words or filenames */
                  yylval.string_val = strdup(yytext); /* strdup allocates memory */
                  return WORD;
                }
%%
