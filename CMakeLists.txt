cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

# project name
set(DRUID druid)
project(${DRUID})

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D DE_DEBUG -std=c++14 -O1 -Wall")
#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -O3 -msse4") # this disables asserts with -DNDEBUG
set(CMAKE_CXX_FLAGS_RELEASE "-std=c++14 -Wall -O3 -msse4")

message("Build target: ${CMAKE_BUILD_TYPE}")
message("Release: ${CMAKE_CXX_FLAGS_RELEASE}")
message("Debug: ${CMAKE_CXX_FLAGS_DEBUG}")

# output directories
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

# packages
file(GLOB PACKAGES_PATHS "source/*")
set(PACKAGES "")

foreach(PACKAGE_PATH ${PACKAGES_PATHS})
  get_filename_component(PACKAGE_NAME ${PACKAGE_PATH} NAME_WE) # NAME_WE = name with extension
  list(APPEND PACKAGES ${PACKAGE_NAME})
endforeach()

# include directory
include_directories(include)

# object libraries
# Alternative: CMakeLists.txt with this code into source directory.
# add_subdirectory(source)
foreach(PACKAGE ${PACKAGES})
  file(GLOB PACKAGE_SOURCE "source/${PACKAGE}/*.cpp")
  add_library(${PACKAGE} OBJECT ${PACKAGE_SOURCE})
endforeach()

# list of objects
foreach(PACKAGE ${PACKAGES})
  list(APPEND OBJS $<TARGET_OBJECTS:${PACKAGE}>)
endforeach()

# single library
add_library(${DRUID} "include/Druid.h" ${OBJS} )

file(GLOB TEST_SOURCES "test/*")

foreach(TEST_PATH ${TEST_SOURCES})
  add_subdirectory(${TEST_PATH})
endforeach(TEST_PATH ${TEST_SOURCES})
