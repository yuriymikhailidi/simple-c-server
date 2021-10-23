cmake_minimum_required(VERSION 3.20)
project(server)

set(CMAKE_CXX_STANDARD 11)

add_executable(server httpechosrv.c Handler.c)
