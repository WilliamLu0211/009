all: main.o image.o matrix.o gmath.o
	g++ main.o image.o matrix.o gmath.o
	./a.out < script

main.o: main.cpp image.h
	g++ -c main.cpp

image.o: image.cpp image.h matrix.h gmath.h
	g++ -c image.cpp

matrix.o: matrix.cpp matrix.h
	g++ -c matrix.cpp

gmath.o: gmath.cpp gmath.h matrix.h
	g++ -c gmath.cpp

clean:
	rm *.o
	rm *.png
