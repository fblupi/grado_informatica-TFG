
cmake_minimum_required(VERSION 2.8)
PROJECT(3DReconstructionFromDICOM)

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

add_executable(3DReconstructionFromDICOM MACOSX_BUNDLE 3DReconstructionFromDICOM)

target_link_libraries(3DReconstructionFromDICOM ${VTK_LIBRARIES})