cmake_minimum_required(VERSION 3.10)

project(socket)

add_executable(unix_domain_server unix_domain/server_tcp.c)
add_executable(unix_domain_server_multiplexing unix_domain/server_multiplexing_tcp.c)
add_executable(unix_domain_client unix_domain/client_tcp.c)

add_executable(rtm unix_domain/sync/route_manager_server.c unix_domain/sync/common.c)
target_link_libraries(rtm pthread)

add_executable(rtm_client unix_domain/sync/client.c unix_domain/sync/common.c)
