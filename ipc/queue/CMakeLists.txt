cmake_minimum_required(VERSION 3.10)

project(queue)

add_executable(sender sender.c)
target_link_libraries(sender rt)

add_executable(receiver receiver.c)
target_link_libraries(receiver rt)