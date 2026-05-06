CC = gcc
CFLAGS = -Wall -Wextra -g -Iinclude

SRC = main.c \
      src/account.c \
      src/codice.c \
      src/segnalazione.c \
      src/utils.c

OBJ = $(SRC:.c=.o)

TARGET = a

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)