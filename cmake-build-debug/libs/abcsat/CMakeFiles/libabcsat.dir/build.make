# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.26

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
CMAKE_COMMAND = /home/lzx/clion/clion-2023.2.2/bin/cmake/linux/x64/bin/cmake

# The command to remove a file.
RM = /home/lzx/clion/clion-2023.2.2/bin/cmake/linux/x64/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/lzx/Desktop/EDA_challenge

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/lzx/Desktop/EDA_challenge/cmake-build-debug

# Include any dependencies generated for this target.
include libs/abcsat/CMakeFiles/libabcsat.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include libs/abcsat/CMakeFiles/libabcsat.dir/compiler_depend.make

# Include the progress variables for this target.
include libs/abcsat/CMakeFiles/libabcsat.dir/progress.make

# Include the compile flags for this target's objects.
include libs/abcsat/CMakeFiles/libabcsat.dir/flags.make

libs/abcsat/CMakeFiles/libabcsat.dir/AbcGlucose.cpp.o: libs/abcsat/CMakeFiles/libabcsat.dir/flags.make
libs/abcsat/CMakeFiles/libabcsat.dir/AbcGlucose.cpp.o: /home/lzx/Desktop/EDA_challenge/libs/abcsat/AbcGlucose.cpp
libs/abcsat/CMakeFiles/libabcsat.dir/AbcGlucose.cpp.o: libs/abcsat/CMakeFiles/libabcsat.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lzx/Desktop/EDA_challenge/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object libs/abcsat/CMakeFiles/libabcsat.dir/AbcGlucose.cpp.o"
	cd /home/lzx/Desktop/EDA_challenge/cmake-build-debug/libs/abcsat && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT libs/abcsat/CMakeFiles/libabcsat.dir/AbcGlucose.cpp.o -MF CMakeFiles/libabcsat.dir/AbcGlucose.cpp.o.d -o CMakeFiles/libabcsat.dir/AbcGlucose.cpp.o -c /home/lzx/Desktop/EDA_challenge/libs/abcsat/AbcGlucose.cpp

libs/abcsat/CMakeFiles/libabcsat.dir/AbcGlucose.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/libabcsat.dir/AbcGlucose.cpp.i"
	cd /home/lzx/Desktop/EDA_challenge/cmake-build-debug/libs/abcsat && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lzx/Desktop/EDA_challenge/libs/abcsat/AbcGlucose.cpp > CMakeFiles/libabcsat.dir/AbcGlucose.cpp.i

libs/abcsat/CMakeFiles/libabcsat.dir/AbcGlucose.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/libabcsat.dir/AbcGlucose.cpp.s"
	cd /home/lzx/Desktop/EDA_challenge/cmake-build-debug/libs/abcsat && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lzx/Desktop/EDA_challenge/libs/abcsat/AbcGlucose.cpp -o CMakeFiles/libabcsat.dir/AbcGlucose.cpp.s

libs/abcsat/CMakeFiles/libabcsat.dir/Glucose.cpp.o: libs/abcsat/CMakeFiles/libabcsat.dir/flags.make
libs/abcsat/CMakeFiles/libabcsat.dir/Glucose.cpp.o: /home/lzx/Desktop/EDA_challenge/libs/abcsat/Glucose.cpp
libs/abcsat/CMakeFiles/libabcsat.dir/Glucose.cpp.o: libs/abcsat/CMakeFiles/libabcsat.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lzx/Desktop/EDA_challenge/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object libs/abcsat/CMakeFiles/libabcsat.dir/Glucose.cpp.o"
	cd /home/lzx/Desktop/EDA_challenge/cmake-build-debug/libs/abcsat && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT libs/abcsat/CMakeFiles/libabcsat.dir/Glucose.cpp.o -MF CMakeFiles/libabcsat.dir/Glucose.cpp.o.d -o CMakeFiles/libabcsat.dir/Glucose.cpp.o -c /home/lzx/Desktop/EDA_challenge/libs/abcsat/Glucose.cpp

libs/abcsat/CMakeFiles/libabcsat.dir/Glucose.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/libabcsat.dir/Glucose.cpp.i"
	cd /home/lzx/Desktop/EDA_challenge/cmake-build-debug/libs/abcsat && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lzx/Desktop/EDA_challenge/libs/abcsat/Glucose.cpp > CMakeFiles/libabcsat.dir/Glucose.cpp.i

