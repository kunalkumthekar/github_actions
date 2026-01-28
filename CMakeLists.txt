#####################################
# Project
#####################################
cmake_minimum_required(VERSION 3.13)
set(TARGET_NAME "BIZ")
set(TEST_TARGET "BIZ_test")
project(${TARGET_NAME}
    VERSION 1.0
    DESCRIPTION "Trying out Github Actions"
    LANGUAGES CXX)

#####################################
# Packages
#####################################
# Setting Test cases path for later use
set(TEST_CASES_PATH ${CMAKE_CURRENT_SOURCE_DIR}/tests/)

#####################################
# Sources (src)
#####################################
file(GLOB SOURCES ${CMAKE_SOURCE_DIR}/src/*.cpp)
# if multiple sources set it under one variable
add_library(${TARGET_NAME} ${SOURCES})

#####################################
# Includes
#####################################
target_include_directories(${TARGET_NAME}
    PUBLIC ${CMAKE_SOURCE_DIR}/src/
    PUBLIC ${TEST_CASES_PATH})

#####################################
# Libraries
#####################################

#####################################
# Target properties
#####################################
add_definitions("-DBOOST_ALLOW_DEPRECATED_HEADERS")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
target_compile_options(${TARGET_NAME} PRIVATE -Wall -Wpedantic)
set_target_properties(${TARGET_NAME}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

#####################################
# Test
#####################################
file(GLOB SOURCE_TEST ${TEST_CASES_PATH}/*.cpp)

add_executable(${TEST_TARGET} ${SOURCE_TEST} ${GTEST_MAIN_LIBRARIES})

find_package(GTest REQUIRED)

target_include_directories(${TEST_TARGET}
    PRIVATE ${GTEST_INCLUDE_DIRS})

target_link_libraries(${TEST_TARGET}
    PRIVATE ${GTEST_LIBRARIES}
    PRIVATE ${TARGET_NAME})

target_compile_options(${TEST_TARGET} PRIVATE -Wall -Wpedantic)

set_target_properties(${TEST_TARGET}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")