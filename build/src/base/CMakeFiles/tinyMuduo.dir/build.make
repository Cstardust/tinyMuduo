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
CMAKE_SOURCE_DIR = /root/Server/tinyMuduo

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /root/Server/tinyMuduo/build

# Include any dependencies generated for this target.
include src/base/CMakeFiles/tinyMuduo.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include src/base/CMakeFiles/tinyMuduo.dir/compiler_depend.make

# Include the progress variables for this target.
include src/base/CMakeFiles/tinyMuduo.dir/progress.make

# Include the compile flags for this target's objects.
include src/base/CMakeFiles/tinyMuduo.dir/flags.make

src/base/CMakeFiles/tinyMuduo.dir/Acceptor.cpp.o: src/base/CMakeFiles/tinyMuduo.dir/flags.make
src/base/CMakeFiles/tinyMuduo.dir/Acceptor.cpp.o: ../src/base/Acceptor.cpp
src/base/CMakeFiles/tinyMuduo.dir/Acceptor.cpp.o: src/base/CMakeFiles/tinyMuduo.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/Server/tinyMuduo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/base/CMakeFiles/tinyMuduo.dir/Acceptor.cpp.o"
	cd /root/Server/tinyMuduo/build/src/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/base/CMakeFiles/tinyMuduo.dir/Acceptor.cpp.o -MF CMakeFiles/tinyMuduo.dir/Acceptor.cpp.o.d -o CMakeFiles/tinyMuduo.dir/Acceptor.cpp.o -c /root/Server/tinyMuduo/src/base/Acceptor.cpp

src/base/CMakeFiles/tinyMuduo.dir/Acceptor.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tinyMuduo.dir/Acceptor.cpp.i"
	cd /root/Server/tinyMuduo/build/src/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/Server/tinyMuduo/src/base/Acceptor.cpp > CMakeFiles/tinyMuduo.dir/Acceptor.cpp.i

src/base/CMakeFiles/tinyMuduo.dir/Acceptor.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tinyMuduo.dir/Acceptor.cpp.s"
	cd /root/Server/tinyMuduo/build/src/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/Server/tinyMuduo/src/base/Acceptor.cpp -o CMakeFiles/tinyMuduo.dir/Acceptor.cpp.s

src/base/CMakeFiles/tinyMuduo.dir/Buffer.cpp.o: src/base/CMakeFiles/tinyMuduo.dir/flags.make
src/base/CMakeFiles/tinyMuduo.dir/Buffer.cpp.o: ../src/base/Buffer.cpp
src/base/CMakeFiles/tinyMuduo.dir/Buffer.cpp.o: src/base/CMakeFiles/tinyMuduo.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/Server/tinyMuduo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/base/CMakeFiles/tinyMuduo.dir/Buffer.cpp.o"
	cd /root/Server/tinyMuduo/build/src/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/base/CMakeFiles/tinyMuduo.dir/Buffer.cpp.o -MF CMakeFiles/tinyMuduo.dir/Buffer.cpp.o.d -o CMakeFiles/tinyMuduo.dir/Buffer.cpp.o -c /root/Server/tinyMuduo/src/base/Buffer.cpp

src/base/CMakeFiles/tinyMuduo.dir/Buffer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tinyMuduo.dir/Buffer.cpp.i"
	cd /root/Server/tinyMuduo/build/src/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/Server/tinyMuduo/src/base/Buffer.cpp > CMakeFiles/tinyMuduo.dir/Buffer.cpp.i

src/base/CMakeFiles/tinyMuduo.dir/Buffer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tinyMuduo.dir/Buffer.cpp.s"
	cd /root/Server/tinyMuduo/build/src/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/Server/tinyMuduo/src/base/Buffer.cpp -o CMakeFiles/tinyMuduo.dir/Buffer.cpp.s

src/base/CMakeFiles/tinyMuduo.dir/Channel.cpp.o: src/base/CMakeFiles/tinyMuduo.dir/flags.make
src/base/CMakeFiles/tinyMuduo.dir/Channel.cpp.o: ../src/base/Channel.cpp
src/base/CMakeFiles/tinyMuduo.dir/Channel.cpp.o: src/base/CMakeFiles/tinyMuduo.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/Server/tinyMuduo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object src/base/CMakeFiles/tinyMuduo.dir/Channel.cpp.o"
	cd /root/Server/tinyMuduo/build/src/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/base/CMakeFiles/tinyMuduo.dir/Channel.cpp.o -MF CMakeFiles/tinyMuduo.dir/Channel.cpp.o.d -o CMakeFiles/tinyMuduo.dir/Channel.cpp.o -c /root/Server/tinyMuduo/src/base/Channel.cpp

