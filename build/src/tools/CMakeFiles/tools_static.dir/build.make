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
include src/tools/CMakeFiles/tools_static.dir/depend.make

# Include the progress variables for this target.
include src/tools/CMakeFiles/tools_static.dir/progress.make

# Include the compile flags for this target's objects.
include src/tools/CMakeFiles/tools_static.dir/flags.make

src/tools/CMakeFiles/tools_static.dir/thread/thread.c.o: src/tools/CMakeFiles/tools_static.dir/flags.make
src/tools/CMakeFiles/tools_static.dir/thread/thread.c.o: ../src/tools/thread/thread.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/mark/code/monitor_system/rx_board/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object src/tools/CMakeFiles/tools_static.dir/thread/thread.c.o"
	cd /home/mark/code/monitor_system/rx_board/build/src/tools && /home/mark/wellav/altera/common/tools/arm-linux-gcc-4.7/bin/arm-linux-gnueabihf-gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/tools_static.dir/thread/thread.c.o   -c /home/mark/code/monitor_system/rx_board/src/tools/thread/thread.c

src/tools/CMakeFiles/tools_static.dir/thread/thread.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/tools_static.dir/thread/thread.c.i"
	cd /home/mark/code/monitor_system/rx_board/build/src/tools && /home/mark/wellav/altera/common/tools/arm-linux-gcc-4.7/bin/arm-linux-gnueabihf-gcc  $(C_DEFINES) $(C_FLAGS) -E /home/mark/code/monitor_system/rx_board/src/tools/thread/thread.c > CMakeFiles/tools_static.dir/thread/thread.c.i

src/tools/CMakeFiles/tools_static.dir/thread/thread.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/tools_static.dir/thread/thread.c.s"
	cd /home/mark/code/monitor_system/rx_board/build/src/tools && /home/mark/wellav/altera/common/tools/arm-linux-gcc-4.7/bin/arm-linux-gnueabihf-gcc  $(C_DEFINES) $(C_FLAGS) -S /home/mark/code/monitor_system/rx_board/src/tools/thread/thread.c -o CMakeFiles/tools_static.dir/thread/thread.c.s

src/tools/CMakeFiles/tools_static.dir/thread/thread.c.o.requires:
.PHONY : src/tools/CMakeFiles/tools_static.dir/thread/thread.c.o.requires

src/tools/CMakeFiles/tools_static.dir/thread/thread.c.o.provides: src/tools/CMakeFiles/tools_static.dir/thread/thread.c.o.requires
	$(MAKE) -f src/tools/CMakeFiles/tools_static.dir/build.make src/tools/CMakeFiles/tools_static.dir/thread/thread.c.o.provides.build
.PHONY : src/tools/CMakeFiles/tools_static.dir/thread/thread.c.o.provides

src/tools/CMakeFiles/tools_static.dir/thread/thread.c.o.provides.build: src/tools/CMakeFiles/tools_static.dir/thread/thread.c.o

src/tools/CMakeFiles/tools_static.dir/mutex.c.o: src/tools/CMakeFiles/tools_static.dir/flags.make
src/tools/CMakeFiles/tools_static.dir/mutex.c.o: ../src/tools/mutex.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/mark/code/monitor_system/rx_board/build/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object src/tools/CMakeFiles/tools_static.dir/mutex.c.o"
	cd /home/mark/code/monitor_system/rx_board/build/src/tools && /home/mark/wellav/altera/common/tools/arm-linux-gcc-4.7/bin/arm-linux-gnueabihf-gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/tools_static.dir/mutex.c.o   -c /home/mark/code/monitor_system/rx_board/src/tools/mutex.c

src/tools/CMakeFiles/tools_static.dir/mutex.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/tools_static.dir/mutex.c.i"
	cd /home/mark/code/monitor_system/rx_board/build/src/tools && /home/mark/wellav/altera/common/tools/arm-linux-gcc-4.7/bin/arm-linux-gnueabihf-gcc  $(C_DEFINES) $(C_FLAGS) -E /home/mark/code/monitor_system/rx_board/src/tools/mutex.c > CMakeFiles/tools_static.dir/mutex.c.i

src/tools/CMakeFiles/tools_static.dir/mutex.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/tools_static.dir/mutex.c.s"
	cd /home/mark/code/monitor_system/rx_board/build/src/tools && /home/mark/wellav/altera/common/tools/arm-linux-gcc-4.7/bin/arm-linux-gnueabihf-gcc  $(C_DEFINES) $(C_FLAGS) -S /home/mark/code/monitor_system/rx_board/src/tools/mutex.c -o CMakeFiles/tools_static.dir/mutex.c.s

