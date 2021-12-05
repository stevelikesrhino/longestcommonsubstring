

llcs: main.o
	g++ main.o -o llcs

main.o: main.cpp
	g++ -c main.cpp

.PHONY: clean
clean:
	rm *.o llcs