src/base/CMakeFiles/tinyMuduo.dir/Channel.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tinyMuduo.dir/Channel.cpp.i"
	cd /root/Server/tinyMuduo/build/src/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/Server/tinyMuduo/src/base/Channel.cpp > CMakeFiles/tinyMuduo.dir/Channel.cpp.i

src/base/CMakeFiles/tinyMuduo.dir/Channel.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tinyMuduo.dir/Channel.cpp.s"
	cd /root/Server/tinyMuduo/build/src/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/Server/tinyMuduo/src/base/Channel.cpp -o CMakeFiles/tinyMuduo.dir/Channel.cpp.s

src/base/CMakeFiles/tinyMuduo.dir/CurrentThread.cpp.o: src/base/CMakeFiles/tinyMuduo.dir/flags.make
src/base/CMakeFiles/tinyMuduo.dir/CurrentThread.cpp.o: ../src/base/CurrentThread.cpp
src/base/CMakeFiles/tinyMuduo.dir/CurrentThread.cpp.o: src/base/CMakeFiles/tinyMuduo.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/Server/tinyMuduo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object src/base/CMakeFiles/tinyMuduo.dir/CurrentThread.cpp.o"
	cd /root/Server/tinyMuduo/build/src/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/base/CMakeFiles/tinyMuduo.dir/CurrentThread.cpp.o -MF CMakeFiles/tinyMuduo.dir/CurrentThread.cpp.o.d -o CMakeFiles/tinyMuduo.dir/CurrentThread.cpp.o -c /root/Server/tinyMuduo/src/base/CurrentThread.cpp

src/base/CMakeFiles/tinyMuduo.dir/CurrentThread.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tinyMuduo.dir/CurrentThread.cpp.i"
	cd /root/Server/tinyMuduo/build/src/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/Server/tinyMuduo/src/base/CurrentThread.cpp > CMakeFiles/tinyMuduo.dir/CurrentThread.cpp.i

src/base/CMakeFiles/tinyMuduo.dir/CurrentThread.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tinyMuduo.dir/CurrentThread.cpp.s"
	cd /root/Server/tinyMuduo/build/src/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/Server/tinyMuduo/src/base/CurrentThread.cpp -o CMakeFiles/tinyMuduo.dir/CurrentThread.cpp.s

src/base/CMakeFiles/tinyMuduo.dir/EpollPoller.cpp.o: src/base/CMakeFiles/tinyMuduo.dir/flags.make
src/base/CMakeFiles/tinyMuduo.dir/EpollPoller.cpp.o: ../src/base/EpollPoller.cpp
src/base/CMakeFiles/tinyMuduo.dir/EpollPoller.cpp.o: src/base/CMakeFiles/tinyMuduo.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/Server/tinyMuduo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object src/base/CMakeFiles/tinyMuduo.dir/EpollPoller.cpp.o"
	cd /root/Server/tinyMuduo/build/src/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/base/CMakeFiles/tinyMuduo.dir/EpollPoller.cpp.o -MF CMakeFiles/tinyMuduo.dir/EpollPoller.cpp.o.d -o CMakeFiles/tinyMuduo.dir/EpollPoller.cpp.o -c /root/Server/tinyMuduo/src/base/EpollPoller.cpp

src/base/CMakeFiles/tinyMuduo.dir/EpollPoller.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tinyMuduo.dir/EpollPoller.cpp.i"
	cd /root/Server/tinyMuduo/build/src/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/Server/tinyMuduo/src/base/EpollPoller.cpp > CMakeFiles/tinyMuduo.dir/EpollPoller.cpp.i

src/base/CMakeFiles/tinyMuduo.dir/EpollPoller.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tinyMuduo.dir/EpollPoller.cpp.s"
	cd /root/Server/tinyMuduo/build/src/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/Server/tinyMuduo/src/base/EpollPoller.cpp -o CMakeFiles/tinyMuduo.dir/EpollPoller.cpp.s

src/base/CMakeFiles/tinyMuduo.dir/EventLoop.cpp.o: src/base/CMakeFiles/tinyMuduo.dir/flags.make
src/base/CMakeFiles/tinyMuduo.dir/EventLoop.cpp.o: ../src/base/EventLoop.cpp
src/base/CMakeFiles/tinyMuduo.dir/EventLoop.cpp.o: src/base/CMakeFiles/tinyMuduo.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/Server/tinyMuduo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object src/base/CMakeFiles/tinyMuduo.dir/EventLoop.cpp.o"
	cd /root/Server/tinyMuduo/build/src/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/base/CMakeFiles/tinyMuduo.dir/EventLoop.cpp.o -MF CMakeFiles/tinyMuduo.dir/EventLoop.cpp.o.d -o CMakeFiles/tinyMuduo.dir/EventLoop.cpp.o -c /root/Server/tinyMuduo/src/base/EventLoop.cpp

