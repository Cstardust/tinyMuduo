# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /root/Server/tinyMuduo/examples/parsingServer

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /root/Server/tinyMuduo/examples/parsingServer/build

# Include any dependencies generated for this target.
include src/CMakeFiles/InferenceServer.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include src/CMakeFiles/InferenceServer.dir/compiler_depend.make

# Include the progress variables for this target.
include src/CMakeFiles/InferenceServer.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/InferenceServer.dir/flags.make

src/CMakeFiles/InferenceServer.dir/InferenceServer.cpp.o: src/CMakeFiles/InferenceServer.dir/flags.make
src/CMakeFiles/InferenceServer.dir/InferenceServer.cpp.o: ../src/InferenceServer.cpp
src/CMakeFiles/InferenceServer.dir/InferenceServer.cpp.o: src/CMakeFiles/InferenceServer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/Server/tinyMuduo/examples/parsingServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/CMakeFiles/InferenceServer.dir/InferenceServer.cpp.o"
	cd /root/Server/tinyMuduo/examples/parsingServer/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/InferenceServer.dir/InferenceServer.cpp.o -MF CMakeFiles/InferenceServer.dir/InferenceServer.cpp.o.d -o CMakeFiles/InferenceServer.dir/InferenceServer.cpp.o -c /root/Server/tinyMuduo/examples/parsingServer/src/InferenceServer.cpp

src/CMakeFiles/InferenceServer.dir/InferenceServer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/InferenceServer.dir/InferenceServer.cpp.i"
	cd /root/Server/tinyMuduo/examples/parsingServer/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/Server/tinyMuduo/examples/parsingServer/src/InferenceServer.cpp > CMakeFiles/InferenceServer.dir/InferenceServer.cpp.i

src/CMakeFiles/InferenceServer.dir/InferenceServer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/InferenceServer.dir/InferenceServer.cpp.s"
	cd /root/Server/tinyMuduo/examples/parsingServer/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/Server/tinyMuduo/examples/parsingServer/src/InferenceServer.cpp -o CMakeFiles/InferenceServer.dir/InferenceServer.cpp.s

# Object files for target InferenceServer
InferenceServer_OBJECTS = \
"CMakeFiles/InferenceServer.dir/InferenceServer.cpp.o"

# External object files for target InferenceServer
InferenceServer_EXTERNAL_OBJECTS =

../bin/InferenceServer: src/CMakeFiles/InferenceServer.dir/InferenceServer.cpp.o
../bin/InferenceServer: src/CMakeFiles/InferenceServer.dir/build.make
../bin/InferenceServer: src/CMakeFiles/InferenceServer.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/root/Server/tinyMuduo/examples/parsingServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../../bin/InferenceServer"
	cd /root/Server/tinyMuduo/examples/parsingServer/build/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/InferenceServer.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/InferenceServer.dir/build: ../bin/InferenceServer
.PHONY : src/CMakeFiles/InferenceServer.dir/build

src/CMakeFiles/InferenceServer.dir/clean:
	cd /root/Server/tinyMuduo/examples/parsingServer/build/src && $(CMAKE_COMMAND) -P CMakeFiles/InferenceServer.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/InferenceServer.dir/clean

src/CMakeFiles/InferenceServer.dir/depend:
	cd /root/Server/tinyMuduo/examples/parsingServer/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/Server/tinyMuduo/examples/parsingServer /root/Server/tinyMuduo/examples/parsingServer/src /root/Server/tinyMuduo/examples/parsingServer/build /root/Server/tinyMuduo/examples/parsingServer/build/src /root/Server/tinyMuduo/examples/parsingServer/build/src/CMakeFiles/InferenceServer.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/InferenceServer.dir/depend

