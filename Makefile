CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
SRC = src/main.c src/input.c src/handler.c src/execute.c src/parsing.c
OBJ = $(SRC:.c=.o)
TARGET = simpleshell

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)