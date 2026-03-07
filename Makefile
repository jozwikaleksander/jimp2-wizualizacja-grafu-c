all:
	gcc -Wall --pedantic src/*.c -o bin/main -lm

test:
	./bin/main examples/graph_1.txt