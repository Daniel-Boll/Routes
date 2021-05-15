cmake_minimum_required(VERSION 3.12)
project(Routes VERSION 1.0.0)

# ------------------------------ Fetch FTXUI ----------------------------------
include(FetchContent)

set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  GIT_TAG 7daeac25c003b14898f9ff9c6731ceb83ec55fd3
)

FetchContent_GetProperties(ftxui)
if(NOT ftxui_POPULATED)
  FetchContent_Populate(ftxui)
  add_subdirectory(${ftxui_SOURCE_DIR} ${ftxui_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()
# -----------------------------------------------------------------------------

# ------------------------------- Fetch JSON ----------------------------------
include(FetchContent)

FetchContent_Declare(json
  GIT_REPOSITORY https://github.com/ArthurSonzogni/nlohmann_json_cmake_fetchcontent
  GIT_TAG v3.7.3)

FetchContent_GetProperties(json)
if(NOT json_POPULATED)
  FetchContent_Populate(json)
  add_subdirectory(${json_SOURCE_DIR} ${json_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()
# -----------------------------------------------------------------------------

# ----------------------------- Add DLinkedList Lib ---------------------------
add_library(
  DLinkedList
  include/DLinkedList.hpp
  src/DLinkedList.cpp
  )
# -----------------------------------------------------------------------------

# ------------------------------ Build and Include ----------------------------
target_include_directories(DLinkedList PUBLIC include/)
add_executable(Routes src/main.cpp)
target_link_libraries(Routes 
  PRIVATE DLinkedList
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component
  PRIVATE nlohmann_json::nlohmann_json
  )
# -----------------------------------------------------------------------------

set_target_properties(Routes PROPERTIES CXX_STANDARD 17)
