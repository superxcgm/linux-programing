cmake_minimum_required(VERSION 3.10)

project(socket)

add_executable(unix_domain_server unix_domain/server_tcp.c)
add_executable(unix_domain_client unix_domain/client_tcp.c)