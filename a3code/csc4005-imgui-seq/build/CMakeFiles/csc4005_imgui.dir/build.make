# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.21

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
CMAKE_COMMAND = /opt/cmake/bin/cmake

# The command to remove a file.
RM = /opt/cmake/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/118010134/files/csc4005-imgui-seq

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/118010134/files/csc4005-imgui-seq/build

# Include any dependencies generated for this target.
include CMakeFiles/csc4005_imgui.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/csc4005_imgui.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/csc4005_imgui.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/csc4005_imgui.dir/flags.make

CMakeFiles/csc4005_imgui.dir/src/graphic.cpp.o: CMakeFiles/csc4005_imgui.dir/flags.make
CMakeFiles/csc4005_imgui.dir/src/graphic.cpp.o: ../src/graphic.cpp
CMakeFiles/csc4005_imgui.dir/src/graphic.cpp.o: CMakeFiles/csc4005_imgui.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/118010134/files/csc4005-imgui-seq/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/csc4005_imgui.dir/src/graphic.cpp.o"
	/usr/local/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/csc4005_imgui.dir/src/graphic.cpp.o -MF CMakeFiles/csc4005_imgui.dir/src/graphic.cpp.o.d -o CMakeFiles/csc4005_imgui.dir/src/graphic.cpp.o -c /home/118010134/files/csc4005-imgui-seq/src/graphic.cpp

CMakeFiles/csc4005_imgui.dir/src/graphic.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/csc4005_imgui.dir/src/graphic.cpp.i"
	/usr/local/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/118010134/files/csc4005-imgui-seq/src/graphic.cpp > CMakeFiles/csc4005_imgui.dir/src/graphic.cpp.i

CMakeFiles/csc4005_imgui.dir/src/graphic.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/csc4005_imgui.dir/src/graphic.cpp.s"
	/usr/local/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/118010134/files/csc4005-imgui-seq/src/graphic.cpp -o CMakeFiles/csc4005_imgui.dir/src/graphic.cpp.s

CMakeFiles/csc4005_imgui.dir/src/main.cpp.o: CMakeFiles/csc4005_imgui.dir/flags.make
CMakeFiles/csc4005_imgui.dir/src/main.cpp.o: ../src/main.cpp
CMakeFiles/csc4005_imgui.dir/src/main.cpp.o: CMakeFiles/csc4005_imgui.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/118010134/files/csc4005-imgui-seq/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/csc4005_imgui.dir/src/main.cpp.o"
	/usr/local/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/csc4005_imgui.dir/src/main.cpp.o -MF CMakeFiles/csc4005_imgui.dir/src/main.cpp.o.d -o CMakeFiles/csc4005_imgui.dir/src/main.cpp.o -c /home/118010134/files/csc4005-imgui-seq/src/main.cpp

CMakeFiles/csc4005_imgui.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/csc4005_imgui.dir/src/main.cpp.i"
	/usr/local/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/118010134/files/csc4005-imgui-seq/src/main.cpp > CMakeFiles/csc4005_imgui.dir/src/main.cpp.i

CMakeFiles/csc4005_imgui.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/csc4005_imgui.dir/src/main.cpp.s"
	/usr/local/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/118010134/files/csc4005-imgui-seq/src/main.cpp -o CMakeFiles/csc4005_imgui.dir/src/main.cpp.s

# Object files for target csc4005_imgui
csc4005_imgui_OBJECTS = \
"CMakeFiles/csc4005_imgui.dir/src/graphic.cpp.o" \
"CMakeFiles/csc4005_imgui.dir/src/main.cpp.o"

# External object files for target csc4005_imgui
csc4005_imgui_EXTERNAL_OBJECTS =

csc4005_imgui: CMakeFiles/csc4005_imgui.dir/src/graphic.cpp.o
csc4005_imgui: CMakeFiles/csc4005_imgui.dir/src/main.cpp.o
csc4005_imgui: CMakeFiles/csc4005_imgui.dir/build.make
csc4005_imgui: libcore.a
csc4005_imgui: /usr/lib64/libfreetype.so
csc4005_imgui: /usr/lib64/libSDL2.so
csc4005_imgui: /usr/lib64/libGLX.so
csc4005_imgui: /usr/lib64/libOpenGL.so
csc4005_imgui: /usr/local/lib/libmpi.so
csc4005_imgui: CMakeFiles/csc4005_imgui.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/118010134/files/csc4005-imgui-seq/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable csc4005_imgui"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/csc4005_imgui.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/csc4005_imgui.dir/build: csc4005_imgui
.PHONY : CMakeFiles/csc4005_imgui.dir/build

CMakeFiles/csc4005_imgui.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/csc4005_imgui.dir/cmake_clean.cmake
.PHONY : CMakeFiles/csc4005_imgui.dir/clean

CMakeFiles/csc4005_imgui.dir/depend:
	cd /home/118010134/files/csc4005-imgui-seq/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/118010134/files/csc4005-imgui-seq /home/118010134/files/csc4005-imgui-seq /home/118010134/files/csc4005-imgui-seq/build /home/118010134/files/csc4005-imgui-seq/build /home/118010134/files/csc4005-imgui-seq/build/CMakeFiles/csc4005_imgui.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/csc4005_imgui.dir/depend

