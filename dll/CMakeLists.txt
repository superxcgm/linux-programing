cmake_minimum_required(VERSION 3.5)

project(dll)

# set(SOURCES
#  src/application.c
#  src/dll.c
#  src/dll_utils.c
#)

file(GLOB SOURCES "src/*.c")

add_executable(exe ${SOURCES})

target_include_directories(exe
  PRIVATE ${PROJECT_SOURCE_DIR}/include
)
