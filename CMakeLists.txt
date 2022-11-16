cmake_minimum_required(VERSION 3.15)
project(program6)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(program6
        HooFSRPCClient.cpp
        HooFSRPCClient.h
        HooFSRPCServer.c
        main.cpp
        server.h)
