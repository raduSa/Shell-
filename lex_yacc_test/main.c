#include "shell.h"
#include <stdio.h>

extern int yyparse();  // Declarația funcției generate de Bison pentru parsare

int main() {
    printf("Welcome to MyShell! Type commands:\n");
    while (1) {
        printf("myshell> ");
        fflush(stdout);  // Asigură afișarea promptului
        if (yyparse() == 0) {  // Dacă parsarea s-a terminat cu succes
            executeCommand();  // Execută comanda parseată
            clearCommand();    // Curăță structurile de date
        }
    }
    return 0;
}