src/base/CMakeFiles/tinyMuduo.dir/EventLoop.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tinyMuduo.dir/EventLoop.cpp.i"
	cd /root/Server/tinyMuduo/build/src/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/Server/tinyMuduo/src/base/EventLoop.cpp > CMakeFiles/tinyMuduo.dir/EventLoop.cpp.i

src/base/CMakeFiles/tinyMuduo.dir/EventLoop.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tinyMuduo.dir/EventLoop.cpp.s"
	cd /root/Server/tinyMuduo/build/src/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/Server/tinyMuduo/src/base/EventLoop.cpp -o CMakeFiles/tinyMuduo.dir/EventLoop.cpp.s

src/base/CMakeFiles/tinyMuduo.dir/EventLoopThread.cpp.o: src/base/CMakeFiles/tinyMuduo.dir/flags.make
src/base/CMakeFiles/tinyMuduo.dir/EventLoopThread.cpp.o: ../src/base/EventLoopThread.cpp
src/base/CMakeFiles/tinyMuduo.dir/EventLoopThread.cpp.o: src/base/CMakeFiles/tinyMuduo.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/Server/tinyMuduo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object src/base/CMakeFiles/tinyMuduo.dir/EventLoopThread.cpp.o"
	cd /root/Server/tinyMuduo/build/src/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/base/CMakeFiles/tinyMuduo.dir/EventLoopThread.cpp.o -MF CMakeFiles/tinyMuduo.dir/EventLoopThread.cpp.o.d -o CMakeFiles/tinyMuduo.dir/EventLoopThread.cpp.o -c /root/Server/tinyMuduo/src/base/EventLoopThread.cpp

src/base/CMakeFiles/tinyMuduo.dir/EventLoopThread.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tinyMuduo.dir/EventLoopThread.cpp.i"
	cd /root/Server/tinyMuduo/build/src/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/Server/tinyMuduo/src/base/EventLoopThread.cpp > CMakeFiles/tinyMuduo.dir/EventLoopThread.cpp.i

src/base/CMakeFiles/tinyMuduo.dir/EventLoopThread.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tinyMuduo.dir/EventLoopThread.cpp.s"
	cd /root/Server/tinyMuduo/build/src/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/Server/tinyMuduo/src/base/EventLoopThread.cpp -o CMakeFiles/tinyMuduo.dir/EventLoopThread.cpp.s

src/base/CMakeFiles/tinyMuduo.dir/EventLoopThreadPool.cpp.o: src/base/CMakeFiles/tinyMuduo.dir/flags.make
src/base/CMakeFiles/tinyMuduo.dir/EventLoopThreadPool.cpp.o: ../src/base/EventLoopThreadPool.cpp
src/base/CMakeFiles/tinyMuduo.dir/EventLoopThreadPool.cpp.o: src/base/CMakeFiles/tinyMuduo.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/Server/tinyMuduo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object src/base/CMakeFiles/tinyMuduo.dir/EventLoopThreadPool.cpp.o"
	cd /root/Server/tinyMuduo/build/src/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/base/CMakeFiles/tinyMuduo.dir/EventLoopThreadPool.cpp.o -MF CMakeFiles/tinyMuduo.dir/EventLoopThreadPool.cpp.o.d -o CMakeFiles/tinyMuduo.dir/EventLoopThreadPool.cpp.o -c /root/Server/tinyMuduo/src/base/EventLoopThreadPool.cpp

src/base/CMakeFiles/tinyMuduo.dir/EventLoopThreadPool.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tinyMuduo.dir/EventLoopThreadPool.cpp.i"
	cd /root/Server/tinyMuduo/build/src/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/Server/tinyMuduo/src/base/EventLoopThreadPool.cpp > CMakeFiles/tinyMuduo.dir/EventLoopThreadPool.cpp.i

src/base/CMakeFiles/tinyMuduo.dir/EventLoopThreadPool.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tinyMuduo.dir/EventLoopThreadPool.cpp.s"
	cd /root/Server/tinyMuduo/build/src/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/Server/tinyMuduo/src/base/EventLoopThreadPool.cpp -o CMakeFiles/tinyMuduo.dir/EventLoopThreadPool.cpp.s

