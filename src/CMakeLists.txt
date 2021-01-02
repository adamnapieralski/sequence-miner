
set(LIB_NAME sequence_miner_lib)

set(SRC_LIST    
    AlgorithmManager.cc    
    SequenceData.cc
    IdList.cc
    EquivalenceClass.cc
    Algorithm.cc
    SpadeAlgorithm.cc
    PrefixSpanAlgorithm.cc
    Parameters.cc
    utils.hpp
)

add_library(${LIB_NAME} ${SRC_LIST})

target_compile_features(${LIB_NAME} PUBLIC cxx_std_17)
set_target_properties(${LIB_NAME} PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(${LIB_NAME} PUBLIC yaml-cpp tbb pthread)
target_include_directories(${LIB_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
