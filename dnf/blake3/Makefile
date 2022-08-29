CC=g++
CFLAGS=-I.
# CONFIG_BLAKE=blake3.c blake3_dispatch.c blake3_portable.c blake3_sse2_x86-64_unix.S blake3_sse41_x86-64_unix.S blake3_avx2_x86-64_unix.S blake3_avx512_x86-64_unix.S
CONFIG=-std=c++17 -lpthread -Wl,-rpath,/usr/local/lib,./libblake3.so

all: blake_hash.cpp
	$(CC) -o blake_hash blake_hash.cpp $(CONFIG)

lib:
	gcc -c -fPIC -O3 -msse2 blake3_sse2.c -o blake3_sse2.o
	gcc -c -fPIC -O3 -msse4.1 blake3_sse41.c -o blake3_sse41.o
	gcc -c -fPIC -O3 -mavx2 blake3_avx2.c -o blake3_avx2.o
	gcc -c -fPIC -O3 -mavx512f -mavx512vl blake3_avx512.c -o blake3_avx512.o
	gcc -shared -O3 -fPIC -o libblake3.so blake3.c blake3_dispatch.c blake3_portable.c blake3_avx2.o blake3_avx512.o blake3_sse41.o blake3_sse2.o

clean:
	rm -rf *.o *.gch