enable_testing()

# Find several Boost dependencies
find_package(Boost 1.70 REQUIRED COMPONENTS unit_test_framework)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
else()
    message(SEND_ERROR "No se pudo encontrar la librería Boost")
endif()

add_executable(unit_tests unit_tests.cpp)
target_link_libraries(unit_tests
    polynomials
    project_options
    project_warnings
    Boost::unit_test_framework
)

add_test(NAME unit_tests COMMAND unit_tests)
