CFLAGS = -Wall -Wconversion -Wextra

# Find all .c files excluding those in n1.ko directory
SRC_FILES := $(shell find . -name '*.c' ! -path './n1.ko/*')

debug:
	gcc -std=gnu2x -g -o n1 $(SRC_FILES) $(CFLAGS) -DDEBUG -fsanitize=address -fno-optimize-sibling-calls -lcurl -lm

release:
	gcc -std=gnu2x -O3 -o n1 $(SRC_FILES) $(CFLAGS) -lcurl -lm

clean:
	rm -rf *.o n1