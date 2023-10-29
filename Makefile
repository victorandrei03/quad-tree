CC = gcc
CFLAGS = -Wall -std=c99

build: quad-tree.c
	$(CC) $(CFLAGS) -o quadtree quad-tree.c

run: quadtree
	./quadtree

.PHONY : clean
clean :
	rm quadtree
