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
CMAKE_SOURCE_DIR = /home/asif/XML_GENERATION

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/asif/XML_GENERATION/build

# Utility rule file for NightlyCoverage.

# Include the progress variables for this target.
include lib/tinyxml2-master/CMakeFiles/NightlyCoverage.dir/progress.make

lib/tinyxml2-master/CMakeFiles/NightlyCoverage:
	cd /home/asif/XML_GENERATION/build/lib/tinyxml2-master && /usr/bin/ctest -D NightlyCoverage

NightlyCoverage: lib/tinyxml2-master/CMakeFiles/NightlyCoverage
NightlyCoverage: lib/tinyxml2-master/CMakeFiles/NightlyCoverage.dir/build.make

.PHONY : NightlyCoverage

# Rule to build all files generated by this target.
lib/tinyxml2-master/CMakeFiles/NightlyCoverage.dir/build: NightlyCoverage

.PHONY : lib/tinyxml2-master/CMakeFiles/NightlyCoverage.dir/build

lib/tinyxml2-master/CMakeFiles/NightlyCoverage.dir/clean:
	cd /home/asif/XML_GENERATION/build/lib/tinyxml2-master && $(CMAKE_COMMAND) -P CMakeFiles/NightlyCoverage.dir/cmake_clean.cmake
.PHONY : lib/tinyxml2-master/CMakeFiles/NightlyCoverage.dir/clean

lib/tinyxml2-master/CMakeFiles/NightlyCoverage.dir/depend:
	cd /home/asif/XML_GENERATION/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/asif/XML_GENERATION /home/asif/XML_GENERATION/lib/tinyxml2-master /home/asif/XML_GENERATION/build /home/asif/XML_GENERATION/build/lib/tinyxml2-master /home/asif/XML_GENERATION/build/lib/tinyxml2-master/CMakeFiles/NightlyCoverage.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : lib/tinyxml2-master/CMakeFiles/NightlyCoverage.dir/depend

