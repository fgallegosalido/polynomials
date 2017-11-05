BIN = bin
INC = inc
SRC = src
OBJ = obj

CXX = g++
CXXFLAGS = -std=c++14 -O2 -Wall -Wextra -pedantic -g -I$(INC)

all: main


main: main.cpp polynomial.hpp
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm main
