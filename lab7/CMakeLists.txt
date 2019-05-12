cmake_minimum_required(VERSION 3.14)
project(lab7)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS
    ${CMAKE_CXX_FLAGS}
    -Wno-int-to-pointer-cast
)

file(GLOB lab7_src
    "*.h"
    "*.cpp"
)

include_directories("
  D:\\projects\\ui\\lab7\\db
")
link_directories("D:\\projects\\ui\\lab7\\db")

add_executable(lab7 ${lab7_src})
target_link_libraries(lab7
    comctl32
)