libs/abcsat/CMakeFiles/libabcsat.dir/Glucose.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/libabcsat.dir/Glucose.cpp.s"
	cd /home/lzx/Desktop/EDA_challenge/cmake-build-debug/libs/abcsat && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lzx/Desktop/EDA_challenge/libs/abcsat/Glucose.cpp -o CMakeFiles/libabcsat.dir/Glucose.cpp.s

libs/abcsat/CMakeFiles/libabcsat.dir/SimpSolver.cpp.o: libs/abcsat/CMakeFiles/libabcsat.dir/flags.make
libs/abcsat/CMakeFiles/libabcsat.dir/SimpSolver.cpp.o: /home/lzx/Desktop/EDA_challenge/libs/abcsat/SimpSolver.cpp
libs/abcsat/CMakeFiles/libabcsat.dir/SimpSolver.cpp.o: libs/abcsat/CMakeFiles/libabcsat.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lzx/Desktop/EDA_challenge/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object libs/abcsat/CMakeFiles/libabcsat.dir/SimpSolver.cpp.o"
	cd /home/lzx/Desktop/EDA_challenge/cmake-build-debug/libs/abcsat && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT libs/abcsat/CMakeFiles/libabcsat.dir/SimpSolver.cpp.o -MF CMakeFiles/libabcsat.dir/SimpSolver.cpp.o.d -o CMakeFiles/libabcsat.dir/SimpSolver.cpp.o -c /home/lzx/Desktop/EDA_challenge/libs/abcsat/SimpSolver.cpp

libs/abcsat/CMakeFiles/libabcsat.dir/SimpSolver.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/libabcsat.dir/SimpSolver.cpp.i"
	cd /home/lzx/Desktop/EDA_challenge/cmake-build-debug/libs/abcsat && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lzx/Desktop/EDA_challenge/libs/abcsat/SimpSolver.cpp > CMakeFiles/libabcsat.dir/SimpSolver.cpp.i

libs/abcsat/CMakeFiles/libabcsat.dir/SimpSolver.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/libabcsat.dir/SimpSolver.cpp.s"
	cd /home/lzx/Desktop/EDA_challenge/cmake-build-debug/libs/abcsat && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lzx/Desktop/EDA_challenge/libs/abcsat/SimpSolver.cpp -o CMakeFiles/libabcsat.dir/SimpSolver.cpp.s

libs/abcsat/CMakeFiles/libabcsat.dir/satSolver.cpp.o: libs/abcsat/CMakeFiles/libabcsat.dir/flags.make
libs/abcsat/CMakeFiles/libabcsat.dir/satSolver.cpp.o: /home/lzx/Desktop/EDA_challenge/libs/abcsat/satSolver.cpp
libs/abcsat/CMakeFiles/libabcsat.dir/satSolver.cpp.o: libs/abcsat/CMakeFiles/libabcsat.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lzx/Desktop/EDA_challenge/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object libs/abcsat/CMakeFiles/libabcsat.dir/satSolver.cpp.o"
	cd /home/lzx/Desktop/EDA_challenge/cmake-build-debug/libs/abcsat && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT libs/abcsat/CMakeFiles/libabcsat.dir/satSolver.cpp.o -MF CMakeFiles/libabcsat.dir/satSolver.cpp.o.d -o CMakeFiles/libabcsat.dir/satSolver.cpp.o -c /home/lzx/Desktop/EDA_challenge/libs/abcsat/satSolver.cpp

libs/abcsat/CMakeFiles/libabcsat.dir/satSolver.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/libabcsat.dir/satSolver.cpp.i"
	cd /home/lzx/Desktop/EDA_challenge/cmake-build-debug/libs/abcsat && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lzx/Desktop/EDA_challenge/libs/abcsat/satSolver.cpp > CMakeFiles/libabcsat.dir/satSolver.cpp.i

libs/abcsat/CMakeFiles/libabcsat.dir/satSolver.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/libabcsat.dir/satSolver.cpp.s"
	cd /home/lzx/Desktop/EDA_challenge/cmake-build-debug/libs/abcsat && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lzx/Desktop/EDA_challenge/libs/abcsat/satSolver.cpp -o CMakeFiles/libabcsat.dir/satSolver.cpp.s