src/tools/CMakeFiles/tools_static.dir/mutex.c.o.requires:
.PHONY : src/tools/CMakeFiles/tools_static.dir/mutex.c.o.requires

src/tools/CMakeFiles/tools_static.dir/mutex.c.o.provides: src/tools/CMakeFiles/tools_static.dir/mutex.c.o.requires
	$(MAKE) -f src/tools/CMakeFiles/tools_static.dir/build.make src/tools/CMakeFiles/tools_static.dir/mutex.c.o.provides.build
.PHONY : src/tools/CMakeFiles/tools_static.dir/mutex.c.o.provides

src/tools/CMakeFiles/tools_static.dir/mutex.c.o.provides.build: src/tools/CMakeFiles/tools_static.dir/mutex.c.o

src/tools/CMakeFiles/tools_static.dir/timer/timer.c.o: src/tools/CMakeFiles/tools_static.dir/flags.make
src/tools/CMakeFiles/tools_static.dir/timer/timer.c.o: ../src/tools/timer/timer.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/mark/code/monitor_system/rx_board/build/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object src/tools/CMakeFiles/tools_static.dir/timer/timer.c.o"
	cd /home/mark/code/monitor_system/rx_board/build/src/tools && /home/mark/wellav/altera/common/tools/arm-linux-gcc-4.7/bin/arm-linux-gnueabihf-gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/tools_static.dir/timer/timer.c.o   -c /home/mark/code/monitor_system/rx_board/src/tools/timer/timer.c

src/tools/CMakeFiles/tools_static.dir/timer/timer.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/tools_static.dir/timer/timer.c.i"
	cd /home/mark/code/monitor_system/rx_board/build/src/tools && /home/mark/wellav/altera/common/tools/arm-linux-gcc-4.7/bin/arm-linux-gnueabihf-gcc  $(C_DEFINES) $(C_FLAGS) -E /home/mark/code/monitor_system/rx_board/src/tools/timer/timer.c > CMakeFiles/tools_static.dir/timer/timer.c.i

src/tools/CMakeFiles/tools_static.dir/timer/timer.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/tools_static.dir/timer/timer.c.s"
	cd /home/mark/code/monitor_system/rx_board/build/src/tools && /home/mark/wellav/altera/common/tools/arm-linux-gcc-4.7/bin/arm-linux-gnueabihf-gcc  $(C_DEFINES) $(C_FLAGS) -S /home/mark/code/monitor_system/rx_board/src/tools/timer/timer.c -o CMakeFiles/tools_static.dir/timer/timer.c.s

src/tools/CMakeFiles/tools_static.dir/timer/timer.c.o.requires:
.PHONY : src/tools/CMakeFiles/tools_static.dir/timer/timer.c.o.requires

src/tools/CMakeFiles/tools_static.dir/timer/timer.c.o.provides: src/tools/CMakeFiles/tools_static.dir/timer/timer.c.o.requires
	$(MAKE) -f src/tools/CMakeFiles/tools_static.dir/build.make src/tools/CMakeFiles/tools_static.dir/timer/timer.c.o.provides.build
.PHONY : src/tools/CMakeFiles/tools_static.dir/timer/timer.c.o.provides

src/tools/CMakeFiles/tools_static.dir/timer/timer.c.o.provides.build: src/tools/CMakeFiles/tools_static.dir/timer/timer.c.o

src/tools/CMakeFiles/tools_static.dir/CRC/crc.c.o: src/tools/CMakeFiles/tools_static.dir/flags.make
src/tools/CMakeFiles/tools_static.dir/CRC/crc.c.o: ../src/tools/CRC/crc.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/mark/code/monitor_system/rx_board/build/CMakeFiles $(CMAKE_PROGRESS_4)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object src/tools/CMakeFiles/tools_static.dir/CRC/crc.c.o"
	cd /home/mark/code/monitor_system/rx_board/build/src/tools && /home/mark/wellav/altera/common/tools/arm-linux-gcc-4.7/bin/arm-linux-gnueabihf-gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/tools_static.dir/CRC/crc.c.o   -c /home/mark/code/monitor_system/rx_board/src/tools/CRC/crc.c

