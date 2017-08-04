file(GLOB_RECURSE c_files ./*.c)

set(TARGET1 rx_board)

add_executable(${TARGET1} ${c_files})
target_link_libraries(${TARGET1} libtools.a libdrivers.a pthread)
