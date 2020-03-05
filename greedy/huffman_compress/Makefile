TARGET = huffman_compress_encode
CC = gcc -g
CFLAGS = -I.
$(TARGET): huffman_compress_encode.c huffman_compress_encode_test.c
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -rf $(TARGET) *.o