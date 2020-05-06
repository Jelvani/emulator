#make file - this is a comment section
 
CC=gcc
TARGET=vm.c #target file name
OUTPUT=vm #target output name
 
all:
	$(CC) -o $(OUTPUT) $(TARGET) -Werror
	$(CC) -o assembler assembler.c -Werror
	$(CC) -S $(TARGET)
clean:
	rm $(OUTPUT)
	rm assembler
