# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.18

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /opt/pylon/share/pylon/Samples/C++/Grab_ChunkImage

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /opt/pylon/share/pylon/Samples/C++/Grab_ChunkImage

# Include any dependencies generated for this target.
include CMakeFiles/basler_test.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/basler_test.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/basler_test.dir/flags.make

CMakeFiles/basler_test.dir/openCVGrab.cpp.o: CMakeFiles/basler_test.dir/flags.make
CMakeFiles/basler_test.dir/openCVGrab.cpp.o: openCVGrab.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/opt/pylon/share/pylon/Samples/C++/Grab_ChunkImage/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/basler_test.dir/openCVGrab.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/basler_test.dir/openCVGrab.cpp.o -c /opt/pylon/share/pylon/Samples/C++/Grab_ChunkImage/openCVGrab.cpp

CMakeFiles/basler_test.dir/openCVGrab.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/basler_test.dir/openCVGrab.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /opt/pylon/share/pylon/Samples/C++/Grab_ChunkImage/openCVGrab.cpp > CMakeFiles/basler_test.dir/openCVGrab.cpp.i

CMakeFiles/basler_test.dir/openCVGrab.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/basler_test.dir/openCVGrab.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /opt/pylon/share/pylon/Samples/C++/Grab_ChunkImage/openCVGrab.cpp -o CMakeFiles/basler_test.dir/openCVGrab.cpp.s

# Object files for target basler_test
basler_test_OBJECTS = \
"CMakeFiles/basler_test.dir/openCVGrab.cpp.o"

# External object files for target basler_test
basler_test_EXTERNAL_OBJECTS =

basler_test: CMakeFiles/basler_test.dir/openCVGrab.cpp.o
basler_test: CMakeFiles/basler_test.dir/build.make
basler_test: /opt/opencv-4.1.0/lib/libopencv_gapi.so.4.1.0
basler_test: /opt/opencv-4.1.0/lib/libopencv_stitching.so.4.1.0
basler_test: /opt/opencv-4.1.0/lib/libopencv_aruco.so.4.1.0
basler_test: /opt/opencv-4.1.0/lib/libopencv_bgsegm.so.4.1.0
basler_test: /opt/opencv-4.1.0/lib/libopencv_bioinspired.so.4.1.0
basler_test: /opt/opencv-4.1.0/lib/libopencv_ccalib.so.4.1.0
basler_test: /opt/opencv-4.1.0/lib/libopencv_dnn_objdetect.so.4.1.0
basler_test: /opt/opencv-4.1.0/lib/libopencv_dpm.so.4.1.0
basler_test: /opt/opencv-4.1.0/lib/libopencv_face.so.4.1.0
basler_test: /opt/opencv-4.1.0/lib/libopencv_freetype.so.4.1.0
basler_test: /opt/opencv-4.1.0/lib/libopencv_fuzzy.so.4.1.0
basler_test: /opt/opencv-4.1.0/lib/libopencv_hfs.so.4.1.0
basler_test: /opt/opencv-4.1.0/lib/libopencv_img_hash.so.4.1.0
basler_test: /opt/opencv-4.1.0/lib/libopencv_line_descriptor.so.4.1.0
basler_test: /opt/opencv-4.1.0/lib/libopencv_quality.so.4.1.0
basler_test: /opt/opencv-4.1.0/lib/libopencv_reg.so.4.1.0
basler_test: /opt/opencv-4.1.0/lib/libopencv_rgbd.so.4.1.0
basler_test: /opt/opencv-4.1.0/lib/libopencv_saliency.so.4.1.0
basler_test: /opt/opencv-4.1.0/lib/libopencv_stereo.so.4.1.0
basler_test: /opt/opencv-4.1.0/lib/libopencv_structured_light.so.4.1.0
basler_test: /opt/opencv-4.1.0/lib/libopencv_superres.so.4.1.0
basler_test: /opt/opencv-4.1.0/lib/libopencv_surface_matching.so.4.1.0
basler_test: /opt/opencv-4.1.0/lib/libopencv_tracking.so.4.1.0
basler_test: /opt/opencv-4.1.0/lib/libopencv_videostab.so.4.1.0
basler_test: /opt/opencv-4.1.0/lib/libopencv_xfeatures2d.so.4.1.0
basler_test: /opt/opencv-4.1.0/lib/libopencv_xobjdetect.so.4.1.0
basler_test: /opt/opencv-4.1.0/lib/libopencv_xphoto.so.4.1.0
basler_test: /opt/opencv-4.1.0/lib/libopencv_shape.so.4.1.0
basler_test: /opt/opencv-4.1.0/lib/libopencv_datasets.so.4.1.0
basler_test: /opt/opencv-4.1.0/lib/libopencv_plot.so.4.1.0
basler_test: /opt/opencv-4.1.0/lib/libopencv_text.so.4.1.0
basler_test: /opt/opencv-4.1.0/lib/libopencv_dnn.so.4.1.0
basler_test: /opt/opencv-4.1.0/lib/libopencv_ml.so.4.1.0
basler_test: /opt/opencv-4.1.0/lib/libopencv_phase_unwrapping.so.4.1.0
basler_test: /opt/opencv-4.1.0/lib/libopencv_optflow.so.4.1.0
basler_test: /opt/opencv-4.1.0/lib/libopencv_ximgproc.so.4.1.0
basler_test: /opt/opencv-4.1.0/lib/libopencv_video.so.4.1.0
basler_test: /opt/opencv-4.1.0/lib/libopencv_objdetect.so.4.1.0
basler_test: /opt/opencv-4.1.0/lib/libopencv_calib3d.so.4.1.0
basler_test: /opt/opencv-4.1.0/lib/libopencv_features2d.so.4.1.0
basler_test: /opt/opencv-4.1.0/lib/libopencv_flann.so.4.1.0
basler_test: /opt/opencv-4.1.0/lib/libopencv_highgui.so.4.1.0
basler_test: /opt/opencv-4.1.0/lib/libopencv_videoio.so.4.1.0
basler_test: /opt/opencv-4.1.0/lib/libopencv_imgcodecs.so.4.1.0
basler_test: /opt/opencv-4.1.0/lib/libopencv_photo.so.4.1.0
basler_test: /opt/opencv-4.1.0/lib/libopencv_imgproc.so.4.1.0
basler_test: /opt/opencv-4.1.0/lib/libopencv_core.so.4.1.0
basler_test: CMakeFiles/basler_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/opt/pylon/share/pylon/Samples/C++/Grab_ChunkImage/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable basler_test"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/basler_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/basler_test.dir/build: basler_test

.PHONY : CMakeFiles/basler_test.dir/build

CMakeFiles/basler_test.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/basler_test.dir/cmake_clean.cmake
.PHONY : CMakeFiles/basler_test.dir/clean

CMakeFiles/basler_test.dir/depend:
	cd /opt/pylon/share/pylon/Samples/C++/Grab_ChunkImage && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /opt/pylon/share/pylon/Samples/C++/Grab_ChunkImage /opt/pylon/share/pylon/Samples/C++/Grab_ChunkImage /opt/pylon/share/pylon/Samples/C++/Grab_ChunkImage /opt/pylon/share/pylon/Samples/C++/Grab_ChunkImage /opt/pylon/share/pylon/Samples/C++/Grab_ChunkImage/CMakeFiles/basler_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/basler_test.dir/depend

