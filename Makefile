CC = gcc
SRC = cjson.c j_linked_list.c j_read_config.c test.c
CFLAGS = -lm
TARGET = test

all:
	$(CC) $(SRC) $(CFLAGS) -o $(TARGET)
