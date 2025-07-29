CC = gcc
CFLAGS = -Wall -g

TARGET = B+tree
OBJS = B+tree.o stack_queue.o

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

B+tree.o: B+tree.c stack_queue.h
	$(CC) $(CFLAGS) -c B+tree.c

stack_queue.o: stack_queue.c stack_queue.h
	$(CC) $(CFLAGS) -c stack_queue.c

clean:
	rm -f *.o $(TARGET)