
# find_package(GTest)

add_subdirectory(${PROJECT_SOURCE_DIR}/ThirdParty/googletest
  ThirdParty/googletest
  EXCLUDE_FROM_ALL)



add_executable(ImGuiExample
  ${CMAKE_CURRENT_SOURCE_DIR}/ImGuiExample.cpp
  )

target_link_libraries(ImGuiExample PRIVATE
  Corrade::Main
  Corrade::Utility
  Corrade::Containers
  Magnum::Application
  Magnum::GL
  Magnum::Magnum
  MagnumIntegration::ImGui
  )

enable_testing()

add_executable(
  test_readdata
  test_readdata.cpp
  ${PROJECT_SOURCE_DIR}/src/readdata.cpp
  )

target_include_directories(test_readdata PUBLIC
  ${PROJECT_SOURCE_DIR}/include
  )

target_link_libraries(
  test_readdata
  gtest_main
  Eigen3::Eigen
)

add_executable(
  test_poca
  test_poca.cpp
  ${POCA_SOURCES}
  )

target_include_directories(test_poca PUBLIC
  ${PROJECT_SOURCE_DIR}/include
  )

target_link_libraries(
  test_poca
  gtest_main
  Eigen3::Eigen
)

include(GoogleTest)

gtest_discover_tests(test_readdata)
gtest_discover_tests(test_poca)
