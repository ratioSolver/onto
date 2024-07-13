add_executable(onto_tests test_onto.cpp)
target_link_libraries(onto_tests PRIVATE onto)

add_test(NAME ontoTest00 COMMAND onto_tests WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})