#compiler
CC = g++

compile : server client

server: server.o
	g++ server.o -o server -std=c++11 -pthread -std=c++1z

client: client.o
	g++ -o client client.cpp

server.o: server.cpp
	g++ -c server.cpp -std=c++11 -pthread -std=c++1z

client.o: client.cpp
	g++ -c client.cpp

clean:
	rm -rf *.o compile
