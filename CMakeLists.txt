cmake_minimum_required(VERSION 3.20.0)
include_directories(include)
project("tinkering" VERSION 0.1 LANGUAGES CXX C)

FILE(GLOB_RECURSE SRCS ${CMAKE_SOURCE_DIR}/src/*.*)

# for threading
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(${PROJECT_NAME} ${SRCS})

target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads) 
