# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

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
CMAKE_SOURCE_DIR = /home/kellekai/Dokumente/BSC/WORK/FTI_REPOS/debug

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/kellekai/Dokumente/BSC/WORK/FTI_REPOS/debug/src

# Include any dependencies generated for this target.
include CMakeFiles/fti_f90.shared.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/fti_f90.shared.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/fti_f90.shared.dir/flags.make

interface.F90: fortran/interface.F90.bpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/kellekai/Dokumente/BSC/WORK/FTI_REPOS/debug/src/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating interface.F90"
	cd /home/kellekai/Dokumente/BSC/WORK/FTI_REPOS/debug && /home/kellekai/Dokumente/BSC/WORK/FTI_REPOS/debug/src/bpp/bpp -I /home/kellekai/Dokumente/BSC/WORK/FTI_REPOS/debug/vendor/bpp/include/ -I /home/kellekai/Dokumente/BSC/WORK/FTI_REPOS/debug/src -I /home/kellekai/Dokumente/BSC/WORK/FTI_REPOS/debug/src -I /home/kellekai/Dokumente/BSC/WORK/FTI_REPOS/debug/include -I /usr/lib/openmpi/include -I /usr/lib/openmpi/lib -I /usr/lib/openmpi/include/openmpi/opal/mca/event/libevent2021/libevent -I /usr/lib/openmpi/include/openmpi/opal/mca/event/libevent2021/libevent/include -I /usr/lib/openmpi/include -I /usr/lib/openmpi/include/openmpi -I /home/kellekai/Dokumente/BSC/WORK/FTI_REPOS/debug/src/bppconf src/fortran/interface.F90.bpp /home/kellekai/Dokumente/BSC/WORK/FTI_REPOS/debug/src/interface.F90

CMakeFiles/fti_f90.shared.dir/interface.F90.o: CMakeFiles/fti_f90.shared.dir/flags.make
CMakeFiles/fti_f90.shared.dir/interface.F90.o: interface.F90
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kellekai/Dokumente/BSC/WORK/FTI_REPOS/debug/src/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building Fortran object CMakeFiles/fti_f90.shared.dir/interface.F90.o"
	/usr/bin/gfortran  $(Fortran_DEFINES) $(Fortran_INCLUDES) $(Fortran_FLAGS) -c /home/kellekai/Dokumente/BSC/WORK/FTI_REPOS/debug/src/interface.F90 -o CMakeFiles/fti_f90.shared.dir/interface.F90.o

CMakeFiles/fti_f90.shared.dir/interface.F90.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing Fortran source to CMakeFiles/fti_f90.shared.dir/interface.F90.i"
	/usr/bin/gfortran  $(Fortran_DEFINES) $(Fortran_INCLUDES) $(Fortran_FLAGS) -E /home/kellekai/Dokumente/BSC/WORK/FTI_REPOS/debug/src/interface.F90 > CMakeFiles/fti_f90.shared.dir/interface.F90.i

CMakeFiles/fti_f90.shared.dir/interface.F90.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling Fortran source to assembly CMakeFiles/fti_f90.shared.dir/interface.F90.s"
	/usr/bin/gfortran  $(Fortran_DEFINES) $(Fortran_INCLUDES) $(Fortran_FLAGS) -S /home/kellekai/Dokumente/BSC/WORK/FTI_REPOS/debug/src/interface.F90 -o CMakeFiles/fti_f90.shared.dir/interface.F90.s

CMakeFiles/fti_f90.shared.dir/interface.F90.o.requires:

.PHONY : CMakeFiles/fti_f90.shared.dir/interface.F90.o.requires

CMakeFiles/fti_f90.shared.dir/interface.F90.o.provides: CMakeFiles/fti_f90.shared.dir/interface.F90.o.requires
	$(MAKE) -f CMakeFiles/fti_f90.shared.dir/build.make CMakeFiles/fti_f90.shared.dir/interface.F90.o.provides.build
.PHONY : CMakeFiles/fti_f90.shared.dir/interface.F90.o.provides

CMakeFiles/fti_f90.shared.dir/interface.F90.o.provides.build: CMakeFiles/fti_f90.shared.dir/interface.F90.o


CMakeFiles/fti_f90.shared.dir/fortran/ftif.c.o: CMakeFiles/fti_f90.shared.dir/flags.make
CMakeFiles/fti_f90.shared.dir/fortran/ftif.c.o: fortran/ftif.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kellekai/Dokumente/BSC/WORK/FTI_REPOS/debug/src/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/fti_f90.shared.dir/fortran/ftif.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/fti_f90.shared.dir/fortran/ftif.c.o   -c /home/kellekai/Dokumente/BSC/WORK/FTI_REPOS/debug/src/fortran/ftif.c

