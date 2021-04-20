cmake_minimum_required(VERSION 3.10)

project(muon-tomography CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package (Eigen3 3.3 REQUIRED NO_MODULE)

find_package(Corrade REQUIRED Containers Main)
# include_directories(${CORRADE_INCLUDE_DIRS})

find_package(Magnum
  REQUIRED
  GL
  MeshTools
  SceneGraph
  Primitives
  Shaders
  Sdl2Application
  )
set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)

set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/imgui)
set(WITH_IMGUI ON CACHE BOOL "" FORCE)
set(WITH_EIGEN ON CACHE BOOL "" FORCE)
add_subdirectory(ThirdParty/magnum-integration EXCLUDE_FROM_ALL)

# set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Modules/" ${CMAKE_MODULE_PATH})
find_package(ImGui)
find_package(MagnumIntegration REQUIRED ImGui)

add_subdirectory(src)

file(GLOB_RECURSE POCA_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/src/grid.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/poca.cpp
  )

add_executable(${PROJECT_NAME}
  ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/grid.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/poca.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/readdata.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/visualize/muontomography.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/visualize/voxel.cpp
  )

target_include_directories(${PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/include/visualize
  )

target_link_libraries(${PROJECT_NAME} PRIVATE
  Eigen3::Eigen
  Corrade::Main
  Corrade::Utility
  Corrade::Containers
  Magnum::Application
  Magnum::GL
  Magnum::Magnum
  Magnum::MeshTools
  Magnum::SceneGraph
  Magnum::Primitives
  Magnum::Shaders
  MagnumIntegration::ImGui
  )

set(TERM_NAME ${PROJECT_NAME}_TERM)

add_executable(${TERM_NAME}
  ${CMAKE_CURRENT_SOURCE_DIR}/src/bin/terminal.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/readdata.cpp
  ${POCA_SOURCES}
  )

target_include_directories(${TERM_NAME} PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  )

target_link_libraries(${PROJECT_NAME} PRIVATE
  Eigen3::Eigen
  )

add_subdirectory(test)
