# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.8

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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/hjf/Desktop/Motif/motif-src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/hjf/Desktop/Motif/motif-src/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/motif_src.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/motif_src.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/motif_src.dir/flags.make

CMakeFiles/motif_src.dir/Graph.cpp.o: CMakeFiles/motif_src.dir/flags.make
CMakeFiles/motif_src.dir/Graph.cpp.o: ../Graph.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/hjf/Desktop/Motif/motif-src/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/motif_src.dir/Graph.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/motif_src.dir/Graph.cpp.o -c /Users/hjf/Desktop/Motif/motif-src/Graph.cpp

CMakeFiles/motif_src.dir/Graph.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/motif_src.dir/Graph.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/hjf/Desktop/Motif/motif-src/Graph.cpp > CMakeFiles/motif_src.dir/Graph.cpp.i

CMakeFiles/motif_src.dir/Graph.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/motif_src.dir/Graph.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/hjf/Desktop/Motif/motif-src/Graph.cpp -o CMakeFiles/motif_src.dir/Graph.cpp.s

CMakeFiles/motif_src.dir/Graph.cpp.o.requires:

.PHONY : CMakeFiles/motif_src.dir/Graph.cpp.o.requires

CMakeFiles/motif_src.dir/Graph.cpp.o.provides: CMakeFiles/motif_src.dir/Graph.cpp.o.requires
	$(MAKE) -f CMakeFiles/motif_src.dir/build.make CMakeFiles/motif_src.dir/Graph.cpp.o.provides.build
.PHONY : CMakeFiles/motif_src.dir/Graph.cpp.o.provides

CMakeFiles/motif_src.dir/Graph.cpp.o.provides.build: CMakeFiles/motif_src.dir/Graph.cpp.o


CMakeFiles/motif_src.dir/main.cpp.o: CMakeFiles/motif_src.dir/flags.make
CMakeFiles/motif_src.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/hjf/Desktop/Motif/motif-src/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/motif_src.dir/main.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/motif_src.dir/main.cpp.o -c /Users/hjf/Desktop/Motif/motif-src/main.cpp

CMakeFiles/motif_src.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/motif_src.dir/main.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/hjf/Desktop/Motif/motif-src/main.cpp > CMakeFiles/motif_src.dir/main.cpp.i

CMakeFiles/motif_src.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/motif_src.dir/main.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/hjf/Desktop/Motif/motif-src/main.cpp -o CMakeFiles/motif_src.dir/main.cpp.s

CMakeFiles/motif_src.dir/main.cpp.o.requires:

.PHONY : CMakeFiles/motif_src.dir/main.cpp.o.requires

CMakeFiles/motif_src.dir/main.cpp.o.provides: CMakeFiles/motif_src.dir/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/motif_src.dir/build.make CMakeFiles/motif_src.dir/main.cpp.o.provides.build
.PHONY : CMakeFiles/motif_src.dir/main.cpp.o.provides

CMakeFiles/motif_src.dir/main.cpp.o.provides.build: CMakeFiles/motif_src.dir/main.cpp.o


CMakeFiles/motif_src.dir/SetTrieNode.cpp.o: CMakeFiles/motif_src.dir/flags.make
CMakeFiles/motif_src.dir/SetTrieNode.cpp.o: ../SetTrieNode.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/hjf/Desktop/Motif/motif-src/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/motif_src.dir/SetTrieNode.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/motif_src.dir/SetTrieNode.cpp.o -c /Users/hjf/Desktop/Motif/motif-src/SetTrieNode.cpp

CMakeFiles/motif_src.dir/SetTrieNode.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/motif_src.dir/SetTrieNode.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/hjf/Desktop/Motif/motif-src/SetTrieNode.cpp > CMakeFiles/motif_src.dir/SetTrieNode.cpp.i

CMakeFiles/motif_src.dir/SetTrieNode.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/motif_src.dir/SetTrieNode.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/hjf/Desktop/Motif/motif-src/SetTrieNode.cpp -o CMakeFiles/motif_src.dir/SetTrieNode.cpp.s

CMakeFiles/motif_src.dir/SetTrieNode.cpp.o.requires:

.PHONY : CMakeFiles/motif_src.dir/SetTrieNode.cpp.o.requires