CMakeFiles/fti_f90.shared.dir/fortran/ftif.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/fti_f90.shared.dir/fortran/ftif.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/kellekai/Dokumente/BSC/WORK/FTI_REPOS/debug/src/fortran/ftif.c > CMakeFiles/fti_f90.shared.dir/fortran/ftif.c.i

CMakeFiles/fti_f90.shared.dir/fortran/ftif.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/fti_f90.shared.dir/fortran/ftif.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/kellekai/Dokumente/BSC/WORK/FTI_REPOS/debug/src/fortran/ftif.c -o CMakeFiles/fti_f90.shared.dir/fortran/ftif.c.s

CMakeFiles/fti_f90.shared.dir/fortran/ftif.c.o.requires:

.PHONY : CMakeFiles/fti_f90.shared.dir/fortran/ftif.c.o.requires

CMakeFiles/fti_f90.shared.dir/fortran/ftif.c.o.provides: CMakeFiles/fti_f90.shared.dir/fortran/ftif.c.o.requires
	$(MAKE) -f CMakeFiles/fti_f90.shared.dir/build.make CMakeFiles/fti_f90.shared.dir/fortran/ftif.c.o.provides.build
.PHONY : CMakeFiles/fti_f90.shared.dir/fortran/ftif.c.o.provides

CMakeFiles/fti_f90.shared.dir/fortran/ftif.c.o.provides.build: CMakeFiles/fti_f90.shared.dir/fortran/ftif.c.o


# Object files for target fti_f90.shared
fti_f90_shared_OBJECTS = \
"CMakeFiles/fti_f90.shared.dir/interface.F90.o" \
"CMakeFiles/fti_f90.shared.dir/fortran/ftif.c.o"

# External object files for target fti_f90.shared
fti_f90_shared_EXTERNAL_OBJECTS =

lib/libfti_f90.so: CMakeFiles/fti_f90.shared.dir/interface.F90.o
lib/libfti_f90.so: CMakeFiles/fti_f90.shared.dir/fortran/ftif.c.o
lib/libfti_f90.so: CMakeFiles/fti_f90.shared.dir/build.make
lib/libfti_f90.so: lib/libfti.so
lib/libfti_f90.so: /usr/lib/openmpi/lib/libmpi_usempif08.so
lib/libfti_f90.so: /usr/lib/openmpi/lib/libmpi_usempi_ignore_tkr.so
lib/libfti_f90.so: /usr/lib/openmpi/lib/libmpi_mpifh.so
lib/libfti_f90.so: /usr/lib/openmpi/lib/libmpi.so
lib/libfti_f90.so: /usr/lib/openmpi/lib/libmpi.so
lib/libfti_f90.so: CMakeFiles/fti_f90.shared.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/kellekai/Dokumente/BSC/WORK/FTI_REPOS/debug/src/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking Fortran shared library lib/libfti_f90.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/fti_f90.shared.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/fti_f90.shared.dir/build: lib/libfti_f90.so

.PHONY : CMakeFiles/fti_f90.shared.dir/build

CMakeFiles/fti_f90.shared.dir/requires: CMakeFiles/fti_f90.shared.dir/interface.F90.o.requires
CMakeFiles/fti_f90.shared.dir/requires: CMakeFiles/fti_f90.shared.dir/fortran/ftif.c.o.requires

.PHONY : CMakeFiles/fti_f90.shared.dir/requires

CMakeFiles/fti_f90.shared.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/fti_f90.shared.dir/cmake_clean.cmake
.PHONY : CMakeFiles/fti_f90.shared.dir/clean

CMakeFiles/fti_f90.shared.dir/depend: interface.F90
	cd /home/kellekai/Dokumente/BSC/WORK/FTI_REPOS/debug/src && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/kellekai/Dokumente/BSC/WORK/FTI_REPOS/debug /home/kellekai/Dokumente/BSC/WORK/FTI_REPOS/debug /home/kellekai/Dokumente/BSC/WORK/FTI_REPOS/debug/src /home/kellekai/Dokumente/BSC/WORK/FTI_REPOS/debug/src /home/kellekai/Dokumente/BSC/WORK/FTI_REPOS/debug/src/CMakeFiles/fti_f90.shared.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/fti_f90.shared.dir/depend

