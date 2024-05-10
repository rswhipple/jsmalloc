CC=gcc
CFLAGS=-Wall -Wextra -Werror #-fsanitize=address -g3
BIN=my_malloc
SRC=src/main.c src/hash_table.c src/my_calloc.c src/my_free.c src/my_malloc.c src/my_realloc.c src/block.c src/heap.c src/utils.c
INC=inc

all:
	$(CC) $(CFLAGS) -I$(INC) $(SRC) -o $(BIN)
clean:
	rm -f $(OBJ)
fclean: clean
	rm -f $(BIN)
re: fclean all