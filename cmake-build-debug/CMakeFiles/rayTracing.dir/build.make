# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.17

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

# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "D:\Program Files\JetBrains\CLion 2020.2\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "D:\Program Files\JetBrains\CLion 2020.2\bin\cmake\win\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = D:\homework\rayTracingTheNextWeek\rayTracing

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = D:\homework\rayTracingTheNextWeek\rayTracing\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/rayTracing.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/rayTracing.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/rayTracing.dir/flags.make

CMakeFiles/rayTracing.dir/main.cpp.obj: CMakeFiles/rayTracing.dir/flags.make
CMakeFiles/rayTracing.dir/main.cpp.obj: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\homework\rayTracingTheNextWeek\rayTracing\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/rayTracing.dir/main.cpp.obj"
	D:\mingw64\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\rayTracing.dir\main.cpp.obj -c D:\homework\rayTracingTheNextWeek\rayTracing\main.cpp

CMakeFiles/rayTracing.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/rayTracing.dir/main.cpp.i"
	D:\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\homework\rayTracingTheNextWeek\rayTracing\main.cpp > CMakeFiles\rayTracing.dir\main.cpp.i

CMakeFiles/rayTracing.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/rayTracing.dir/main.cpp.s"
	D:\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\homework\rayTracingTheNextWeek\rayTracing\main.cpp -o CMakeFiles\rayTracing.dir\main.cpp.s

# Object files for target rayTracing
rayTracing_OBJECTS = \
"CMakeFiles/rayTracing.dir/main.cpp.obj"

# External object files for target rayTracing
rayTracing_EXTERNAL_OBJECTS =

rayTracing.exe: CMakeFiles/rayTracing.dir/main.cpp.obj
rayTracing.exe: CMakeFiles/rayTracing.dir/build.make
rayTracing.exe: CMakeFiles/rayTracing.dir/linklibs.rsp
rayTracing.exe: CMakeFiles/rayTracing.dir/objects1.rsp
rayTracing.exe: CMakeFiles/rayTracing.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=D:\homework\rayTracingTheNextWeek\rayTracing\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable rayTracing.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\rayTracing.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/rayTracing.dir/build: rayTracing.exe

.PHONY : CMakeFiles/rayTracing.dir/build

CMakeFiles/rayTracing.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\rayTracing.dir\cmake_clean.cmake
.PHONY : CMakeFiles/rayTracing.dir/clean

CMakeFiles/rayTracing.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" D:\homework\rayTracingTheNextWeek\rayTracing D:\homework\rayTracingTheNextWeek\rayTracing D:\homework\rayTracingTheNextWeek\rayTracing\cmake-build-debug D:\homework\rayTracingTheNextWeek\rayTracing\cmake-build-debug D:\homework\rayTracingTheNextWeek\rayTracing\cmake-build-debug\CMakeFiles\rayTracing.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/rayTracing.dir/depend

