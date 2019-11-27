# args
CC = g++
CFLAGS  = -g -Wall -Weffc++ -std=c++11
DBGFLAGS = -g -std=c++11
LFLAGS  = -L/usr/lib

# All Targets
all: splflix

# linker
splflix: bin/Main.o
	$(CC) -o bin/splflix bin/Session.o bin/Watchable.o bin/User.o bin/PopularTag.o bin/Action.o bin/Main.o \
		$(LFLAGS)

# source files
bin/Main.o: src/Main.cpp bin/Session.o
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Main.o src/Main.cpp

bin/Session.o: include/Session.h src/Session.cpp bin/Watchable.o bin/User.o bin/Action.o
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Session.o src/Session.cpp

bin/Watchable.o: include/Watchable.h src/Watchable.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Watchable.o src/Watchable.cpp

bin/User.o: include/User.h src/User.cpp bin/PopularTag.o
	$(CC) $(CFLAGS) -c -Iinclude -o bin/User.o src/User.cpp

bin/Action.o: include/Action.h src/Action.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Action.o src/Action.cpp

bin/PopularTag.o: include/PopularTag.h src/PopularTag.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/PopularTag.o src/PopularTag.cpp



#Clean the build directory
.PHONY: all clean
clean:
	rm -f bin/*
