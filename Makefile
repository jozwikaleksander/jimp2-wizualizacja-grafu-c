all:
	gcc -Wall --pedantic src/*.c -o bin/main -lm -lplanarity

test:
	./bin/main examples/graph_1.txt

clean:
	rm -f *.bin *.txt