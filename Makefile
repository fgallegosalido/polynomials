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

example: example.cpp $(INC)/aux.hpp $(INC)/polynomial.hpp $(INC)/z_module.hpp $(INC)/z_module_prime.hpp $(SRC)/z_module.cpp $(SRC)/z_module_prime.cpp $(SRC)/z_module_arithmetics.cpp $(SRC)/z_module_comparisons.cpp
	@echo "Compilando $@........"
	$(CXX) $(CXXFLAGS) $< -o $@

tests: $(TEST)/z_module_test

$(TEST)/z_module_test: $(INC)/aux.hpp $(TEST)/z_module_test.cpp $(INC)/z_module.hpp $(INC)/z_module_prime.hpp $(SRC)/z_module.cpp $(SRC)/z_module_prime.cpp $(SRC)/z_module_arithmetics.cpp $(SRC)/z_module_comparisons.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm example $(TEST)/z_module_test
