cmake_minimum_required(VERSION 3.5)

project(socket)

add_executable(${PROJECT_NAME}_server server.c)
add_executable(${PROJECT_NAME}_client client.c)
