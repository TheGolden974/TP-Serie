## Séries - TP Calcul de pi
## Makefile
## Auteur : Romain DUJOL

## Configuration

# Compilateur C
CC = gcc -c
CFLAGS = -Wall -pedantic

# Editeur de liens C
LD = gcc
LDFLAGS = -lm

HDR = $(wildcard *.h) # Tous les fichiers .h
SRC = $(wildcard *.c) # Tous les fichiers .c
OBJ = $(SRC:.c=.o)    # Tous les fichiers .o associés aux fichiers .c
BIN = ./pi

# Les noms des règles suivantes ne font pas référence à des noms de vrais fichiers.
.PHONY : clean reset

## Dependances

default : $(BIN)

# Edition des liens
$(BIN) : $(OBJ)
	$(LD) $^ $(LDFLAGS) -o $@

# Compilation C : règle par défaut
%.o : %.c
	$(CC) $(CFLAGS) $< -o $@

# Règles de nettoyage
clean :
	rm -f $(OBJ)
reset : clean
	rm -f $(BIN)

