CC = gcc
CFLAGS = -Wall -g

EXEC = translate

SOURCES = address.c main.c stat.c

HEADERS = lab4.h

OBJECTS = $(SOURCES:.c=.o)

all: $(EXEC)

$(EXEC): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJECTS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(EXEC)

.PHONY: all clean
