## Séries - TP Compression d'un signal audio
## Makefile
## Auteur : Romain DUJOL

## Configuration

# Compilateur C
CC = gcc -c
CFLAGS = -Wall -pedantic

# Editeur de liens C
LD = gcc
LDFLAGS = -lsndfile -lgsl -lgslcblas -lm

HDR = $(wildcard *.h) $(wildcard */*.h)
SRC = $(wildcard *.c) $(wildcard */*.c)
OBJ = $(SRC:.c=.o)
BIN = ./compress

# Les noms des règles suivantes ne font pas référence à des noms de vrais fichiers.
.PHONY : clean reset

# Ces fichiers sont considérés comme non-intermédaires et ne seront pas supprimés automatiquement.
.PRECIOUS : $(wildcard *.wav)

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
	rm $(BIN)

