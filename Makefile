
all: problem1 problem2

problem1: problem1.c
	gcc problem1.c -o problem1

problem2: problem2.c
	gcc problem2.c -o problem2

.PHONY: clean
clean: 
	rm -rf problem1 problem2