all:server client switch
server:server.cpp tknlib.o
	g++ -pthread server.cpp tknlib.o -o server
client:client.cpp tknlib.o
	g++ -pthread client.cpp tknlib.o -o client
switch:switch.cpp tknlib.o
	g++ -pthread switch.cpp tknlib.o -o switch
tknlib.o:tknlib.h tknlib.cpp
	g++ -c tknlib.cpp
Clean:
	rm -rv *.o server client

