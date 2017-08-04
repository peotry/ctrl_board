#file(GLOB_RECURSE c_files ./*.c)

add_executable(broatcast_client broatcast_client.c)
target_link_libraries(broatcast_client net tools pthread)

add_executable(unicast_client unicast_client.c)
target_link_libraries(unicast_client net tools pthread)
