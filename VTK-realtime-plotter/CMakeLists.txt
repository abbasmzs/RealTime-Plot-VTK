cmake_minimum_required(VERSION 2.8.8)
project(vtk-realtime-plotter-example)
find_package(VTK REQUIRED)

find_package(VTK COMPONENTS 
  ChartsCore
  CommonColor
  CommonCore
  CommonDataModel
  InteractionStyle
  RenderingContext2D
  RenderingContextOpenGL2
  RenderingCore
  RenderingFreeType
  RenderingGL2PSOpenGL2
  RenderingOpenGL2
  ViewsContext2D
)

if (NOT VTK_FOUND)
  message(FATAL_ERROR "LinePlot: Unable to find the VTK build folder.")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-pthread")

include(${VTK_USE_FILE})
include_directories(${CMAKE_SOURCE_DIR}F:/VTK/Debug/include ${VTK_INCLUDE_DIRS})

add_executable(example example.cpp)
target_link_libraries(example ${VTK_LIBRARIES})

# vtk_module_autoinit is needed
vtk_module_autoinit(
  TARGETS example 
  MODULES ${VTK_LIBRARIES}
)
