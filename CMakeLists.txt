cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

# project name
set(DRUID druid)
project(${DRUID})

# We need c++14 because it defines the auto type.

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D DE_DEBUG -std=c++14 -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -std=c++14 -Wall -O3 -ffast-math -mavx")

# output directories
set(LIBRARY_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/libOutput")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/testOutput")

message("Compiler: ${CMAKE_CXX_COMPILER}")
message("Release: ${CMAKE_CXX_FLAGS_RELEASE}")
message("Debug: ${CMAKE_CXX_FLAGS_DEBUG}")

message("Selected Build target: ${CMAKE_BUILD_TYPE}")

# third party libraries
set(DEPENDENCIES_DIR "dependencies")

# OpenGL
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE) # see : http://www.glfw.org/docs/latest/build_guide.html
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory("${DEPENDENCIES_DIR}/glfw-3.2.1")
include_directories("${DEPENDENCIES_DIR}/glfw-3.2.1/include")

# glad
set(GLAD_SOURCE "${DEPENDENCIES_DIR}/glad/src/glad.c")
include_directories("${DEPENDENCIES_DIR}/glad/include")

set(GLAD_SOURCE "${DEPENDENCIES_DIR}/glad/src/glad.c")
include_directories("${DEPENDENCIES_DIR}/glad/include")

# packages
file(GLOB PACKAGES_PATHS "code/source/*")
set(PACKAGES "")

foreach(PACKAGE_PATH ${PACKAGES_PATHS})
  get_filename_component(PACKAGE_NAME ${PACKAGE_PATH} NAME_WE) # NAME_WE = name with extension
  list(APPEND PACKAGES ${PACKAGE_NAME})
endforeach()

# include directory

file(GLOB INCLUDE_PATHS "code/include/*")

foreach(INCLUDE_PATH ${INCLUDE_PATHS})
  get_filename_component(INCLUDE_NAME ${INCLUDE_PATH} NAME_WE) # NAME_WE = name with extension
  include_directories("code/include/${INCLUDE_NAME}")
endforeach()

# object libraries
# Alternative: CMakeLists.txt with this code into source directory.
# add_subdirectory(source)
foreach(PACKAGE ${PACKAGES})
  file(GLOB PACKAGE_SOURCE "code/source/${PACKAGE}/*.cpp")
  add_library(${PACKAGE} OBJECT ${PACKAGE_SOURCE})
endforeach()

# list of objects
foreach(PACKAGE ${PACKAGES})
  list(APPEND OBJS $<TARGET_OBJECTS:${PACKAGE}>)
endforeach()

# single library
add_library(${DRUID} "code/include/Druid/Druid.h" ${OBJS} ${GLAD_SOURCE})

# link libraries with druid library
target_link_libraries( ${DRUID} ${OPENGL_gl_LIBRARY})
target_link_libraries( ${DRUID} ${OPENGL_glu_LIBRARY})
target_link_libraries( ${DRUID} glfw)
# SOIL
target_link_libraries( ${DRUID} SOIL )


# compile tests

if(BUILD_TESTS)

# unit tests

file(GLOB TEST_PATHS "test/unit/*")

foreach(TEST_PATH ${TEST_PATHS})

	file(GLOB TEST_SOURCES "${TEST_PATH}/*.cpp")

	foreach(TEST_PATH ${TEST_SOURCES})

	  get_filename_component(TEST_NAME ${TEST_PATH} NAME_WE) # NAME_WE = name with extension

	  set(TEST_FINAL_NAME "test_unit_${TEST_NAME}" )
	  add_executable( ${TEST_FINAL_NAME} ${TEST_SOURCES} )
	  target_link_libraries( ${TEST_FINAL_NAME} ${DRUID})
	  install(TARGETS ${TEST_FINAL_NAME} DESTINATION ${EXECUTABLE_OUTPUT_PATH})
	endforeach()

endforeach()

# integration tests

file(GLOB TEST_PATHS "test/integration/*")

foreach(TEST_PATH ${TEST_PATHS})

	file(GLOB TEST_SOURCES "${TEST_PATH}/*.cpp")

	foreach(TEST_PATH ${TEST_SOURCES})

	  get_filename_component(TEST_NAME ${TEST_PATH} NAME_WE) # NAME_WE = name with extension

	  set(TEST_FINAL_NAME "test_integration_${TEST_NAME}" )
	  add_executable( ${TEST_FINAL_NAME} ${TEST_SOURCES} )
	  target_link_libraries( ${TEST_FINAL_NAME} ${DRUID})
	  install(TARGETS ${TEST_FINAL_NAME} DESTINATION ${EXECUTABLE_OUTPUT_PATH})
	endforeach()

endforeach()

endif(BUILD_TESTS)
