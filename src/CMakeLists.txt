cmake_minimum_required(VERSION 3.5)

set(BUILD_NAME gl)

project(${BUILD_NAME} C)

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_CXX_COMPILER gcc)
set(CXX_FLAGS "-O2 -Wall -Wextra -Wpedantic -Werror -std=c99")

file(GLOB_RECURSE SOURCES "${BUILD_NAME}/*.c")
file(GLOB_RECURSE HEADERS "../include/*.h")

include_directories(
  ${CMAKE_CURRENT_LIST_DIR}/../include
)
add_executable(${BUILD_NAME} ${SOURCES} ${HEADERS})
target_link_libraries(${BUILD_NAME} GLEW GL SDL2 m ${CMAKE_THREAD_LIBS_INIT})
set_target_properties(${BUILD_NAME} PROPERTIES COMPILE_FLAGS "${CXX_FLAGS} -m64" LINK_FLAGS "-m64")
add_definitions(${GCC_COVERAGE_COMPILE_FLAGS})