src/tools/CMakeFiles/tools_static.dir/CRC/crc.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/tools_static.dir/CRC/crc.c.i"
	cd /home/mark/code/monitor_system/rx_board/build/src/tools && /home/mark/wellav/altera/common/tools/arm-linux-gcc-4.7/bin/arm-linux-gnueabihf-gcc  $(C_DEFINES) $(C_FLAGS) -E /home/mark/code/monitor_system/rx_board/src/tools/CRC/crc.c > CMakeFiles/tools_static.dir/CRC/crc.c.i

src/tools/CMakeFiles/tools_static.dir/CRC/crc.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/tools_static.dir/CRC/crc.c.s"
	cd /home/mark/code/monitor_system/rx_board/build/src/tools && /home/mark/wellav/altera/common/tools/arm-linux-gcc-4.7/bin/arm-linux-gnueabihf-gcc  $(C_DEFINES) $(C_FLAGS) -S /home/mark/code/monitor_system/rx_board/src/tools/CRC/crc.c -o CMakeFiles/tools_static.dir/CRC/crc.c.s

src/tools/CMakeFiles/tools_static.dir/CRC/crc.c.o.requires:
.PHONY : src/tools/CMakeFiles/tools_static.dir/CRC/crc.c.o.requires

src/tools/CMakeFiles/tools_static.dir/CRC/crc.c.o.provides: src/tools/CMakeFiles/tools_static.dir/CRC/crc.c.o.requires
	$(MAKE) -f src/tools/CMakeFiles/tools_static.dir/build.make src/tools/CMakeFiles/tools_static.dir/CRC/crc.c.o.provides.build
.PHONY : src/tools/CMakeFiles/tools_static.dir/CRC/crc.c.o.provides

src/tools/CMakeFiles/tools_static.dir/CRC/crc.c.o.provides.build: src/tools/CMakeFiles/tools_static.dir/CRC/crc.c.o

src/tools/CMakeFiles/tools_static.dir/event/event.c.o: src/tools/CMakeFiles/tools_static.dir/flags.make
src/tools/CMakeFiles/tools_static.dir/event/event.c.o: ../src/tools/event/event.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/mark/code/monitor_system/rx_board/build/CMakeFiles $(CMAKE_PROGRESS_5)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object src/tools/CMakeFiles/tools_static.dir/event/event.c.o"
	cd /home/mark/code/monitor_system/rx_board/build/src/tools && /home/mark/wellav/altera/common/tools/arm-linux-gcc-4.7/bin/arm-linux-gnueabihf-gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/tools_static.dir/event/event.c.o   -c /home/mark/code/monitor_system/rx_board/src/tools/event/event.c

src/tools/CMakeFiles/tools_static.dir/event/event.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/tools_static.dir/event/event.c.i"
	cd /home/mark/code/monitor_system/rx_board/build/src/tools && /home/mark/wellav/altera/common/tools/arm-linux-gcc-4.7/bin/arm-linux-gnueabihf-gcc  $(C_DEFINES) $(C_FLAGS) -E /home/mark/code/monitor_system/rx_board/src/tools/event/event.c > CMakeFiles/tools_static.dir/event/event.c.i

src/tools/CMakeFiles/tools_static.dir/event/event.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/tools_static.dir/event/event.c.s"
	cd /home/mark/code/monitor_system/rx_board/build/src/tools && /home/mark/wellav/altera/common/tools/arm-linux-gcc-4.7/bin/arm-linux-gnueabihf-gcc  $(C_DEFINES) $(C_FLAGS) -S /home/mark/code/monitor_system/rx_board/src/tools/event/event.c -o CMakeFiles/tools_static.dir/event/event.c.s

src/tools/CMakeFiles/tools_static.dir/event/event.c.o.requires:
.PHONY : src/tools/CMakeFiles/tools_static.dir/event/event.c.o.requires

src/tools/CMakeFiles/tools_static.dir/event/event.c.o.provides: src/tools/CMakeFiles/tools_static.dir/event/event.c.o.requires
	$(MAKE) -f src/tools/CMakeFiles/tools_static.dir/build.make src/tools/CMakeFiles/tools_static.dir/event/event.c.o.provides.build
.PHONY : src/tools/CMakeFiles/tools_static.dir/event/event.c.o.provides

src/tools/CMakeFiles/tools_static.dir/event/event.c.o.provides.build: src/tools/CMakeFiles/tools_static.dir/event/event.c.o

