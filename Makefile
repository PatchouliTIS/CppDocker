C = gcc
CXX = g++
C_LIB = network.c nl.c
C_LINK = network.o nl.o
MAIN = main.cpp
LD = -std=c++11
OUT = docker-run

all:
	sudo sh -c '$(C) -c $(C_LIB)'
	sudo sh -c '$(CXX) $(LD) -o $(OUT) $(MAIN) $(C_LINK)'

clean:
	rm *.o $(OUT)
