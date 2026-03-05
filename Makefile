all:
	gcc -Wall --pedantic src/graph.c src/main.c src/output.c src/vector.c -o bin/main -lm