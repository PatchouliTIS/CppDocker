# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_SOURCE_DIR = /mnt/d/CLionWorkspace/C++/CppDocker

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/d/CLionWorkspace/C++/CppDocker/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/CppDocker.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/CppDocker.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/CppDocker.dir/flags.make

CMakeFiles/CppDocker.dir/main.cpp.o: CMakeFiles/CppDocker.dir/flags.make
CMakeFiles/CppDocker.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/d/CLionWorkspace/C++/CppDocker/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/CppDocker.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/CppDocker.dir/main.cpp.o -c /mnt/d/CLionWorkspace/C++/CppDocker/main.cpp

CMakeFiles/CppDocker.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/CppDocker.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/d/CLionWorkspace/C++/CppDocker/main.cpp > CMakeFiles/CppDocker.dir/main.cpp.i

CMakeFiles/CppDocker.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/CppDocker.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/d/CLionWorkspace/C++/CppDocker/main.cpp -o CMakeFiles/CppDocker.dir/main.cpp.s

# Object files for target CppDocker
CppDocker_OBJECTS = \
"CMakeFiles/CppDocker.dir/main.cpp.o"

# External object files for target CppDocker
CppDocker_EXTERNAL_OBJECTS =

CppDocker: CMakeFiles/CppDocker.dir/main.cpp.o
CppDocker: CMakeFiles/CppDocker.dir/build.make
CppDocker: CMakeFiles/CppDocker.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/d/CLionWorkspace/C++/CppDocker/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable CppDocker"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/CppDocker.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/CppDocker.dir/build: CppDocker

.PHONY : CMakeFiles/CppDocker.dir/build

CMakeFiles/CppDocker.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/CppDocker.dir/cmake_clean.cmake
.PHONY : CMakeFiles/CppDocker.dir/clean

CMakeFiles/CppDocker.dir/depend:
	cd /mnt/d/CLionWorkspace/C++/CppDocker/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/d/CLionWorkspace/C++/CppDocker /mnt/d/CLionWorkspace/C++/CppDocker /mnt/d/CLionWorkspace/C++/CppDocker/cmake-build-debug /mnt/d/CLionWorkspace/C++/CppDocker/cmake-build-debug /mnt/d/CLionWorkspace/C++/CppDocker/cmake-build-debug/CMakeFiles/CppDocker.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/CppDocker.dir/depend