src/base/CMakeFiles/tinyMuduo.dir/InetAddress.cpp.o: src/base/CMakeFiles/tinyMuduo.dir/flags.make
src/base/CMakeFiles/tinyMuduo.dir/InetAddress.cpp.o: ../src/base/InetAddress.cpp
src/base/CMakeFiles/tinyMuduo.dir/InetAddress.cpp.o: src/base/CMakeFiles/tinyMuduo.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/Server/tinyMuduo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object src/base/CMakeFiles/tinyMuduo.dir/InetAddress.cpp.o"
	cd /root/Server/tinyMuduo/build/src/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/base/CMakeFiles/tinyMuduo.dir/InetAddress.cpp.o -MF CMakeFiles/tinyMuduo.dir/InetAddress.cpp.o.d -o CMakeFiles/tinyMuduo.dir/InetAddress.cpp.o -c /root/Server/tinyMuduo/src/base/InetAddress.cpp

src/base/CMakeFiles/tinyMuduo.dir/InetAddress.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tinyMuduo.dir/InetAddress.cpp.i"
	cd /root/Server/tinyMuduo/build/src/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/Server/tinyMuduo/src/base/InetAddress.cpp > CMakeFiles/tinyMuduo.dir/InetAddress.cpp.i

src/base/CMakeFiles/tinyMuduo.dir/InetAddress.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tinyMuduo.dir/InetAddress.cpp.s"
	cd /root/Server/tinyMuduo/build/src/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/Server/tinyMuduo/src/base/InetAddress.cpp -o CMakeFiles/tinyMuduo.dir/InetAddress.cpp.s

src/base/CMakeFiles/tinyMuduo.dir/Logger.cpp.o: src/base/CMakeFiles/tinyMuduo.dir/flags.make
src/base/CMakeFiles/tinyMuduo.dir/Logger.cpp.o: ../src/base/Logger.cpp
src/base/CMakeFiles/tinyMuduo.dir/Logger.cpp.o: src/base/CMakeFiles/tinyMuduo.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/Server/tinyMuduo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object src/base/CMakeFiles/tinyMuduo.dir/Logger.cpp.o"
	cd /root/Server/tinyMuduo/build/src/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/base/CMakeFiles/tinyMuduo.dir/Logger.cpp.o -MF CMakeFiles/tinyMuduo.dir/Logger.cpp.o.d -o CMakeFiles/tinyMuduo.dir/Logger.cpp.o -c /root/Server/tinyMuduo/src/base/Logger.cpp

src/base/CMakeFiles/tinyMuduo.dir/Logger.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tinyMuduo.dir/Logger.cpp.i"
	cd /root/Server/tinyMuduo/build/src/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/Server/tinyMuduo/src/base/Logger.cpp > CMakeFiles/tinyMuduo.dir/Logger.cpp.i

src/base/CMakeFiles/tinyMuduo.dir/Logger.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tinyMuduo.dir/Logger.cpp.s"
	cd /root/Server/tinyMuduo/build/src/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/Server/tinyMuduo/src/base/Logger.cpp -o CMakeFiles/tinyMuduo.dir/Logger.cpp.s

src/base/CMakeFiles/tinyMuduo.dir/Poller.cpp.o: src/base/CMakeFiles/tinyMuduo.dir/flags.make
src/base/CMakeFiles/tinyMuduo.dir/Poller.cpp.o: ../src/base/Poller.cpp
src/base/CMakeFiles/tinyMuduo.dir/Poller.cpp.o: src/base/CMakeFiles/tinyMuduo.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/Server/tinyMuduo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object src/base/CMakeFiles/tinyMuduo.dir/Poller.cpp.o"
	cd /root/Server/tinyMuduo/build/src/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/base/CMakeFiles/tinyMuduo.dir/Poller.cpp.o -MF CMakeFiles/tinyMuduo.dir/Poller.cpp.o.d -o CMakeFiles/tinyMuduo.dir/Poller.cpp.o -c /root/Server/tinyMuduo/src/base/Poller.cpp

src/base/CMakeFiles/tinyMuduo.dir/Poller.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tinyMuduo.dir/Poller.cpp.i"
	cd /root/Server/tinyMuduo/build/src/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/Server/tinyMuduo/src/base/Poller.cpp > CMakeFiles/tinyMuduo.dir/Poller.cpp.i

src/base/CMakeFiles/tinyMuduo.dir/Poller.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tinyMuduo.dir/Poller.cpp.s"
	cd /root/Server/tinyMuduo/build/src/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/Server/tinyMuduo/src/base/Poller.cpp -o CMakeFiles/tinyMuduo.dir/Poller.cpp.s

