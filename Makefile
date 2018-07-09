SHELL = /bin/bash

INC = include
TESTS = tests
TESTS_BIN = $(TESTS)/bin

CXX = g++
CXXFLAGS = -std=c++17 -O2 $(CXXWARNINGS) -I$(INC)
CXXWARNINGS = -Wall -Wextra -pedantic

DEBUGGER = gdb

DEPS = $(wildcard $(INC)/*)
TEST_EXECS = $(TESTS_BIN)/validation_tests $(TESTS_BIN)/unit_tests
TEST_LIBS = /usr/local/lib/libboost_unit_test_framework.a

CLANG_TIDY = clang-tidy-6.0
CLANG_TIDY_FLAGS = -checks=$(CLANG_TIDY_CHECKS) -header-filter=.* -- $(CXXFLAGS)
CLANG_TIDY_CHECKS = '*,-google-*,-llvm-*,\
					-fuchsia-*,-*-braces-around-statements,\
					-readability-named-parameter,\
					-cppcoreguidelines-pro-bounds-constant-array-index'

all: compile exe

exe: compile
	@echo   "╔═════════════════════════╗"
	@echo   "║Ejecutando el programa...║"
	@printf "╚═════════════════════════╝\n\n"
	@./example

compile: example
	@printf "\n"

example: example.cpp $(INC)/aux.hpp $(INC)/polynomial.hpp
	@echo "Compiling $@........"
	$(CXX) $(CXXFLAGS) $< -o $@
	@echo "============================="

run_tests: validation_tests unit_tests

tests_setup:
	@if [ ! -d $(TESTS)/bin ]; then\
		mkdir $(TESTS)/bin;\
	fi

validation_tests: tests_setup $(TESTS_BIN)/validation_tests
	@printf	"\033[0;93m\n"
	@printf "╔══════════════════════════╗\n"
	@printf "║Executing validation tests║\n"
	@printf "╚══════════════════════════╝\n"
	@printf	"\033[0m"
	./$(TESTS)/bin/$@

unit_tests: tests_setup $(TESTS_BIN)/unit_tests
	@printf	"\033[0;93m\n"
	@printf "╔════════════════════╗\n"
	@printf "║Executing unit tests║\n"
	@printf "╚════════════════════╝\n"
	@printf	"\033[0m"
	./$(TESTS)/bin/$@

$(TESTS_BIN)/%: $(TESTS)/%.cpp $(DEPS)
	@printf "\033[0;97m[\033[0;92mCompiling $*\033[0;97m]\033[0m\n"
	$(CXX) $(CXXFLAGS) $< -o $@ $(TEST_LIBS)

clang_tidy_test: $(DEPS)
	@printf "\033[0;97m[\033[0;96mExecuting clang-tidy to find extra warnings\033[0;97m]\033[0m\n"
	@$(CLANG_TIDY) $(INC)/polynomial.hpp $(CLANG_TIDY_FLAGS)


.PHONY: clean
clean:
	@printf "\033[0;97m[\033[0;91mCleaning the working directory\033[0;97m]\033[0m\n"
	-rm -rf example $(TESTS)/bin/ 2> /dev/null
