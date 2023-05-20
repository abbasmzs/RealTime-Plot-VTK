
cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

project(LinePlot)

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

# Prevent a "command line is too long" failure in Windows.
set(CMAKE_NINJA_FORCE_RESPONSE_FILE "ON" CACHE BOOL "Force Ninja to use response files.")
add_executable(LinePlot MACOSX_BUNDLE LinePlot.cxx )
  target_link_libraries(LinePlot PRIVATE ${VTK_LIBRARIES}
)
# vtk_module_autoinit is needed
vtk_module_autoinit(
  TARGETS LinePlot
  MODULES ${VTK_LIBRARIES}
)
