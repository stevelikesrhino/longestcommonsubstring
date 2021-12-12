all: llcs rand_gen

llcs: main_mt.o
	g++ main_mt.o -O2 -mavx2 -lpthread -o llcs

rand_gen: rstrg.o
	g++ rstrg.o -o rand_gen

main_mt.o: main_mt.cpp
	g++ -c -O2 -mavx2 main_mt.cpp -lpthread

rstrg.o: rstrg.cpp
	g++ -c rstrg.cpp

.PHONY: clean
clean:
	rm *.o llcs rand_gen
