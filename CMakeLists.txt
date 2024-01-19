cmake_minimum_required(VERSION 3.15)
project(csapp)

if (MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# set(CSAPP_PUBLIC_HEADER ${CMAKE_SOURCE_DIR}/include)
set(CSAPP_LIB csapp_lib)

add_subdirectory(external/csapp)
add_subdirectory(problems)