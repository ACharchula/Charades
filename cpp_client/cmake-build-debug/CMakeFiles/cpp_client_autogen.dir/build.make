# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.12

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
CMAKE_COMMAND = /home/adam/clion-2018.2.6/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/adam/clion-2018.2.6/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/adam/dev/tin/Charades/cpp_client

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/adam/dev/tin/Charades/cpp_client/cmake-build-debug

# Utility rule file for cpp_client_autogen.

# Include the progress variables for this target.
include CMakeFiles/cpp_client_autogen.dir/progress.make

CMakeFiles/cpp_client_autogen:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/adam/dev/tin/Charades/cpp_client/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Automatic MOC for target cpp_client"
	/home/adam/clion-2018.2.6/bin/cmake/linux/bin/cmake -E cmake_autogen /home/adam/dev/tin/Charades/cpp_client/cmake-build-debug/CMakeFiles/cpp_client_autogen.dir/AutogenInfo.cmake Debug

cpp_client_autogen: CMakeFiles/cpp_client_autogen
cpp_client_autogen: CMakeFiles/cpp_client_autogen.dir/build.make

.PHONY : cpp_client_autogen

# Rule to build all files generated by this target.
CMakeFiles/cpp_client_autogen.dir/build: cpp_client_autogen

.PHONY : CMakeFiles/cpp_client_autogen.dir/build

CMakeFiles/cpp_client_autogen.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/cpp_client_autogen.dir/cmake_clean.cmake
.PHONY : CMakeFiles/cpp_client_autogen.dir/clean

CMakeFiles/cpp_client_autogen.dir/depend:
	cd /home/adam/dev/tin/Charades/cpp_client/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/adam/dev/tin/Charades/cpp_client /home/adam/dev/tin/Charades/cpp_client /home/adam/dev/tin/Charades/cpp_client/cmake-build-debug /home/adam/dev/tin/Charades/cpp_client/cmake-build-debug /home/adam/dev/tin/Charades/cpp_client/cmake-build-debug/CMakeFiles/cpp_client_autogen.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/cpp_client_autogen.dir/depend