src/base/CMakeFiles/tinyMuduo.dir/Socket.cpp.o: src/base/CMakeFiles/tinyMuduo.dir/flags.make
src/base/CMakeFiles/tinyMuduo.dir/Socket.cpp.o: ../src/base/Socket.cpp
src/base/CMakeFiles/tinyMuduo.dir/Socket.cpp.o: src/base/CMakeFiles/tinyMuduo.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/Server/tinyMuduo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building CXX object src/base/CMakeFiles/tinyMuduo.dir/Socket.cpp.o"
	cd /root/Server/tinyMuduo/build/src/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/base/CMakeFiles/tinyMuduo.dir/Socket.cpp.o -MF CMakeFiles/tinyMuduo.dir/Socket.cpp.o.d -o CMakeFiles/tinyMuduo.dir/Socket.cpp.o -c /root/Server/tinyMuduo/src/base/Socket.cpp

src/base/CMakeFiles/tinyMuduo.dir/Socket.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tinyMuduo.dir/Socket.cpp.i"
	cd /root/Server/tinyMuduo/build/src/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/Server/tinyMuduo/src/base/Socket.cpp > CMakeFiles/tinyMuduo.dir/Socket.cpp.i

src/base/CMakeFiles/tinyMuduo.dir/Socket.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tinyMuduo.dir/Socket.cpp.s"
	cd /root/Server/tinyMuduo/build/src/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/Server/tinyMuduo/src/base/Socket.cpp -o CMakeFiles/tinyMuduo.dir/Socket.cpp.s

src/base/CMakeFiles/tinyMuduo.dir/TcpConnection.cpp.o: src/base/CMakeFiles/tinyMuduo.dir/flags.make
src/base/CMakeFiles/tinyMuduo.dir/TcpConnection.cpp.o: ../src/base/TcpConnection.cpp
src/base/CMakeFiles/tinyMuduo.dir/TcpConnection.cpp.o: src/base/CMakeFiles/tinyMuduo.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/Server/tinyMuduo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Building CXX object src/base/CMakeFiles/tinyMuduo.dir/TcpConnection.cpp.o"
	cd /root/Server/tinyMuduo/build/src/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/base/CMakeFiles/tinyMuduo.dir/TcpConnection.cpp.o -MF CMakeFiles/tinyMuduo.dir/TcpConnection.cpp.o.d -o CMakeFiles/tinyMuduo.dir/TcpConnection.cpp.o -c /root/Server/tinyMuduo/src/base/TcpConnection.cpp

src/base/CMakeFiles/tinyMuduo.dir/TcpConnection.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tinyMuduo.dir/TcpConnection.cpp.i"
	cd /root/Server/tinyMuduo/build/src/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/Server/tinyMuduo/src/base/TcpConnection.cpp > CMakeFiles/tinyMuduo.dir/TcpConnection.cpp.i

src/base/CMakeFiles/tinyMuduo.dir/TcpConnection.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tinyMuduo.dir/TcpConnection.cpp.s"
	cd /root/Server/tinyMuduo/build/src/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/Server/tinyMuduo/src/base/TcpConnection.cpp -o CMakeFiles/tinyMuduo.dir/TcpConnection.cpp.s

src/base/CMakeFiles/tinyMuduo.dir/TcpServer.cpp.o: src/base/CMakeFiles/tinyMuduo.dir/flags.make
src/base/CMakeFiles/tinyMuduo.dir/TcpServer.cpp.o: ../src/base/TcpServer.cpp
src/base/CMakeFiles/tinyMuduo.dir/TcpServer.cpp.o: src/base/CMakeFiles/tinyMuduo.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/Server/tinyMuduo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_14) "Building CXX object src/base/CMakeFiles/tinyMuduo.dir/TcpServer.cpp.o"
	cd /root/Server/tinyMuduo/build/src/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/base/CMakeFiles/tinyMuduo.dir/TcpServer.cpp.o -MF CMakeFiles/tinyMuduo.dir/TcpServer.cpp.o.d -o CMakeFiles/tinyMuduo.dir/TcpServer.cpp.o -c /root/Server/tinyMuduo/src/base/TcpServer.cpp

src/base/CMakeFiles/tinyMuduo.dir/TcpServer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tinyMuduo.dir/TcpServer.cpp.i"
	cd /root/Server/tinyMuduo/build/src/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/Server/tinyMuduo/src/base/TcpServer.cpp > CMakeFiles/tinyMuduo.dir/TcpServer.cpp.i

