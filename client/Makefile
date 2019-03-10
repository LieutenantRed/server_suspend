TARGET=client
CC=gcc
CFLAGS=-Wall -g 
LIBS=-pthread

OBJ = $(patsubst src/%.c, %.o, $(wildcard src/*.c) )
SRC = $(wildcard src/*.c)
HEADERS = $(wildcard src/*.h)

.PHONY: clean
.DEFAULT: $(TARGET)

$(TARGET): $(OBJ) $(HEADERS)
	$(CC) $(OBJ) -o $(TARGET) $(LIBS)
	rm *.o

%.o: src/%.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

uninstall: clean

clean:
	@-(rm $(TARGET)) || true