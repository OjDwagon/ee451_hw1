
all: p1a p1b p2

p1a: p1a.c
	gcc p1a.c -o p1a

p1b: p1b.c
	gcc p1b.c -o p1b

p2: p2.c
	gcc p2.c -o p2

.PHONY: clean
clean: 
	rm -rf p1a p1b p2