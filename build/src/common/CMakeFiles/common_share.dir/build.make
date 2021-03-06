# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list

# Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/mark/code/monitor_system/rx_board

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/mark/code/monitor_system/rx_board/build

# Include any dependencies generated for this target.
include src/common/CMakeFiles/common_share.dir/depend.make

# Include the progress variables for this target.
include src/common/CMakeFiles/common_share.dir/progress.make

# Include the compile flags for this target's objects.
include src/common/CMakeFiles/common_share.dir/flags.make

src/common/CMakeFiles/common_share.dir/err/wv_err.c.o: src/common/CMakeFiles/common_share.dir/flags.make
src/common/CMakeFiles/common_share.dir/err/wv_err.c.o: ../src/common/err/wv_err.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/mark/code/monitor_system/rx_board/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object src/common/CMakeFiles/common_share.dir/err/wv_err.c.o"
	cd /home/mark/code/monitor_system/rx_board/build/src/common && /home/mark/wellav/altera/common/tools/arm-linux-gcc-4.7/bin/arm-linux-gnueabihf-gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/common_share.dir/err/wv_err.c.o   -c /home/mark/code/monitor_system/rx_board/src/common/err/wv_err.c

src/common/CMakeFiles/common_share.dir/err/wv_err.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/common_share.dir/err/wv_err.c.i"
	cd /home/mark/code/monitor_system/rx_board/build/src/common && /home/mark/wellav/altera/common/tools/arm-linux-gcc-4.7/bin/arm-linux-gnueabihf-gcc  $(C_DEFINES) $(C_FLAGS) -E /home/mark/code/monitor_system/rx_board/src/common/err/wv_err.c > CMakeFiles/common_share.dir/err/wv_err.c.i

src/common/CMakeFiles/common_share.dir/err/wv_err.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/common_share.dir/err/wv_err.c.s"
	cd /home/mark/code/monitor_system/rx_board/build/src/common && /home/mark/wellav/altera/common/tools/arm-linux-gcc-4.7/bin/arm-linux-gnueabihf-gcc  $(C_DEFINES) $(C_FLAGS) -S /home/mark/code/monitor_system/rx_board/src/common/err/wv_err.c -o CMakeFiles/common_share.dir/err/wv_err.c.s

src/common/CMakeFiles/common_share.dir/err/wv_err.c.o.requires:
.PHONY : src/common/CMakeFiles/common_share.dir/err/wv_err.c.o.requires

src/common/CMakeFiles/common_share.dir/err/wv_err.c.o.provides: src/common/CMakeFiles/common_share.dir/err/wv_err.c.o.requires
	$(MAKE) -f src/common/CMakeFiles/common_share.dir/build.make src/common/CMakeFiles/common_share.dir/err/wv_err.c.o.provides.build
.PHONY : src/common/CMakeFiles/common_share.dir/err/wv_err.c.o.provides

src/common/CMakeFiles/common_share.dir/err/wv_err.c.o.provides.build: src/common/CMakeFiles/common_share.dir/err/wv_err.c.o

# Object files for target common_share
common_share_OBJECTS = \
"CMakeFiles/common_share.dir/err/wv_err.c.o"

# External object files for target common_share
common_share_EXTERNAL_OBJECTS =

lib/libcommon.so.1.0: src/common/CMakeFiles/common_share.dir/err/wv_err.c.o
lib/libcommon.so.1.0: src/common/CMakeFiles/common_share.dir/build.make
lib/libcommon.so.1.0: src/common/CMakeFiles/common_share.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C shared library ../../lib/libcommon.so"
	cd /home/mark/code/monitor_system/rx_board/build/src/common && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/common_share.dir/link.txt --verbose=$(VERBOSE)
	cd /home/mark/code/monitor_system/rx_board/build/src/common && $(CMAKE_COMMAND) -E cmake_symlink_library ../../lib/libcommon.so.1.0 ../../lib/libcommon.so.1 ../../lib/libcommon.so

lib/libcommon.so.1: lib/libcommon.so.1.0

lib/libcommon.so: lib/libcommon.so.1.0

# Rule to build all files generated by this target.
src/common/CMakeFiles/common_share.dir/build: lib/libcommon.so
.PHONY : src/common/CMakeFiles/common_share.dir/build

src/common/CMakeFiles/common_share.dir/requires: src/common/CMakeFiles/common_share.dir/err/wv_err.c.o.requires
.PHONY : src/common/CMakeFiles/common_share.dir/requires

src/common/CMakeFiles/common_share.dir/clean:
	cd /home/mark/code/monitor_system/rx_board/build/src/common && $(CMAKE_COMMAND) -P CMakeFiles/common_share.dir/cmake_clean.cmake
.PHONY : src/common/CMakeFiles/common_share.dir/clean

src/common/CMakeFiles/common_share.dir/depend:
	cd /home/mark/code/monitor_system/rx_board/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/mark/code/monitor_system/rx_board /home/mark/code/monitor_system/rx_board/src/common /home/mark/code/monitor_system/rx_board/build /home/mark/code/monitor_system/rx_board/build/src/common /home/mark/code/monitor_system/rx_board/build/src/common/CMakeFiles/common_share.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/common/CMakeFiles/common_share.dir/depend

