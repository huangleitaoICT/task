# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/hlt/Desktop/task/7Serial

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/hlt/Desktop/task/7Serial/build

# Include any dependencies generated for this target.
include CMakeFiles/chuankou.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/chuankou.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/chuankou.dir/flags.make

CMakeFiles/chuankou.dir/main.cpp.o: CMakeFiles/chuankou.dir/flags.make
CMakeFiles/chuankou.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/hlt/Desktop/task/7Serial/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/chuankou.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/chuankou.dir/main.cpp.o -c /home/hlt/Desktop/task/7Serial/main.cpp

CMakeFiles/chuankou.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/chuankou.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/hlt/Desktop/task/7Serial/main.cpp > CMakeFiles/chuankou.dir/main.cpp.i

CMakeFiles/chuankou.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/chuankou.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/hlt/Desktop/task/7Serial/main.cpp -o CMakeFiles/chuankou.dir/main.cpp.s

CMakeFiles/chuankou.dir/main.cpp.o.requires:

.PHONY : CMakeFiles/chuankou.dir/main.cpp.o.requires

CMakeFiles/chuankou.dir/main.cpp.o.provides: CMakeFiles/chuankou.dir/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/chuankou.dir/build.make CMakeFiles/chuankou.dir/main.cpp.o.provides.build
.PHONY : CMakeFiles/chuankou.dir/main.cpp.o.provides

CMakeFiles/chuankou.dir/main.cpp.o.provides.build: CMakeFiles/chuankou.dir/main.cpp.o


# Object files for target chuankou
chuankou_OBJECTS = \
"CMakeFiles/chuankou.dir/main.cpp.o"

# External object files for target chuankou
chuankou_EXTERNAL_OBJECTS =

chuankou: CMakeFiles/chuankou.dir/main.cpp.o
chuankou: CMakeFiles/chuankou.dir/build.make
chuankou: CMakeFiles/chuankou.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/hlt/Desktop/task/7Serial/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable chuankou"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/chuankou.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/chuankou.dir/build: chuankou

.PHONY : CMakeFiles/chuankou.dir/build

CMakeFiles/chuankou.dir/requires: CMakeFiles/chuankou.dir/main.cpp.o.requires

.PHONY : CMakeFiles/chuankou.dir/requires

CMakeFiles/chuankou.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/chuankou.dir/cmake_clean.cmake
.PHONY : CMakeFiles/chuankou.dir/clean

CMakeFiles/chuankou.dir/depend:
	cd /home/hlt/Desktop/task/7Serial/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/hlt/Desktop/task/7Serial /home/hlt/Desktop/task/7Serial /home/hlt/Desktop/task/7Serial/build /home/hlt/Desktop/task/7Serial/build /home/hlt/Desktop/task/7Serial/build/CMakeFiles/chuankou.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/chuankou.dir/depend
