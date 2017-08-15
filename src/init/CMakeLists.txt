file(GLOB_RECURSE c_files ./*.c)

set(TARGET1 ctrl_board)
set(TARGET2 phy_reg)

add_executable(${TARGET1} main.c)
target_link_libraries(${TARGET1} libtools.a libdrivers.a pthread)

add_executable(${TARGET2} phy_reg.c)
target_link_libraries(${TARGET2} libtools.a libdrivers.a pthread)
