BIN = bin
INC = inc
SRC = src
OBJ = obj

CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra -pedantic -g -I$(INC) -ftemplate-depth=65536

all: compile exe

exe: compile
	@printf "\n"
	@./example

compile: example
	@printf "\n"

example: example.cpp polynomial.hpp z_module.hpp
	@echo "Compilando $@........"
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm main
