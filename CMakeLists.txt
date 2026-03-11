cmake_minimum_required(VERSION 3.10)
project(IslemYoneticisi)

set(CMAKE_C_STANDARD 99)

add_executable(IslemYoneticisi main.c)

target_link_libraries(IslemYoneticisi psapi)
