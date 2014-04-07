INCLUDE=-Iinclude
SRCS=src/*.cpp
PARAMS=-std=c++0x -lwiringPi -o bin/ledcube

all: cpp
dir: ; mkdir -p bin
cpp: dir src/main.cpp ; g++ $(INCLUDE) $(SRCS) $(PARAMS)
clean: ; rm -rf bin

