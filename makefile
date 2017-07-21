icicSim:Def.o main.o
	g++ -o icicSim Def.o main.o
Def.o:Def.cpp Def.h
	g++ -c Def.cpp
main.o:main.cpp
	g++ -c main.cpp

clean:
	rm -f icicSim Def.o main.o