TARGET = mergeSamilFile
CC = gcc
CFLAGS = -I.
$(TARGET): fileManage.c priorityQueue.c mergeSamilFile.c
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -rf $(TARGET) *.o