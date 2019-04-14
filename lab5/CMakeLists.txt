cmake_minimum_required(VERSION 3.13)
project(lab5)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_STANDARD 14)

set(CMAKE_CXX_FLAGS
    ${CMAKE_CXX_FLAGS}
    -Wno-int-to-pointer-cast
)

include_directories("D:\\projects\\ui\\lab5\\db")
link_directories("D:\\projects\\ui\\lab5\\db")

add_executable(lab5 main.cpp DbHandler.cpp DbHandler.h)
target_link_libraries(lab5
    sqlite3.dll
    comctl32
)