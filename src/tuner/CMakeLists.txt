file(GLOB_RECURSE c_files ./*.c)

set(LIB_NAME tuner)
set(LIB_SHARE_NAME ${LIB_NAME}_share)
set(LIB_STATIC_NAME ${LIB_NAME}_static)

add_library(${LIB_SHARE_NAME} SHARED ${c_files})
add_library(${LIB_STATIC_NAME} STATIC ${c_files})

set_target_properties(${LIB_SHARE_NAME} PROPERTIES OUTPUT_NAME ${LIB_NAME})
set_target_properties(${LIB_STATIC_NAME} PROPERTIES OUTPUT_NAME ${LIB_NAME})

set_target_properties(${LIB_SHARE_NAME} PROPERTIES VERSION 1.0 SOVERSION 1)
