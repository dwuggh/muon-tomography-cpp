
include(GoogleTest)



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
