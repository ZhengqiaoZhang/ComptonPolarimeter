# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.17

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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/zhengqiao/test_geant4/compton_IP6_latticeDesign

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/zhengqiao/test_geant4/compton_IP6_latticeDesign/build

# Include any dependencies generated for this target.
include CMakeFiles/run.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/run.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/run.dir/flags.make

CMakeFiles/run.dir/run.cxx.o: CMakeFiles/run.dir/flags.make
CMakeFiles/run.dir/run.cxx.o: ../run.cxx
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/zhengqiao/test_geant4/compton_IP6_latticeDesign/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/run.dir/run.cxx.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/run.dir/run.cxx.o -c /Users/zhengqiao/test_geant4/compton_IP6_latticeDesign/run.cxx

CMakeFiles/run.dir/run.cxx.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/run.dir/run.cxx.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/zhengqiao/test_geant4/compton_IP6_latticeDesign/run.cxx > CMakeFiles/run.dir/run.cxx.i

CMakeFiles/run.dir/run.cxx.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/run.dir/run.cxx.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/zhengqiao/test_geant4/compton_IP6_latticeDesign/run.cxx -o CMakeFiles/run.dir/run.cxx.s

# Object files for target run
run_OBJECTS = \
"CMakeFiles/run.dir/run.cxx.o"

# External object files for target run
run_EXTERNAL_OBJECTS =