libs/abcsat/CMakeFiles/libabcsat.dir/satStore.cpp.o: libs/abcsat/CMakeFiles/libabcsat.dir/flags.make
libs/abcsat/CMakeFiles/libabcsat.dir/satStore.cpp.o: /home/lzx/Desktop/EDA_challenge/libs/abcsat/satStore.cpp
libs/abcsat/CMakeFiles/libabcsat.dir/satStore.cpp.o: libs/abcsat/CMakeFiles/libabcsat.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lzx/Desktop/EDA_challenge/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object libs/abcsat/CMakeFiles/libabcsat.dir/satStore.cpp.o"
	cd /home/lzx/Desktop/EDA_challenge/cmake-build-debug/libs/abcsat && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT libs/abcsat/CMakeFiles/libabcsat.dir/satStore.cpp.o -MF CMakeFiles/libabcsat.dir/satStore.cpp.o.d -o CMakeFiles/libabcsat.dir/satStore.cpp.o -c /home/lzx/Desktop/EDA_challenge/libs/abcsat/satStore.cpp

libs/abcsat/CMakeFiles/libabcsat.dir/satStore.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/libabcsat.dir/satStore.cpp.i"
	cd /home/lzx/Desktop/EDA_challenge/cmake-build-debug/libs/abcsat && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lzx/Desktop/EDA_challenge/libs/abcsat/satStore.cpp > CMakeFiles/libabcsat.dir/satStore.cpp.i

libs/abcsat/CMakeFiles/libabcsat.dir/satStore.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/libabcsat.dir/satStore.cpp.s"
	cd /home/lzx/Desktop/EDA_challenge/cmake-build-debug/libs/abcsat && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lzx/Desktop/EDA_challenge/libs/abcsat/satStore.cpp -o CMakeFiles/libabcsat.dir/satStore.cpp.s

# Object files for target libabcsat
libabcsat_OBJECTS = \
"CMakeFiles/libabcsat.dir/AbcGlucose.cpp.o" \
"CMakeFiles/libabcsat.dir/Glucose.cpp.o" \
"CMakeFiles/libabcsat.dir/SimpSolver.cpp.o" \
"CMakeFiles/libabcsat.dir/satSolver.cpp.o" \
"CMakeFiles/libabcsat.dir/satStore.cpp.o"

# External object files for target libabcsat
libabcsat_EXTERNAL_OBJECTS =

libs/abcsat/liblibabcsat.a: libs/abcsat/CMakeFiles/libabcsat.dir/AbcGlucose.cpp.o
libs/abcsat/liblibabcsat.a: libs/abcsat/CMakeFiles/libabcsat.dir/Glucose.cpp.o
libs/abcsat/liblibabcsat.a: libs/abcsat/CMakeFiles/libabcsat.dir/SimpSolver.cpp.o
libs/abcsat/liblibabcsat.a: libs/abcsat/CMakeFiles/libabcsat.dir/satSolver.cpp.o
libs/abcsat/liblibabcsat.a: libs/abcsat/CMakeFiles/libabcsat.dir/satStore.cpp.o
libs/abcsat/liblibabcsat.a: libs/abcsat/CMakeFiles/libabcsat.dir/build.make
libs/abcsat/liblibabcsat.a: libs/abcsat/CMakeFiles/libabcsat.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/lzx/Desktop/EDA_challenge/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking CXX static library liblibabcsat.a"
	cd /home/lzx/Desktop/EDA_challenge/cmake-build-debug/libs/abcsat && $(CMAKE_COMMAND) -P CMakeFiles/libabcsat.dir/cmake_clean_target.cmake
	cd /home/lzx/Desktop/EDA_challenge/cmake-build-debug/libs/abcsat && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/libabcsat.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
libs/abcsat/CMakeFiles/libabcsat.dir/build: libs/abcsat/liblibabcsat.a
.PHONY : libs/abcsat/CMakeFiles/libabcsat.dir/build

libs/abcsat/CMakeFiles/libabcsat.dir/clean:
	cd /home/lzx/Desktop/EDA_challenge/cmake-build-debug/libs/abcsat && $(CMAKE_COMMAND) -P CMakeFiles/libabcsat.dir/cmake_clean.cmake
.PHONY : libs/abcsat/CMakeFiles/libabcsat.dir/clean

libs/abcsat/CMakeFiles/libabcsat.dir/depend:
	cd /home/lzx/Desktop/EDA_challenge/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/lzx/Desktop/EDA_challenge /home/lzx/Desktop/EDA_challenge/libs/abcsat /home/lzx/Desktop/EDA_challenge/cmake-build-debug /home/lzx/Desktop/EDA_challenge/cmake-build-debug/libs/abcsat /home/lzx/Desktop/EDA_challenge/cmake-build-debug/libs/abcsat/CMakeFiles/libabcsat.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : libs/abcsat/CMakeFiles/libabcsat.dir/depend