src/base/CMakeFiles/tinyMuduo.dir/TcpServer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tinyMuduo.dir/TcpServer.cpp.s"
	cd /root/Server/tinyMuduo/build/src/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/Server/tinyMuduo/src/base/TcpServer.cpp -o CMakeFiles/tinyMuduo.dir/TcpServer.cpp.s

src/base/CMakeFiles/tinyMuduo.dir/Thread.cpp.o: src/base/CMakeFiles/tinyMuduo.dir/flags.make
src/base/CMakeFiles/tinyMuduo.dir/Thread.cpp.o: ../src/base/Thread.cpp
src/base/CMakeFiles/tinyMuduo.dir/Thread.cpp.o: src/base/CMakeFiles/tinyMuduo.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/Server/tinyMuduo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_15) "Building CXX object src/base/CMakeFiles/tinyMuduo.dir/Thread.cpp.o"
	cd /root/Server/tinyMuduo/build/src/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/base/CMakeFiles/tinyMuduo.dir/Thread.cpp.o -MF CMakeFiles/tinyMuduo.dir/Thread.cpp.o.d -o CMakeFiles/tinyMuduo.dir/Thread.cpp.o -c /root/Server/tinyMuduo/src/base/Thread.cpp

src/base/CMakeFiles/tinyMuduo.dir/Thread.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tinyMuduo.dir/Thread.cpp.i"
	cd /root/Server/tinyMuduo/build/src/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/Server/tinyMuduo/src/base/Thread.cpp > CMakeFiles/tinyMuduo.dir/Thread.cpp.i

src/base/CMakeFiles/tinyMuduo.dir/Thread.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tinyMuduo.dir/Thread.cpp.s"
	cd /root/Server/tinyMuduo/build/src/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/Server/tinyMuduo/src/base/Thread.cpp -o CMakeFiles/tinyMuduo.dir/Thread.cpp.s

src/base/CMakeFiles/tinyMuduo.dir/Timestamp.cpp.o: src/base/CMakeFiles/tinyMuduo.dir/flags.make
src/base/CMakeFiles/tinyMuduo.dir/Timestamp.cpp.o: ../src/base/Timestamp.cpp
src/base/CMakeFiles/tinyMuduo.dir/Timestamp.cpp.o: src/base/CMakeFiles/tinyMuduo.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/Server/tinyMuduo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_16) "Building CXX object src/base/CMakeFiles/tinyMuduo.dir/Timestamp.cpp.o"
	cd /root/Server/tinyMuduo/build/src/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/base/CMakeFiles/tinyMuduo.dir/Timestamp.cpp.o -MF CMakeFiles/tinyMuduo.dir/Timestamp.cpp.o.d -o CMakeFiles/tinyMuduo.dir/Timestamp.cpp.o -c /root/Server/tinyMuduo/src/base/Timestamp.cpp

src/base/CMakeFiles/tinyMuduo.dir/Timestamp.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tinyMuduo.dir/Timestamp.cpp.i"
	cd /root/Server/tinyMuduo/build/src/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/Server/tinyMuduo/src/base/Timestamp.cpp > CMakeFiles/tinyMuduo.dir/Timestamp.cpp.i

src/base/CMakeFiles/tinyMuduo.dir/Timestamp.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tinyMuduo.dir/Timestamp.cpp.s"
	cd /root/Server/tinyMuduo/build/src/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/Server/tinyMuduo/src/base/Timestamp.cpp -o CMakeFiles/tinyMuduo.dir/Timestamp.cpp.s

src/base/CMakeFiles/tinyMuduo.dir/main.cpp.o: src/base/CMakeFiles/tinyMuduo.dir/flags.make
src/base/CMakeFiles/tinyMuduo.dir/main.cpp.o: ../src/base/main.cpp
src/base/CMakeFiles/tinyMuduo.dir/main.cpp.o: src/base/CMakeFiles/tinyMuduo.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/Server/tinyMuduo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_17) "Building CXX object src/base/CMakeFiles/tinyMuduo.dir/main.cpp.o"
	cd /root/Server/tinyMuduo/build/src/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/base/CMakeFiles/tinyMuduo.dir/main.cpp.o -MF CMakeFiles/tinyMuduo.dir/main.cpp.o.d -o CMakeFiles/tinyMuduo.dir/main.cpp.o -c /root/Server/tinyMuduo/src/base/main.cpp

src/base/CMakeFiles/tinyMuduo.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tinyMuduo.dir/main.cpp.i"
	cd /root/Server/tinyMuduo/build/src/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/Server/tinyMuduo/src/base/main.cpp > CMakeFiles/tinyMuduo.dir/main.cpp.i

