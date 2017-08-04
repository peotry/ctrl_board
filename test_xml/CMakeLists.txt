file(GLOB_RECURSE c_files ./*.c)

add_executable(test_xml ${c_files})
target_link_libraries(test_xml libxml.a libtools.a xml2)
