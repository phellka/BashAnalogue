cmake_minimum_required(VERSION 3.10)

project(MyProject C)

set(CMAKE_C_STANDARD 11)

file(GLOB_RECURSE SOURCES "${CMAKE_SOURCE_DIR}/*.c")
list(FILTER SOURCES EXCLUDE REGEX ".*/CMakeFiles/.*")

file(GLOB_RECURSE HEADERS "${CMAKE_SOURCE_DIR}/*.h")
list(FILTER HEADERS EXCLUDE REGEX ".*/CMakeFiles/.*")

find_library(READLINE_LIBRARY NAMES readline)

add_executable(main ${SOURCES} ${HEADERS})

target_link_libraries(main PRIVATE ${READLINE_LIBRARY})