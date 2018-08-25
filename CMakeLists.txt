cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

# project name
set(DRUID druid)
project(${DRUID})

# We need c++14 because it defines the auto type.

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D DE_DEBUG -std=c++14 -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -std=c++14 -Wall -O3 -ffast-math -mavx")

message("Compiler: ${CMAKE_CXX_COMPILER}")
message("Release: ${CMAKE_CXX_FLAGS_RELEASE}")
message("Debug: ${CMAKE_CXX_FLAGS_DEBUG}")

message("Selected Build target: ${CMAKE_BUILD_TYPE}")

# output directories
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/test_output)

# packages
file(GLOB PACKAGES_PATHS "source/*")
set(PACKAGES "")

foreach(PACKAGE_PATH ${PACKAGES_PATHS})
  get_filename_component(PACKAGE_NAME ${PACKAGE_PATH} NAME_WE) # NAME_WE = name with extension
  list(APPEND PACKAGES ${PACKAGE_NAME})
endforeach()

# include directory

file(GLOB INCLUDE_PATHS "include/*")

foreach(INCLUDE_PATH ${INCLUDE_PATHS})
  get_filename_component(INCLUDE_NAME ${INCLUDE_PATH} NAME_WE) # NAME_WE = name with extension
  include_directories("include/${INCLUDE_NAME}")
endforeach()

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
add_library(${DRUID} "include/Druid/Druid.h" ${OBJS} )

file(GLOB TEST_PATHS "test/*")

foreach(TEST_PATH ${TEST_PATHS})

	file(GLOB TEST_SOURCES "${TEST_PATH}/*.cpp")

	foreach(TEST_PATH ${TEST_SOURCES})

	  get_filename_component(TEST_NAME ${TEST_PATH} NAME_WE) # NAME_WE = name with extension

	  set(TEST_FINAL_NAME "test_${DRUID}_${TEST_NAME}" )

	  add_executable( ${TEST_FINAL_NAME} ${TEST_SOURCES} )
	  target_link_libraries( ${TEST_FINAL_NAME} ${DRUID} )
	  install(TARGETS ${TEST_FINAL_NAME} DESTINATION ${EXECUTABLE_OUTPUT_PATH})
	endforeach()

endforeach()
