cmake_minimum_required(VERSION 3.5)

project(p_dll)

set(SOURCES
  src/application.c
  # src/dll.c
  # src/dll_utils.c
)

# file(GLOB SOURCES "src/*.c")

add_library(dll STATIC
  src/dll.c
  src/dll_utils.c
)

target_include_directories(dll
  PUBLIC ${PROJECT_SOURCE_DIR}/include
)

add_executable(exe ${SOURCES})

target_link_libraries(exe
  PRIVATE dll
)
