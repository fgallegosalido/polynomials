BIN = bin
INC = include
SRC = source
TEST = tests
OBJ = obj

CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra -pedantic -g -I$(INC)

all: compile exe

exe: compile
	@printf "\n"
	@./example

compile: example
	@printf "\n"

example: example.cpp $(INC)/polynomial.hpp $(INC)/z_module.hpp $(INC)/z_module_primes.hpp $(SRC)/z_module.cpp $(SRC)/z_module_prime.cpp
	@echo "Compilando $@........"
	$(CXX) $(CXXFLAGS) $< -o $@

tests: $(TEST)/z_module_test

$(TEST)/z_module_test: $(TEST)/z_module_test.cpp $(INC)/z_module.hpp $(SRC)/z_module.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm example $(TEST)/z_module_test
