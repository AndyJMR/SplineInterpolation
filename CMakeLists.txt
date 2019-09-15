
cmake_minimum_required(VERSION 2.8)
 
PROJECT(SplineInterpolation)
 
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})
 
add_executable(SplineInterpolation MACOSX_BUNDLE SplineInterpolation.cpp )
 
target_link_libraries(SplineInterpolation ${VTK_LIBRARIES})
