CFLAGS = -Wall -Wconversion -Wextra

debug:
	gcc -std=gnu2x -g -o n1 ${shell find . -name '*.c'} $(CFLAGS) -DDEBUG -fsanitize=address -fno-optimize-sibling-calls -lcurl -lm

release:
	gcc -std=gnu2x -O3 -o n1 ${shell find . -name '*.c'} $(CFLAGS) -lcurl -lm

clean:
	rm -rf *.o n1