# Nume executabil
TARGET = shell

# Fișiere sursă
YACC_FILE = shell.y
LEX_FILE = shell.l
C_FILES = Command.c

# Fișiere generate
YACC_C = y.tab.c
YACC_H = y.tab.h
LEX_C = lex.yy.c

# Compilator și opțiuni
CC = gcc
YACC = yacc
LEX = flex
CFLAGS = -Wall -Wextra -g
LIBS = -lfl

# Regulă implicită
all: $(TARGET)

# Generare executabil
$(TARGET): $(YACC_C) $(LEX_C) $(C_FILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(YACC_C) $(LEX_C) $(C_FILES) $(LIBS)

# Generare fișier .c și .h din .y (Yacc)
$(YACC_C): $(YACC_FILE)
	$(YACC) -d $(YACC_FILE)

# Generare fișier .c din .l (Lex)
$(LEX_C): $(LEX_FILE)
	$(LEX) $(LEX_FILE)

# Curățare fișiere temporare
clean:
	rm -f $(TARGET) $(YACC_C) $(YACC_H) $(LEX_C)

# Curățare completă
distclean: clean
	rm -f *.o

