all:
	gcc -Wall --pedantic src/*.c -o bin/main -lm -lplanarity

test:
	./bin/main examples/graph_1.txt

complex:
	 bin/main examples/graph_complex.txt -t t -o output.txt -s 1234 -a t

run:
	 bin/main examples/graph_1.txt -t t -o output.txt -s 1234 -a t


clean:
	rm -f *.bin *.txt