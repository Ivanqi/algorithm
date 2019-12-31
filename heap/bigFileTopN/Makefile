TARGET1 = fileManage
TARGET2 = bigFileTopN
CC = gcc
CFLAGS1 = -I.
DEBUG = -g

$(TARGET1): fileManage.c
	$(CC) $(DEBUG) -o $@ $^ $(CFLAGS1)

$(TARGET2): hashMap.c priorityQueue.c bigFileTopN.c 
	$(CC) $(DEBUG) -o $@ $^ $(CFLAGS1)

clean:
	rm -rf $(TARGET1) $(TARGET2) *.o