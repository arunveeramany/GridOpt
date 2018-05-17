# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.7

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


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
CMAKE_COMMAND = /projects/opt/centos7/cmake/3.7.1/bin/cmake

# The command to remove a file.
RM = /projects/opt/centos7/cmake/3.7.1/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/hlh/private-gravity

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/hlh/private-gravity/build

# Include any dependencies generated for this target.
include examples/CMakeFiles/gravity101.dir/depend.make

# Include the progress variables for this target.
include examples/CMakeFiles/gravity101.dir/progress.make

# Include the compile flags for this target's objects.
include examples/CMakeFiles/gravity101.dir/flags.make

examples/CMakeFiles/gravity101.dir/Gravity101_main.cpp.o: examples/CMakeFiles/gravity101.dir/flags.make
examples/CMakeFiles/gravity101.dir/Gravity101_main.cpp.o: ../examples/Gravity101_main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/hlh/private-gravity/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object examples/CMakeFiles/gravity101.dir/Gravity101_main.cpp.o"
	cd /home/hlh/private-gravity/build/examples && /projects/opt/centos7/gcc/8.1.0/bin/g++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/gravity101.dir/Gravity101_main.cpp.o -c /home/hlh/private-gravity/examples/Gravity101_main.cpp

examples/CMakeFiles/gravity101.dir/Gravity101_main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/gravity101.dir/Gravity101_main.cpp.i"
	cd /home/hlh/private-gravity/build/examples && /projects/opt/centos7/gcc/8.1.0/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/hlh/private-gravity/examples/Gravity101_main.cpp > CMakeFiles/gravity101.dir/Gravity101_main.cpp.i

examples/CMakeFiles/gravity101.dir/Gravity101_main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/gravity101.dir/Gravity101_main.cpp.s"
	cd /home/hlh/private-gravity/build/examples && /projects/opt/centos7/gcc/8.1.0/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/hlh/private-gravity/examples/Gravity101_main.cpp -o CMakeFiles/gravity101.dir/Gravity101_main.cpp.s

examples/CMakeFiles/gravity101.dir/Gravity101_main.cpp.o.requires:

.PHONY : examples/CMakeFiles/gravity101.dir/Gravity101_main.cpp.o.requires

examples/CMakeFiles/gravity101.dir/Gravity101_main.cpp.o.provides: examples/CMakeFiles/gravity101.dir/Gravity101_main.cpp.o.requires
	$(MAKE) -f examples/CMakeFiles/gravity101.dir/build.make examples/CMakeFiles/gravity101.dir/Gravity101_main.cpp.o.provides.build
.PHONY : examples/CMakeFiles/gravity101.dir/Gravity101_main.cpp.o.provides

examples/CMakeFiles/gravity101.dir/Gravity101_main.cpp.o.provides.build: examples/CMakeFiles/gravity101.dir/Gravity101_main.cpp.o


# Object files for target gravity101
gravity101_OBJECTS = \
"CMakeFiles/gravity101.dir/Gravity101_main.cpp.o"

# External object files for target gravity101
gravity101_EXTERNAL_OBJECTS =

../bin/gravity101: examples/CMakeFiles/gravity101.dir/Gravity101_main.cpp.o
../bin/gravity101: examples/CMakeFiles/gravity101.dir/build.make
../bin/gravity101: ../lib/libgravity.so
../bin/gravity101: ../ext_lib/CoinIpopt/build/lib/libipopt.so
../bin/gravity101: ../ext_lib/xlnt-master/build/source/libxlnt.so
../bin/gravity101: ../ext_lib/xlnt-master/build/source/libxlnt.so
../bin/gravity101: ext_lib/armadillo-8.100.0/libarmadillo.so.8.100.0
../bin/gravity101: /usr/lib64/libopenblaso.so
../bin/gravity101: /usr/lib64/liblapack.so
../bin/gravity101: /usr/lib64/libhdf5.so
../bin/gravity101: /usr/lib64/libsz.so
../bin/gravity101: /usr/lib64/libz.so
../bin/gravity101: /usr/lib64/libdl.so
../bin/gravity101: /usr/lib64/libm.so
../bin/gravity101: ext_lib/cpp_option_parser/liboption_parser.so
../bin/gravity101: examples/CMakeFiles/gravity101.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/hlh/private-gravity/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../../bin/gravity101"
	cd /home/hlh/private-gravity/build/examples && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/gravity101.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
examples/CMakeFiles/gravity101.dir/build: ../bin/gravity101

.PHONY : examples/CMakeFiles/gravity101.dir/build

examples/CMakeFiles/gravity101.dir/requires: examples/CMakeFiles/gravity101.dir/Gravity101_main.cpp.o.requires

.PHONY : examples/CMakeFiles/gravity101.dir/requires

examples/CMakeFiles/gravity101.dir/clean:
	cd /home/hlh/private-gravity/build/examples && $(CMAKE_COMMAND) -P CMakeFiles/gravity101.dir/cmake_clean.cmake
.PHONY : examples/CMakeFiles/gravity101.dir/clean

examples/CMakeFiles/gravity101.dir/depend:
	cd /home/hlh/private-gravity/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/hlh/private-gravity /home/hlh/private-gravity/examples /home/hlh/private-gravity/build /home/hlh/private-gravity/build/examples /home/hlh/private-gravity/build/examples/CMakeFiles/gravity101.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : examples/CMakeFiles/gravity101.dir/depend
