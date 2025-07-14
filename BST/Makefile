CC = gcc
CFLAGS = -Wall -g

TARGET = binary_search_tree
OBJS = binary_search_tree.o stack_queue.o

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

binary_search_tree.o: binary_search_tree.c stack_queue.h
	$(CC) $(CFLAGS) -c binary_search_tree.c

stack_queue.o: stack_queue.c stack_queue.h
	$(CC) $(CFLAGS) -c stack_queue.c

clean:
	rm -f *.o $(TARGET)