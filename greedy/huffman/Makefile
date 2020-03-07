TARGET = huffman
CC = g++ -g -std=c++11
CFLAGS = -I.
$(TARGET): huffman.cpp
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -rf $(TARGET) *.o