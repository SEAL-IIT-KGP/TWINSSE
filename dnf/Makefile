CC=g++
CFLAGS=-I. -I./blake3/
CONFIG=-std=c++17 -O3 -msse2 -msse -msse4 -mssse3 -march=native -maes -lpthread -lgmpxx -lgmp -lhiredis -lredis++ -pthread -Wl,-rpath,/usr/local/lib,./blake3/libblake3.so

all: sse_setup sse_search

sse_setup: aes.cpp rawdatautil.cpp ecc_x25519.cpp bloom_filter.cpp ./blake3/blake_hash.cpp mainwindow.cpp sse_setup.cpp
	$(CC) $(CFLAGS) -o sse_setup aes.cpp rawdatautil.cpp ecc_x25519.cpp bloom_filter.cpp ./blake3/blake_hash.cpp mainwindow.cpp sse_setup.cpp $(CONFIG)

sse_search: aes.cpp rawdatautil.cpp ecc_x25519.cpp bloom_filter.cpp ./blake3/blake_hash.cpp mainwindow.cpp sse_search.cpp
	$(CC) $(CFLAGS) -o sse_search aes.cpp rawdatautil.cpp ecc_x25519.cpp bloom_filter.cpp ./blake3/blake_hash.cpp mainwindow.cpp sse_search.cpp $(CONFIG)

.PHONY: clean clean_all

clean:
	rm -rf *.o *.gch sse_setup sse_search

clean_all:
	rm -rf *.o *.gch sse_setup sse_search bloom_filter.dat eidxdb.csv
	@redis-cli flushall
	@redis-cli save