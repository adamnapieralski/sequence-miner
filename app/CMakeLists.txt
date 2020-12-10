
set(APP_NAME miner)

set(SRC_LIST
    main.cc
)

add_executable(${APP_NAME} ${SRC_LIST})

target_compile_features(${APP_NAME} PUBLIC cxx_std_11)
set_target_properties(${APP_NAME} PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(${APP_NAME} PRIVATE sequence_miner_lib)

