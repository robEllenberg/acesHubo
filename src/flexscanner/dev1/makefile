all:
	flex --header-file=robotis.yy.h --outfile=robotis.yy.c robotis.l 
	g++ -g -c packet.cpp
	g++ -g robotis.yy.c packet.o -static -lfl -o test

tiny:
	flex --header-file=robotis.yy.h --outfile=robotis.yy.c robotis.l 
	g++ -Os -c packet.cpp
	g++ -Os robotis.yy.c packet.o -static -lfl -o test
	strip test

pre:
	flex --header-file=robotis.yy.h robotis.l 

test:: all
	./test < read-bin

clean:
	@-rm robotis.yy.c robotis.yy.h *.o *.opp
	@-rm test

