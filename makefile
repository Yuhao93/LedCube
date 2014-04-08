INCLUDE=-Iinclude
SRCS=src/*.cpp
PARAMS=-std=c++0x -lwiringPi -lpthread -o bin/ledcube

all: cpp
dir: ; mkdir -p bin
cpp: dir $(SRCS) ; g++ $(INCLUDE) $(SRCS) $(PARAMS)
clean: ; rm -rf bin

