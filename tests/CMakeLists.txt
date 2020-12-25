
set(TEST_TARGET sequence-miner-tests)

set(SRC_LIST 
    test.cc
)

add_executable(${TEST_TARGET} ${SRC_LIST})
target_compile_features(${TEST_TARGET} PRIVATE cxx_std_17)
set_target_properties(${TEST_TARGET} PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(${TEST_TARGET} PRIVATE sequence_miner_lib Catch2::Catch)
add_test(NAME test_miner COMMAND ${TEST_TARGET}) 
