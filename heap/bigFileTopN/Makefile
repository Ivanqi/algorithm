TARGET1 = fileManage
TARGET2 = hashMap
CC = gcc
CFLAGS1 = -I.
DEBUG = -g

$(TARGET1): fileManage.c
	$(CC) $(DEBUG) -o $@ $^ $(CFLAGS1)

$(TARGET2): hashMap.c
	$(CC) $(DEBUG) -o $@ $^ $(CFLAGS1)

clean:
	rm -rf $(TARGET1) $(TARGET2) *.o