cxx:=g++

all: ecc

ecc: main.o 
	      $(cxx) main.o -o build/ecc

main.o: sources/main.cpp
	$(cxx) -c sources/main.cpp

clean:
	rm -rf *o build/ecc