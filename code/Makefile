# Makefile pour OceanDepth (C99, compilation modulaire)
# Généré automatiquement

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
LDFLAGS = 

SRCS = src/main.c src/carte.c src/carte_utils.c src/carte_actions.c src/combat.c src/creatures.c src/joueur.c src/inventaire.c src/quetes.c src/sauvegarde.c
OBJS = $(SRCS:.c=.o)

TARGET = oceandepth_modular.exe

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

src/%.o: src/%.c
	$(CC) $(CFLAGS) -Iinclude -c $< -o $@

clean:
	del /Q src\*.o *.exe 2>NUL || rm -f src/*.o *.exe

.PHONY: all clean
