file(GLOB_RECURSE c_files ./*.c)

add_executable(test_multi_recv ${c_files})
target_link_libraries(test_multi_recv net tools pthread)

