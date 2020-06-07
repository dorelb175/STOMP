CFLAGS:=-c -Wall -Weffc++ -g -std=c++11 -Iinclude
LDFLAGS:=-lboost_system

all: StompBookClubClient
	g++ -pthread -o bin/StompBookClubClient bin/connectionHandler.o bin/StompBookClubClient.o bin/Book.o bin/Frame.o bin/Inventory.o bin/KeyboardInput.o bin/Topic.o bin/User.o $(LDFLAGS) 

StompBookClubClient: bin/connectionHandler.o bin/StompBookClubClient.o bin/Book.o bin/Frame.o bin/Inventory.o bin/KeyboardInput.o bin/Topic.o bin/User.o
	
bin/connectionHandler.o: src/connectionHandler.cpp
	g++ -pthread $(CFLAGS) -o bin/connectionHandler.o src/connectionHandler.cpp

bin/StompBookClubClient.o: src/StompBookClubClient.cpp
	g++ -pthread $(CFLAGS) -o bin/StompBookClubClient.o src/StompBookClubClient.cpp
	
bin/Book.o: src/Book.cpp
	g++ -pthread $(CFLAGS) -o bin/Book.o src/Book.cpp
	
bin/Frame.o: src/Frame.cpp
	g++ -pthread $(CFLAGS) -o bin/Frame.o src/Frame.cpp	
	
bin/Inventory.o: src/Inventory.cpp
	g++ -pthread $(CFLAGS) -o bin/Inventory.o src/Inventory.cpp
	
bin/KeyboardInput.o: src/KeyboardInput.cpp
	g++ -pthread $(CFLAGS) -o bin/KeyboardInput.o src/KeyboardInput.cpp
	
bin/Topic.o: src/Topic.cpp
	g++ -pthread $(CFLAGS) -o bin/Topic.o src/Topic.cpp
	
bin/User.o: src/User.cpp
	g++ -pthread $(CFLAGS) -o bin/User.o src/User.cpp
	
.PHONY: clean
clean:
	rm -f bin/*