CMakeFiles/motif_src.dir/SetTrieNode.cpp.o.provides: CMakeFiles/motif_src.dir/SetTrieNode.cpp.o.requires
	$(MAKE) -f CMakeFiles/motif_src.dir/build.make CMakeFiles/motif_src.dir/SetTrieNode.cpp.o.provides.build
.PHONY : CMakeFiles/motif_src.dir/SetTrieNode.cpp.o.provides

CMakeFiles/motif_src.dir/SetTrieNode.cpp.o.provides.build: CMakeFiles/motif_src.dir/SetTrieNode.cpp.o


CMakeFiles/motif_src.dir/subgraph_match.cpp.o: CMakeFiles/motif_src.dir/flags.make
CMakeFiles/motif_src.dir/subgraph_match.cpp.o: ../subgraph_match.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/hjf/Desktop/Motif/motif-src/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/motif_src.dir/subgraph_match.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/motif_src.dir/subgraph_match.cpp.o -c /Users/hjf/Desktop/Motif/motif-src/subgraph_match.cpp

CMakeFiles/motif_src.dir/subgraph_match.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/motif_src.dir/subgraph_match.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/hjf/Desktop/Motif/motif-src/subgraph_match.cpp > CMakeFiles/motif_src.dir/subgraph_match.cpp.i

CMakeFiles/motif_src.dir/subgraph_match.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/motif_src.dir/subgraph_match.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/hjf/Desktop/Motif/motif-src/subgraph_match.cpp -o CMakeFiles/motif_src.dir/subgraph_match.cpp.s

CMakeFiles/motif_src.dir/subgraph_match.cpp.o.requires:

.PHONY : CMakeFiles/motif_src.dir/subgraph_match.cpp.o.requires

CMakeFiles/motif_src.dir/subgraph_match.cpp.o.provides: CMakeFiles/motif_src.dir/subgraph_match.cpp.o.requires
	$(MAKE) -f CMakeFiles/motif_src.dir/build.make CMakeFiles/motif_src.dir/subgraph_match.cpp.o.provides.build
.PHONY : CMakeFiles/motif_src.dir/subgraph_match.cpp.o.provides

CMakeFiles/motif_src.dir/subgraph_match.cpp.o.provides.build: CMakeFiles/motif_src.dir/subgraph_match.cpp.o


CMakeFiles/motif_src.dir/global-variable.cpp.o: CMakeFiles/motif_src.dir/flags.make
CMakeFiles/motif_src.dir/global-variable.cpp.o: ../global-variable.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/hjf/Desktop/Motif/motif-src/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/motif_src.dir/global-variable.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/motif_src.dir/global-variable.cpp.o -c /Users/hjf/Desktop/Motif/motif-src/global-variable.cpp

CMakeFiles/motif_src.dir/global-variable.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/motif_src.dir/global-variable.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/hjf/Desktop/Motif/motif-src/global-variable.cpp > CMakeFiles/motif_src.dir/global-variable.cpp.i

CMakeFiles/motif_src.dir/global-variable.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/motif_src.dir/global-variable.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/hjf/Desktop/Motif/motif-src/global-variable.cpp -o CMakeFiles/motif_src.dir/global-variable.cpp.s

CMakeFiles/motif_src.dir/global-variable.cpp.o.requires:

.PHONY : CMakeFiles/motif_src.dir/global-variable.cpp.o.requires

CMakeFiles/motif_src.dir/global-variable.cpp.o.provides: CMakeFiles/motif_src.dir/global-variable.cpp.o.requires
	$(MAKE) -f CMakeFiles/motif_src.dir/build.make CMakeFiles/motif_src.dir/global-variable.cpp.o.provides.build
.PHONY : CMakeFiles/motif_src.dir/global-variable.cpp.o.provides

CMakeFiles/motif_src.dir/global-variable.cpp.o.provides.build: CMakeFiles/motif_src.dir/global-variable.cpp.o


CMakeFiles/motif_src.dir/maximal-motif-clique.cpp.o: CMakeFiles/motif_src.dir/flags.make
CMakeFiles/motif_src.dir/maximal-motif-clique.cpp.o: ../maximal-motif-clique.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/hjf/Desktop/Motif/motif-src/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/motif_src.dir/maximal-motif-clique.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/motif_src.dir/maximal-motif-clique.cpp.o -c /Users/hjf/Desktop/Motif/motif-src/maximal-motif-clique.cpp

