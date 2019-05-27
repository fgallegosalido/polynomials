cmake_minimum_required(VERSION 3.5)
project(POLYNOMIALS
    DESCRIPTION "C++17 library for manipulating polynomials"
    LANGUAGES CXX
)

# Option for static analyzer
option(ENABLE_CLANG_TIDY "Enable testing with clang-tidy" FALSE)

option(BUILD_EXAMPLES   "Builds examples"   FALSE)
option(BUILD_TESTS      "Builds tests"      FALSE)
option(BUILD_BENCHMARKS "Builds benchmarks" FALSE)

if(BUILD_EXAMPLES OR BUILD_TESTS OR BUILD_BENCHMARKS)
    # Link this 'library' to set the c++ standard / compile-time options requested
    add_library(project_options INTERFACE)
    # Use C++20 standar
    target_compile_features(project_options INTERFACE cxx_std_20)

    # Link this 'library' to use the standard warnings
    add_library(project_warnings INTERFACE)
    target_compile_options(project_warnings INTERFACE
        -Wall
        -Wextra
        -Wpedantic
        -Wshadow
        -Wold-style-cast
        -Wcast-align
        -Wnon-virtual-dtor
        -Woverloaded-virtual
    )

    if(ENABLE_CLANG_TIDY)
        find_program(CLANGTIDY clang-tidy)
        if(CLANGTIDY)
            set(CMAKE_CXX_CLANG_TIDY ${CLANGTIDY})
        else()
            message(SEND_ERROR "clang-tidy requested but executable not found")
        endif()
    endif()
endif()


if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
if(BUILD_BENCHMARKS)
    add_subdirectory(benchmarks)
endif()

add_library(polynomials INTERFACE)
target_include_directories(polynomials INTERFACE include/)