src/tools/CMakeFiles/tools_static.dir/log/log.c.o: src/tools/CMakeFiles/tools_static.dir/flags.make
src/tools/CMakeFiles/tools_static.dir/log/log.c.o: ../src/tools/log/log.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/mark/code/monitor_system/rx_board/build/CMakeFiles $(CMAKE_PROGRESS_6)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object src/tools/CMakeFiles/tools_static.dir/log/log.c.o"
	cd /home/mark/code/monitor_system/rx_board/build/src/tools && /home/mark/wellav/altera/common/tools/arm-linux-gcc-4.7/bin/arm-linux-gnueabihf-gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/tools_static.dir/log/log.c.o   -c /home/mark/code/monitor_system/rx_board/src/tools/log/log.c

src/tools/CMakeFiles/tools_static.dir/log/log.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/tools_static.dir/log/log.c.i"
	cd /home/mark/code/monitor_system/rx_board/build/src/tools && /home/mark/wellav/altera/common/tools/arm-linux-gcc-4.7/bin/arm-linux-gnueabihf-gcc  $(C_DEFINES) $(C_FLAGS) -E /home/mark/code/monitor_system/rx_board/src/tools/log/log.c > CMakeFiles/tools_static.dir/log/log.c.i

src/tools/CMakeFiles/tools_static.dir/log/log.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/tools_static.dir/log/log.c.s"
	cd /home/mark/code/monitor_system/rx_board/build/src/tools && /home/mark/wellav/altera/common/tools/arm-linux-gcc-4.7/bin/arm-linux-gnueabihf-gcc  $(C_DEFINES) $(C_FLAGS) -S /home/mark/code/monitor_system/rx_board/src/tools/log/log.c -o CMakeFiles/tools_static.dir/log/log.c.s

src/tools/CMakeFiles/tools_static.dir/log/log.c.o.requires:
.PHONY : src/tools/CMakeFiles/tools_static.dir/log/log.c.o.requires

src/tools/CMakeFiles/tools_static.dir/log/log.c.o.provides: src/tools/CMakeFiles/tools_static.dir/log/log.c.o.requires
	$(MAKE) -f src/tools/CMakeFiles/tools_static.dir/build.make src/tools/CMakeFiles/tools_static.dir/log/log.c.o.provides.build
.PHONY : src/tools/CMakeFiles/tools_static.dir/log/log.c.o.provides

src/tools/CMakeFiles/tools_static.dir/log/log.c.o.provides.build: src/tools/CMakeFiles/tools_static.dir/log/log.c.o

src/tools/CMakeFiles/tools_static.dir/mem/mem.c.o: src/tools/CMakeFiles/tools_static.dir/flags.make
src/tools/CMakeFiles/tools_static.dir/mem/mem.c.o: ../src/tools/mem/mem.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/mark/code/monitor_system/rx_board/build/CMakeFiles $(CMAKE_PROGRESS_7)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object src/tools/CMakeFiles/tools_static.dir/mem/mem.c.o"
	cd /home/mark/code/monitor_system/rx_board/build/src/tools && /home/mark/wellav/altera/common/tools/arm-linux-gcc-4.7/bin/arm-linux-gnueabihf-gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/tools_static.dir/mem/mem.c.o   -c /home/mark/code/monitor_system/rx_board/src/tools/mem/mem.c

src/tools/CMakeFiles/tools_static.dir/mem/mem.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/tools_static.dir/mem/mem.c.i"
	cd /home/mark/code/monitor_system/rx_board/build/src/tools && /home/mark/wellav/altera/common/tools/arm-linux-gcc-4.7/bin/arm-linux-gnueabihf-gcc  $(C_DEFINES) $(C_FLAGS) -E /home/mark/code/monitor_system/rx_board/src/tools/mem/mem.c > CMakeFiles/tools_static.dir/mem/mem.c.i

src/tools/CMakeFiles/tools_static.dir/mem/mem.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/tools_static.dir/mem/mem.c.s"
	cd /home/mark/code/monitor_system/rx_board/build/src/tools && /home/mark/wellav/altera/common/tools/arm-linux-gcc-4.7/bin/arm-linux-gnueabihf-gcc  $(C_DEFINES) $(C_FLAGS) -S /home/mark/code/monitor_system/rx_board/src/tools/mem/mem.c -o CMakeFiles/tools_static.dir/mem/mem.c.s

src/tools/CMakeFiles/tools_static.dir/mem/mem.c.o.requires:
.PHONY : src/tools/CMakeFiles/tools_static.dir/mem/mem.c.o.requires