CMakeFiles/motif_src.dir/maximal-motif-clique.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/motif_src.dir/maximal-motif-clique.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/hjf/Desktop/Motif/motif-src/maximal-motif-clique.cpp > CMakeFiles/motif_src.dir/maximal-motif-clique.cpp.i

CMakeFiles/motif_src.dir/maximal-motif-clique.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/motif_src.dir/maximal-motif-clique.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/hjf/Desktop/Motif/motif-src/maximal-motif-clique.cpp -o CMakeFiles/motif_src.dir/maximal-motif-clique.cpp.s

CMakeFiles/motif_src.dir/maximal-motif-clique.cpp.o.requires:

.PHONY : CMakeFiles/motif_src.dir/maximal-motif-clique.cpp.o.requires

CMakeFiles/motif_src.dir/maximal-motif-clique.cpp.o.provides: CMakeFiles/motif_src.dir/maximal-motif-clique.cpp.o.requires
	$(MAKE) -f CMakeFiles/motif_src.dir/build.make CMakeFiles/motif_src.dir/maximal-motif-clique.cpp.o.provides.build
.PHONY : CMakeFiles/motif_src.dir/maximal-motif-clique.cpp.o.provides

CMakeFiles/motif_src.dir/maximal-motif-clique.cpp.o.provides.build: CMakeFiles/motif_src.dir/maximal-motif-clique.cpp.o


# Object files for target motif_src
motif_src_OBJECTS = \
"CMakeFiles/motif_src.dir/Graph.cpp.o" \
"CMakeFiles/motif_src.dir/main.cpp.o" \
"CMakeFiles/motif_src.dir/SetTrieNode.cpp.o" \
"CMakeFiles/motif_src.dir/subgraph_match.cpp.o" \
"CMakeFiles/motif_src.dir/global-variable.cpp.o" \
"CMakeFiles/motif_src.dir/maximal-motif-clique.cpp.o"

# External object files for target motif_src
motif_src_EXTERNAL_OBJECTS =

motif_src: CMakeFiles/motif_src.dir/Graph.cpp.o
motif_src: CMakeFiles/motif_src.dir/main.cpp.o
motif_src: CMakeFiles/motif_src.dir/SetTrieNode.cpp.o
motif_src: CMakeFiles/motif_src.dir/subgraph_match.cpp.o
motif_src: CMakeFiles/motif_src.dir/global-variable.cpp.o
motif_src: CMakeFiles/motif_src.dir/maximal-motif-clique.cpp.o
motif_src: CMakeFiles/motif_src.dir/build.make
motif_src: CMakeFiles/motif_src.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/hjf/Desktop/Motif/motif-src/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Linking CXX executable motif_src"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/motif_src.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/motif_src.dir/build: motif_src

.PHONY : CMakeFiles/motif_src.dir/build

CMakeFiles/motif_src.dir/requires: CMakeFiles/motif_src.dir/Graph.cpp.o.requires
CMakeFiles/motif_src.dir/requires: CMakeFiles/motif_src.dir/main.cpp.o.requires
CMakeFiles/motif_src.dir/requires: CMakeFiles/motif_src.dir/SetTrieNode.cpp.o.requires
CMakeFiles/motif_src.dir/requires: CMakeFiles/motif_src.dir/subgraph_match.cpp.o.requires
CMakeFiles/motif_src.dir/requires: CMakeFiles/motif_src.dir/global-variable.cpp.o.requires
CMakeFiles/motif_src.dir/requires: CMakeFiles/motif_src.dir/maximal-motif-clique.cpp.o.requires

.PHONY : CMakeFiles/motif_src.dir/requires

CMakeFiles/motif_src.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/motif_src.dir/cmake_clean.cmake
.PHONY : CMakeFiles/motif_src.dir/clean

CMakeFiles/motif_src.dir/depend:
	cd /Users/hjf/Desktop/Motif/motif-src/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/hjf/Desktop/Motif/motif-src /Users/hjf/Desktop/Motif/motif-src /Users/hjf/Desktop/Motif/motif-src/cmake-build-debug /Users/hjf/Desktop/Motif/motif-src/cmake-build-debug /Users/hjf/Desktop/Motif/motif-src/cmake-build-debug/CMakeFiles/motif_src.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/motif_src.dir/depend

