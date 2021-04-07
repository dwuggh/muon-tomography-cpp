cmake_minimum_required(VERSION 3.10)

project(muon-tomography)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Armadillo REQUIRED)
include_directories(${ARMADILLO_INCLUDE_DIRS})


find_package(Corrade REQUIRED Containers)
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

add_subdirectory(src)
add_subdirectory(test)

add_executable(${PROJECT_NAME}
  ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/grid.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/visualize/muontomography.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/visualize/meshgroup.cpp
  )

target_include_directories(${PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/include/visualize
  )

# target_link_libraries(${PROJECT_NAME} )

target_link_libraries(${PROJECT_NAME} PRIVATE
  ${ARMADILLO_LIBRARIES}
  Corrade::Utility
  Corrade::Containers
  Magnum::Application
  Magnum::GL
  Magnum::Magnum
  Magnum::MeshTools
  Magnum::SceneGraph
  Magnum::Primitives
  Magnum::Shaders
  )