src/base/CMakeFiles/tinyMuduo.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tinyMuduo.dir/main.cpp.s"
	cd /root/Server/tinyMuduo/build/src/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/Server/tinyMuduo/src/base/main.cpp -o CMakeFiles/tinyMuduo.dir/main.cpp.s

src/base/CMakeFiles/tinyMuduo.dir/newDefaultPoller.cpp.o: src/base/CMakeFiles/tinyMuduo.dir/flags.make
src/base/CMakeFiles/tinyMuduo.dir/newDefaultPoller.cpp.o: ../src/base/newDefaultPoller.cpp
src/base/CMakeFiles/tinyMuduo.dir/newDefaultPoller.cpp.o: src/base/CMakeFiles/tinyMuduo.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/Server/tinyMuduo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_18) "Building CXX object src/base/CMakeFiles/tinyMuduo.dir/newDefaultPoller.cpp.o"
	cd /root/Server/tinyMuduo/build/src/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/base/CMakeFiles/tinyMuduo.dir/newDefaultPoller.cpp.o -MF CMakeFiles/tinyMuduo.dir/newDefaultPoller.cpp.o.d -o CMakeFiles/tinyMuduo.dir/newDefaultPoller.cpp.o -c /root/Server/tinyMuduo/src/base/newDefaultPoller.cpp

src/base/CMakeFiles/tinyMuduo.dir/newDefaultPoller.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tinyMuduo.dir/newDefaultPoller.cpp.i"
	cd /root/Server/tinyMuduo/build/src/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/Server/tinyMuduo/src/base/newDefaultPoller.cpp > CMakeFiles/tinyMuduo.dir/newDefaultPoller.cpp.i

src/base/CMakeFiles/tinyMuduo.dir/newDefaultPoller.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tinyMuduo.dir/newDefaultPoller.cpp.s"
	cd /root/Server/tinyMuduo/build/src/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/Server/tinyMuduo/src/base/newDefaultPoller.cpp -o CMakeFiles/tinyMuduo.dir/newDefaultPoller.cpp.s

src/base/CMakeFiles/tinyMuduo.dir/__/http/HttpServer.cpp.o: src/base/CMakeFiles/tinyMuduo.dir/flags.make
src/base/CMakeFiles/tinyMuduo.dir/__/http/HttpServer.cpp.o: ../src/http/HttpServer.cpp
src/base/CMakeFiles/tinyMuduo.dir/__/http/HttpServer.cpp.o: src/base/CMakeFiles/tinyMuduo.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/Server/tinyMuduo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_19) "Building CXX object src/base/CMakeFiles/tinyMuduo.dir/__/http/HttpServer.cpp.o"
	cd /root/Server/tinyMuduo/build/src/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/base/CMakeFiles/tinyMuduo.dir/__/http/HttpServer.cpp.o -MF CMakeFiles/tinyMuduo.dir/__/http/HttpServer.cpp.o.d -o CMakeFiles/tinyMuduo.dir/__/http/HttpServer.cpp.o -c /root/Server/tinyMuduo/src/http/HttpServer.cpp

src/base/CMakeFiles/tinyMuduo.dir/__/http/HttpServer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tinyMuduo.dir/__/http/HttpServer.cpp.i"
	cd /root/Server/tinyMuduo/build/src/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/Server/tinyMuduo/src/http/HttpServer.cpp > CMakeFiles/tinyMuduo.dir/__/http/HttpServer.cpp.i

src/base/CMakeFiles/tinyMuduo.dir/__/http/HttpServer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tinyMuduo.dir/__/http/HttpServer.cpp.s"
	cd /root/Server/tinyMuduo/build/src/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/Server/tinyMuduo/src/http/HttpServer.cpp -o CMakeFiles/tinyMuduo.dir/__/http/HttpServer.cpp.s

# Object files for target tinyMuduo
tinyMuduo_OBJECTS = \
"CMakeFiles/tinyMuduo.dir/Acceptor.cpp.o" \
"CMakeFiles/tinyMuduo.dir/Buffer.cpp.o" \
"CMakeFiles/tinyMuduo.dir/Channel.cpp.o" \
"CMakeFiles/tinyMuduo.dir/CurrentThread.cpp.o" \
"CMakeFiles/tinyMuduo.dir/EpollPoller.cpp.o" \
"CMakeFiles/tinyMuduo.dir/EventLoop.cpp.o" \
"CMakeFiles/tinyMuduo.dir/EventLoopThread.cpp.o" \
"CMakeFiles/tinyMuduo.dir/EventLoopThreadPool.cpp.o" \
"CMakeFiles/tinyMuduo.dir/InetAddress.cpp.o" \
"CMakeFiles/tinyMuduo.dir/Logger.cpp.o" \
"CMakeFiles/tinyMuduo.dir/Poller.cpp.o" \
"CMakeFiles/tinyMuduo.dir/Socket.cpp.o" \
"CMakeFiles/tinyMuduo.dir/TcpConnection.cpp.o" \
"CMakeFiles/tinyMuduo.dir/TcpServer.cpp.o" \
"CMakeFiles/tinyMuduo.dir/Thread.cpp.o" \
"CMakeFiles/tinyMuduo.dir/Timestamp.cpp.o" \
"CMakeFiles/tinyMuduo.dir/main.cpp.o" \
"CMakeFiles/tinyMuduo.dir/newDefaultPoller.cpp.o" \
"CMakeFiles/tinyMuduo.dir/__/http/HttpServer.cpp.o"

