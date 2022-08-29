CC=g++
CFLAGS = -std=c++17 -march=native -O3 -funroll-loops -fstrict-aliasing
CONFIG = -I. -lpthread -lcryptopp -Wl,-rpath,/usr/local/lib

all: gen_mkw.cpp dis_query_process.cpp dis_query_generation.cpp main.cpp
	$(CC) $(CFLAGS) -o gen_db gen_mkw.cpp dis_query_process.cpp dis_query_generation.cpp main.cpp $(CONFIG)

.PHONY: clean
clean:
	rm -rf *.o *.gch gen_db