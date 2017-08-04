#file(GLOB_RECURSE c_files ./*.c)

add_executable(broatcast_server broatcast_server.c)
target_link_libraries(broatcast_server net tools pthread)

add_executable(unicast_server unicast_server.c)
target_link_libraries(unicast_server net tools pthread)
