build: typing.o
	g++ typing.o -o typing

run: build
	./typing

typing.o : typing.cpp
	g++ -std=c++11 -Wall typing.cpp -c -o typing.o

clean:
	rm -f typing
	rm -f typing.o