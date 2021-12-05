all: llcs rand_gen

llcs: main.o
	g++ main.o -o llcs

rand_gen: rstrg.o
	g++ rstrg.o -o rand_gen

main.o: main.cpp
	g++ -c main.cpp

rstrg.o: rstrg.cpp
	g++ -c rstrg.cpp

.PHONY: clean
clean:
	rm *.o llcs rand_gen
