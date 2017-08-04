file(GLOB_RECURSE c_files ./*.c)

add_executable(timer_test ${c_files})
target_link_libraries(timer_test libtools.so pthread)