# External object files for target tinyMuduo
tinyMuduo_EXTERNAL_OBJECTS =

../lib/libtinyMuduo.so: src/base/CMakeFiles/tinyMuduo.dir/Acceptor.cpp.o
../lib/libtinyMuduo.so: src/base/CMakeFiles/tinyMuduo.dir/Buffer.cpp.o
../lib/libtinyMuduo.so: src/base/CMakeFiles/tinyMuduo.dir/Channel.cpp.o
../lib/libtinyMuduo.so: src/base/CMakeFiles/tinyMuduo.dir/CurrentThread.cpp.o
../lib/libtinyMuduo.so: src/base/CMakeFiles/tinyMuduo.dir/EpollPoller.cpp.o
../lib/libtinyMuduo.so: src/base/CMakeFiles/tinyMuduo.dir/EventLoop.cpp.o
../lib/libtinyMuduo.so: src/base/CMakeFiles/tinyMuduo.dir/EventLoopThread.cpp.o
../lib/libtinyMuduo.so: src/base/CMakeFiles/tinyMuduo.dir/EventLoopThreadPool.cpp.o
../lib/libtinyMuduo.so: src/base/CMakeFiles/tinyMuduo.dir/InetAddress.cpp.o
../lib/libtinyMuduo.so: src/base/CMakeFiles/tinyMuduo.dir/Logger.cpp.o
../lib/libtinyMuduo.so: src/base/CMakeFiles/tinyMuduo.dir/Poller.cpp.o
../lib/libtinyMuduo.so: src/base/CMakeFiles/tinyMuduo.dir/Socket.cpp.o
../lib/libtinyMuduo.so: src/base/CMakeFiles/tinyMuduo.dir/TcpConnection.cpp.o
../lib/libtinyMuduo.so: src/base/CMakeFiles/tinyMuduo.dir/TcpServer.cpp.o
../lib/libtinyMuduo.so: src/base/CMakeFiles/tinyMuduo.dir/Thread.cpp.o
../lib/libtinyMuduo.so: src/base/CMakeFiles/tinyMuduo.dir/Timestamp.cpp.o
../lib/libtinyMuduo.so: src/base/CMakeFiles/tinyMuduo.dir/main.cpp.o
../lib/libtinyMuduo.so: src/base/CMakeFiles/tinyMuduo.dir/newDefaultPoller.cpp.o
../lib/libtinyMuduo.so: src/base/CMakeFiles/tinyMuduo.dir/__/http/HttpServer.cpp.o
../lib/libtinyMuduo.so: src/base/CMakeFiles/tinyMuduo.dir/build.make
../lib/libtinyMuduo.so: src/base/CMakeFiles/tinyMuduo.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/root/Server/tinyMuduo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_20) "Linking CXX shared library ../../../lib/libtinyMuduo.so"
	cd /root/Server/tinyMuduo/build/src/base && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/tinyMuduo.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/base/CMakeFiles/tinyMuduo.dir/build: ../lib/libtinyMuduo.so
.PHONY : src/base/CMakeFiles/tinyMuduo.dir/build

src/base/CMakeFiles/tinyMuduo.dir/clean:
	cd /root/Server/tinyMuduo/build/src/base && $(CMAKE_COMMAND) -P CMakeFiles/tinyMuduo.dir/cmake_clean.cmake
.PHONY : src/base/CMakeFiles/tinyMuduo.dir/clean

src/base/CMakeFiles/tinyMuduo.dir/depend:
	cd /root/Server/tinyMuduo/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/Server/tinyMuduo /root/Server/tinyMuduo/src/base /root/Server/tinyMuduo/build /root/Server/tinyMuduo/build/src/base /root/Server/tinyMuduo/build/src/base/CMakeFiles/tinyMuduo.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/base/CMakeFiles/tinyMuduo.dir/depend

