TARGET = huffman_compress
CC = gcc -g
CFLAGS = -I.
$(TARGET): huffman_compress.c huffman_compress_main.c
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -rf $(TARGET) *.o