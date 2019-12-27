TARGET = midnum
CC = gcc
CFLAGS = -I.
$(TARGET): priorityQueue.c midnum.c
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -rf $(TARGET) *.o