# args
CC = g++
CFLAGS  = -g -Wall -Weffc++ -std=c++11
DBGFLAGS = -g -std=c++11 -o0
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

debug: include/PopularTag.h src/PopularTag.cpp include/Action.h src/Action.cpp include/User.h src/User.cpp \
        include/Watchable.h src/Watchable.cpp include/Session.h src/Session.cpp src/Main.cpp
	$(CC) $(DBGFLAGS) -c -Iinclude -o bin/PopularTag.o src/PopularTag.cpp
	$(CC) $(DBGFLAGS) -c -Iinclude -o bin/Action.o src/Action.cpp
	$(CC) $(DBGFLAGS) -c -Iinclude -o bin/User.o src/User.cpp
	$(CC) $(DBGFLAGS) -c -Iinclude -o bin/Watchable.o src/Watchable.cpp
	$(CC) $(DBGFLAGS) -c -Iinclude -o bin/Session.o src/Session.cpp
	$(CC) $(DBGFLAGS) -c -Iinclude -o bin/Main.o src/Main.cpp
	$(CC) -o bin/splflix bin/Session.o bin/Watchable.o bin/User.o bin/PopularTag.o bin/Action.o bin/Main.o $(LFLAGS)

test:
	make clean
	make debug
	$(CC) $(DBGFLAGS) -c -Iinclude -o bin/Main.o testing/check1_main.cpp
	$(CC) -o bin/splflix bin/Session.o bin/Watchable.o bin/User.o bin/PopularTag.o bin/Action.o bin/Main.o $(LFLAGS)
	./bin/splflix testing/config1.json testing/config2.json < testing/check1.in > testing/results/check1.log
	make debug
	./testing/test_Script.sh
#Clean the build directory
.PHONY: all clean debug test
clean:
	rm -f bin/*
