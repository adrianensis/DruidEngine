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
CMAKE_SOURCE_DIR = /home/adri/Dropbox/Projects/CPP/NewEngine

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/adri/Dropbox/Projects/CPP/NewEngine/build

# Include any dependencies generated for this target.
include CMakeFiles/Scripting.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Scripting.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Scripting.dir/flags.make

CMakeFiles/Scripting.dir/code/Scripting/Script.cpp.o: CMakeFiles/Scripting.dir/flags.make
CMakeFiles/Scripting.dir/code/Scripting/Script.cpp.o: ../code/Scripting/Script.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/adri/Dropbox/Projects/CPP/NewEngine/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Scripting.dir/code/Scripting/Script.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Scripting.dir/code/Scripting/Script.cpp.o -c /home/adri/Dropbox/Projects/CPP/NewEngine/code/Scripting/Script.cpp

CMakeFiles/Scripting.dir/code/Scripting/Script.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Scripting.dir/code/Scripting/Script.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/adri/Dropbox/Projects/CPP/NewEngine/code/Scripting/Script.cpp > CMakeFiles/Scripting.dir/code/Scripting/Script.cpp.i

CMakeFiles/Scripting.dir/code/Scripting/Script.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Scripting.dir/code/Scripting/Script.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/adri/Dropbox/Projects/CPP/NewEngine/code/Scripting/Script.cpp -o CMakeFiles/Scripting.dir/code/Scripting/Script.cpp.s

CMakeFiles/Scripting.dir/code/Scripting/ScriptEngine.cpp.o: CMakeFiles/Scripting.dir/flags.make
CMakeFiles/Scripting.dir/code/Scripting/ScriptEngine.cpp.o: ../code/Scripting/ScriptEngine.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/adri/Dropbox/Projects/CPP/NewEngine/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/Scripting.dir/code/Scripting/ScriptEngine.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Scripting.dir/code/Scripting/ScriptEngine.cpp.o -c /home/adri/Dropbox/Projects/CPP/NewEngine/code/Scripting/ScriptEngine.cpp

CMakeFiles/Scripting.dir/code/Scripting/ScriptEngine.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Scripting.dir/code/Scripting/ScriptEngine.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/adri/Dropbox/Projects/CPP/NewEngine/code/Scripting/ScriptEngine.cpp > CMakeFiles/Scripting.dir/code/Scripting/ScriptEngine.cpp.i

CMakeFiles/Scripting.dir/code/Scripting/ScriptEngine.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Scripting.dir/code/Scripting/ScriptEngine.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/adri/Dropbox/Projects/CPP/NewEngine/code/Scripting/ScriptEngine.cpp -o CMakeFiles/Scripting.dir/code/Scripting/ScriptEngine.cpp.s

Scripting: CMakeFiles/Scripting.dir/code/Scripting/Script.cpp.o
Scripting: CMakeFiles/Scripting.dir/code/Scripting/ScriptEngine.cpp.o
Scripting: CMakeFiles/Scripting.dir/build.make

.PHONY : Scripting

# Rule to build all files generated by this target.
CMakeFiles/Scripting.dir/build: Scripting

.PHONY : CMakeFiles/Scripting.dir/build

CMakeFiles/Scripting.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Scripting.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Scripting.dir/clean

CMakeFiles/Scripting.dir/depend:
	cd /home/adri/Dropbox/Projects/CPP/NewEngine/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/adri/Dropbox/Projects/CPP/NewEngine /home/adri/Dropbox/Projects/CPP/NewEngine /home/adri/Dropbox/Projects/CPP/NewEngine/build /home/adri/Dropbox/Projects/CPP/NewEngine/build /home/adri/Dropbox/Projects/CPP/NewEngine/build/CMakeFiles/Scripting.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Scripting.dir/depend
