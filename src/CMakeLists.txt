
set(LIB_NAME sequence_miner_lib)

set(SRC_LIST    
    AlgorithmManager.cc
    InputData.cc
    SpadeAlgorithm.cc
    PrefixSpanAlgorithm.cc
)

add_library(${LIB_NAME} ${SRC_LIST})

target_compile_features(${LIB_NAME} PUBLIC cxx_std_17)
set_target_properties(${LIB_NAME} PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(${LIB_NAME} PUBLIC yaml-cpp)
target_include_directories(${LIB_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