src/tools/CMakeFiles/tools_static.dir/mem/mem.c.o.provides: src/tools/CMakeFiles/tools_static.dir/mem/mem.c.o.requires
	$(MAKE) -f src/tools/CMakeFiles/tools_static.dir/build.make src/tools/CMakeFiles/tools_static.dir/mem/mem.c.o.provides.build
.PHONY : src/tools/CMakeFiles/tools_static.dir/mem/mem.c.o.provides

src/tools/CMakeFiles/tools_static.dir/mem/mem.c.o.provides.build: src/tools/CMakeFiles/tools_static.dir/mem/mem.c.o

# Object files for target tools_static
tools_static_OBJECTS = \
"CMakeFiles/tools_static.dir/thread/thread.c.o" \
"CMakeFiles/tools_static.dir/mutex.c.o" \
"CMakeFiles/tools_static.dir/timer/timer.c.o" \
"CMakeFiles/tools_static.dir/CRC/crc.c.o" \
"CMakeFiles/tools_static.dir/event/event.c.o" \
"CMakeFiles/tools_static.dir/log/log.c.o" \
"CMakeFiles/tools_static.dir/mem/mem.c.o"

# External object files for target tools_static
tools_static_EXTERNAL_OBJECTS =

lib/libtools.a: src/tools/CMakeFiles/tools_static.dir/thread/thread.c.o
lib/libtools.a: src/tools/CMakeFiles/tools_static.dir/mutex.c.o
lib/libtools.a: src/tools/CMakeFiles/tools_static.dir/timer/timer.c.o
lib/libtools.a: src/tools/CMakeFiles/tools_static.dir/CRC/crc.c.o
lib/libtools.a: src/tools/CMakeFiles/tools_static.dir/event/event.c.o
lib/libtools.a: src/tools/CMakeFiles/tools_static.dir/log/log.c.o
lib/libtools.a: src/tools/CMakeFiles/tools_static.dir/mem/mem.c.o
lib/libtools.a: src/tools/CMakeFiles/tools_static.dir/build.make
lib/libtools.a: src/tools/CMakeFiles/tools_static.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C static library ../../lib/libtools.a"
	cd /home/mark/code/monitor_system/rx_board/build/src/tools && $(CMAKE_COMMAND) -P CMakeFiles/tools_static.dir/cmake_clean_target.cmake
	cd /home/mark/code/monitor_system/rx_board/build/src/tools && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/tools_static.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/tools/CMakeFiles/tools_static.dir/build: lib/libtools.a
.PHONY : src/tools/CMakeFiles/tools_static.dir/build

src/tools/CMakeFiles/tools_static.dir/requires: src/tools/CMakeFiles/tools_static.dir/thread/thread.c.o.requires
src/tools/CMakeFiles/tools_static.dir/requires: src/tools/CMakeFiles/tools_static.dir/mutex.c.o.requires
src/tools/CMakeFiles/tools_static.dir/requires: src/tools/CMakeFiles/tools_static.dir/timer/timer.c.o.requires
src/tools/CMakeFiles/tools_static.dir/requires: src/tools/CMakeFiles/tools_static.dir/CRC/crc.c.o.requires
src/tools/CMakeFiles/tools_static.dir/requires: src/tools/CMakeFiles/tools_static.dir/event/event.c.o.requires
src/tools/CMakeFiles/tools_static.dir/requires: src/tools/CMakeFiles/tools_static.dir/log/log.c.o.requires
src/tools/CMakeFiles/tools_static.dir/requires: src/tools/CMakeFiles/tools_static.dir/mem/mem.c.o.requires
.PHONY : src/tools/CMakeFiles/tools_static.dir/requires

src/tools/CMakeFiles/tools_static.dir/clean:
	cd /home/mark/code/monitor_system/rx_board/build/src/tools && $(CMAKE_COMMAND) -P CMakeFiles/tools_static.dir/cmake_clean.cmake
.PHONY : src/tools/CMakeFiles/tools_static.dir/clean

src/tools/CMakeFiles/tools_static.dir/depend:
	cd /home/mark/code/monitor_system/rx_board/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/mark/code/monitor_system/rx_board /home/mark/code/monitor_system/rx_board/src/tools /home/mark/code/monitor_system/rx_board/build /home/mark/code/monitor_system/rx_board/build/src/tools /home/mark/code/monitor_system/rx_board/build/src/tools/CMakeFiles/tools_static.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/tools/CMakeFiles/tools_static.dir/depend

