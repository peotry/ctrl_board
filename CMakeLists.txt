cmake_minimum_required(VERSION 2.6)

project(7u_rx)

#设置库文件存放路径
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build/lib)

#设置执行文件存放路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build/bin)

#获取当前目录及子目录(递归获取),添加到头文件搜索路径
function(include_sub_directories_recursively root_dir)
    if (IS_DIRECTORY ${root_dir})               # 当前路径是一个目录吗，是的话就加入到包含目录
        message("include dir: " ${root_dir})
        include_directories(${root_dir})
    endif()

    file(GLOB ALL_SUB RELATIVE ${root_dir} ${root_dir}/*) # 获得当前目录下的所有文件，让如ALL_SUB列表中
    foreach(sub ${ALL_SUB})
        if (IS_DIRECTORY ${root_dir}/${sub})
            include_sub_directories_recursively(${root_dir}/${sub}) # 对子目录递归调用，包含
        endif()
    endforeach()
endfunction()

#项目的所有目录都为头文件搜索路径
include_sub_directories_recursively(${PROJECT_SOURCE_DIR})
#include_directories()

#添加库文件搜索路径
link_directories(
    ${PROJECT_SOURCE_DIR}/build/lib
    ${PROJECT_SOURCE_DIR}/lib/libxml2/lib
    )

#添加子目录
add_subdirectory(src)
#add_subdirectory(test_multi_send)
#add_subdirectory(test_multi_recv)
#add_subdirectory(test_xml)
#add_subdirectory(test_timer)
#add_subdirectory(test_udp_server)
#add_subdirectory(test_udp_client)


#设置编译选项
set(CMAKE_C_FLAGS "-g -Wall")

#设置编译器
set(CMAKE_C_COMPILER /home/mark/public/Wellav/NB100A/compile_tool/arm_xilinx_tool/xilinx/bin/arm-xilinx-linux-gnueabi-gcc)
message("Compiler :" ${CMAKE_C_COMPILER})

#设置安装目录
set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/install)
