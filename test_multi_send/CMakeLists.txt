file(GLOB_RECURSE c_files ./*.c)

add_executable(test_multi_send ${c_files})
target_link_libraries(test_multi_send net tools pthread)

