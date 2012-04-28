CFLAGS = -g
CXXFLAGS = -g
LDFLAGS = -g

a.out: block_mult.o lib.o main.o solve.o 
	g++ $(LDFLAGS) -o $@ $^

block_mult.o: block_mult.h
lib.o: lib.h
main.o: lib.h solve.h
solve.o: lib.h block_mult.h solve.h