run: CMakeFiles/run.dir/run.cxx.o
run: CMakeFiles/run.dir/build.make
run: libdet.dylib
run: /Users/zhengqiao/geant4/geant4-v11.0.1-install/lib/libG4Tree.dylib
run: /Users/zhengqiao/geant4/geant4-v11.0.1-install/lib/libG4FR.dylib
run: /Users/zhengqiao/geant4/geant4-v11.0.1-install/lib/libG4GMocren.dylib
run: /Users/zhengqiao/geant4/geant4-v11.0.1-install/lib/libG4visHepRep.dylib
run: /Users/zhengqiao/geant4/geant4-v11.0.1-install/lib/libG4RayTracer.dylib
run: /Users/zhengqiao/geant4/geant4-v11.0.1-install/lib/libG4VRML.dylib
run: /Users/zhengqiao/geant4/geant4-v11.0.1-install/lib/libG4OpenGL.dylib
run: /usr/local/lib/QtOpenGL.framework/QtOpenGL
run: /usr/local/Cellar/qt@5/5.15.3/lib/QtPrintSupport.framework/QtPrintSupport
run: /usr/X11R6/lib/libGL.dylib
run: /Users/zhengqiao/geant4/geant4-v11.0.1-install/lib/libG4gl2ps.dylib
run: /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX12.3.sdk/System/Library/Frameworks/OpenGL.framework/OpenGL.tbd
run: /Users/zhengqiao/geant4/geant4-v11.0.1-install/lib/libG4visQt3D.dylib
run: /usr/local/lib/Qt3DExtras.framework/Qt3DExtras
run: /usr/local/lib/Qt3DInput.framework/Qt3DInput
run: /usr/local/Cellar/qt@5/5.15.3/lib/QtGamepad.framework/QtGamepad
run: /usr/local/lib/Qt3DLogic.framework/Qt3DLogic
run: /usr/local/Cellar/qt@5/5.15.3/lib/Qt3DRender.framework/Qt3DRender
run: /usr/local/lib/Qt3DCore.framework/Qt3DCore
run: /usr/local/Cellar/qt@5/5.15.3/lib/QtNetwork.framework/QtNetwork
run: /Users/zhengqiao/geant4/geant4-v11.0.1-install/lib/libG4vis_management.dylib
run: /Users/zhengqiao/geant4/geant4-v11.0.1-install/lib/libG4modeling.dylib
run: /Users/zhengqiao/geant4/geant4-v11.0.1-install/lib/libG4interfaces.dylib
run: /usr/local/lib/libXm.dylib
run: /usr/local/Cellar/qt@5/5.15.3/lib/QtWidgets.framework/QtWidgets
run: /usr/local/Cellar/qt@5/5.15.3/lib/QtGui.framework/QtGui
run: /usr/local/lib/libXmu.dylib
run: /usr/local/lib/libXext.dylib
run: /usr/local/Cellar/qt@5/5.15.3/lib/QtCore.framework/QtCore
run: /usr/local/lib/libXt.dylib
run: /usr/local/lib/libICE.dylib
run: /usr/local/lib/libSM.dylib
run: /usr/local/lib/libX11.dylib
run: /Users/zhengqiao/geant4/geant4-v11.0.1-install/lib/libG4persistency.dylib
run: /usr/local/lib/libxerces-c.dylib
run: /Users/zhengqiao/geant4/geant4-v11.0.1-install/lib/libG4error_propagation.dylib
run: /Users/zhengqiao/geant4/geant4-v11.0.1-install/lib/libG4readout.dylib
run: /Users/zhengqiao/geant4/geant4-v11.0.1-install/lib/libG4physicslists.dylib
run: /Users/zhengqiao/geant4/geant4-v11.0.1-install/lib/libG4tasking.dylib
run: /Users/zhengqiao/geant4/geant4-v11.0.1-install/lib/libG4run.dylib
run: /Users/zhengqiao/geant4/geant4-v11.0.1-install/lib/libG4event.dylib
run: /Users/zhengqiao/geant4/geant4-v11.0.1-install/lib/libG4tracking.dylib
run: /Users/zhengqiao/geant4/geant4-v11.0.1-install/lib/libG4parmodels.dylib
run: /Users/zhengqiao/geant4/geant4-v11.0.1-install/lib/libG4processes.dylib
run: /Users/zhengqiao/geant4/geant4-v11.0.1-install/lib/libG4analysis.dylib
run: /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX12.3.sdk/usr/lib/libexpat.tbd
run: /Users/zhengqiao/geant4/geant4-v11.0.1-install/lib/libG4digits_hits.dylib
run: /Users/zhengqiao/geant4/geant4-v11.0.1-install/lib/libG4track.dylib
run: /Users/zhengqiao/geant4/geant4-v11.0.1-install/lib/libG4particles.dylib
run: /Users/zhengqiao/geant4/geant4-v11.0.1-install/lib/libG4geometry.dylib
run: /Users/zhengqiao/geant4/geant4-v11.0.1-install/lib/libG4materials.dylib
run: /Users/zhengqiao/geant4/geant4-v11.0.1-install/lib/libG4graphics_reps.dylib
run: /Users/zhengqiao/geant4/geant4-v11.0.1-install/lib/libG4intercoms.dylib
run: /Users/zhengqiao/geant4/geant4-v11.0.1-install/lib/libG4global.dylib
run: /usr/local/lib/libCLHEP-2.4.5.1.dylib
run: /Users/zhengqiao/geant4/geant4-v11.0.1-install/lib/libG4tools.dylib
run: /Users/zhengqiao/geant4/geant4-v11.0.1-install/lib/libG4zlib.dylib
run: /Users/zhengqiao/geant4/geant4-v11.0.1-install/lib/libG4ptl.0.0.2.dylib
run: CMakeFiles/run.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/zhengqiao/test_geant4/compton_IP6_latticeDesign/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable run"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/run.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/run.dir/build: run

.PHONY : CMakeFiles/run.dir/build

CMakeFiles/run.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/run.dir/cmake_clean.cmake
.PHONY : CMakeFiles/run.dir/clean

CMakeFiles/run.dir/depend:
	cd /Users/zhengqiao/test_geant4/compton_IP6_latticeDesign/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/zhengqiao/test_geant4/compton_IP6_latticeDesign /Users/zhengqiao/test_geant4/compton_IP6_latticeDesign /Users/zhengqiao/test_geant4/compton_IP6_latticeDesign/build /Users/zhengqiao/test_geant4/compton_IP6_latticeDesign/build /Users/zhengqiao/test_geant4/compton_IP6_latticeDesign/build/CMakeFiles/run.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/run.dir/depend

