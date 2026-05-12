CC = gcc
CFLAGS = -Wall -Wextra -g -Iinclude

SRC = main.c \
      src/account.c \
      src/codice.c \
      src/segnalazione.c \
      src/utils.c

OBJ = $(SRC:%.c=build/%.o)

TARGET = progettoPSD

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

build/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf build $(TARGET)