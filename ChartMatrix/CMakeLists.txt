
cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

project(ChartMatrix)

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
  message(FATAL_ERROR "ChartMatrix: Unable to find the VTK build folder.")
endif()

# Prevent a "command line is too long" failure in Windows.
set(CMAKE_NINJA_FORCE_RESPONSE_FILE "ON" CACHE BOOL "Force Ninja to use response files.")
add_executable(ChartMatrix MACOSX_BUNDLE ChartMatrix.cxx )
  target_link_libraries(ChartMatrix PRIVATE ${VTK_LIBRARIES}
)
# vtk_module_autoinit is needed
vtk_module_autoinit(
  TARGETS ChartMatrix
  MODULES ${VTK_LIBRARIES}
)
