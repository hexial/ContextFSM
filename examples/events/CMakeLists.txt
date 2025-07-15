cmake_minimum_required(VERSION 3.22)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_PROJECT_NAME contextfsm-examples-events)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)


project(${CMAKE_PROJECT_NAME})

include(FetchContent)

FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG v1.15.3
)

FetchContent_MakeAvailable(spdlog)

add_executable(${CMAKE_PROJECT_NAME}
    src/main.cpp
)
target_include_directories(${CMAKE_PROJECT_NAME} 
    PRIVATE ../../src
)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE spdlog::spdlog)

