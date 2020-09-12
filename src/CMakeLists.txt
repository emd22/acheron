cmake_minimum_required(VERSION 3.5)

set(BUILD_NAME acheron)

project(${BUILD_NAME} C)

find_package(Threads REQUIRED)

set(DEBUG FALSE)
set(CMAKE_CXX_COMPILER gcc)

if (${DEBUG})
    # dynamic is used to place symbol names into exectuable for
    # internal backtrace
    set(LINKER_DEBUG_FLAGS "-rdynamic")
    set(C_DEBUG_FLAGS "-g -DAR_DEBUG -DAR_MEMORY_DEBUG")
    # link with dl for access to linux dlfcn library
    set(DEBUG_LIBARIES "dl")
else()
    set(LINKER_DEBUG_FLAGS "")
    set(C_DEBUG_FLAGS "-DBACKTRACE_NO_FUNCTION_NAMES")
    set(DEBUG_LIBARIES "")
endif()

set(C_WARNING_FLAGS "-Wall -Wextra -Wpedantic -Werror")
set(C_FLAGS "${C_DEBUG_FLAGS} -O2 -std=c99 ${C_WARNING_FLAGS} -Wno-implicit-fallthrough -DAR_USE_OPENGL")
set(LINKER_FLAGS "-m64 ${LINKER_DEBUG_FLAGS}")

file(GLOB_RECURSE SOURCES "${BUILD_NAME}/*.c")
file(GLOB_RECURSE HEADERS "../include/*.h")

include_directories(
  ${CMAKE_CURRENT_LIST_DIR}/../include
)
link_directories(
  ${CMAKE_CURRENT_LIST_DIR}/../lib
)

add_executable(${BUILD_NAME} ${SOURCES} ${HEADERS})
target_link_libraries(${BUILD_NAME} ${DEBUG_LIBARIES} ar_image GLEW GL SDL2 m Threads::Threads)
set_target_properties(${BUILD_NAME} PROPERTIES COMPILE_FLAGS ${C_FLAGS} LINK_FLAGS ${LINKER_FLAGS})
