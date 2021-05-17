# ----------------------------------------------------------------------------
# DRUID ENGINE
# ----------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

# project name
set(DRUID druid)
project(${DRUID})

# ----------------------------------------------------------------------------
# CPP TARGETS
# ----------------------------------------------------------------------------
# We need c++14 because it defines the auto type.

# set(CMAKE_CXX_FLAGS_ALL "-std=c++17 -Wall") # this macro was invented by me

if(ENABLE_LOGS)
  set(CMAKE_CXX_FLAGS_ALL "${CMAKE_CXX_FLAGS_ALL} -D DE_ENABLE_LOGS")
endif(ENABLE_LOGS)

if(UNIX)
  set(CMAKE_CXX_FLAGS_ALL "${CMAKE_CXX_FLAGS_ALL} -std=c++17 ") # this macro was invented by me
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${CMAKE_CXX_FLAGS_ALL} -D DE_DEBUG -ffast-math -mavx")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${CMAKE_CXX_FLAGS_ALL} -O3 -ffast-math -mavx")
  #set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${CMAKE_CXX_FLAGS_ALL} -Ofast -ffast-math -mavx")
endif(UNIX)

if(WIN32)
  set(CMAKE_CXX_FLAGS_ALL "${CMAKE_CXX_FLAGS_ALL} /m /TP /std:c++17") # this macro was invented by me
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${CMAKE_CXX_FLAGS_ALL} -D DE_DEBUG")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${CMAKE_CXX_FLAGS_ALL}")
endif(WIN32)

# ----------------------------------------------------------------------------
# OUTPUT DIRECTORIES
# ----------------------------------------------------------------------------

set(LIBRARY_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/compiledLibs")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/binaries")

# ----------------------------------------------------------------------------

message("Compiler: ${CMAKE_CXX_COMPILER}")
message("Release: ${CMAKE_CXX_FLAGS_RELEASE}")
message("Debug: ${CMAKE_CXX_FLAGS_DEBUG}")

message("Selected Build target: ${CMAKE_BUILD_TYPE}")

# ----------------------------------------------------------------------------
# DEPENDENCIES
# ----------------------------------------------------------------------------

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

# SOIL
include_directories("${DEPENDENCIES_DIR}/SOIL/inc")
#link_directories("${DEPENDENCIES_DIR}/SOIL/lib")
file(GLOB SOIL_SOURCE "${DEPENDENCIES_DIR}/SOIL/src/*.c")
# file(GLOB_RECURSE SOIL_CPP_SOURCE "${DEPENDENCIES_DIR}/SOIL/src/*.cpp")
# message(${SOIL_SOURCE})

# JSON
set(JSON_BuildTests OFF CACHE INTERNAL "")
add_subdirectory("${DEPENDENCIES_DIR}/json-3.9.1")

# Pyhon
find_package (Python3 COMPONENTS Interpreter Development)
message("Python3 found: ${Python3_FOUND}")
include_directories(${Python3_INCLUDE_DIRS})
message(${Python3_INCLUDE_DIRS})
message(${Python3_LIBRARIES})

# ----------------------------------------------------------------------------
# ENGINE PACKAGES
# ----------------------------------------------------------------------------

file(GLOB PACKAGES_PATHS "code/*")
set(PACKAGES "")

foreach(PACKAGE_PATH ${PACKAGES_PATHS})
  get_filename_component(PACKAGE_NAME ${PACKAGE_PATH} NAME_WE) # NAME_WE = name with extension
  list(APPEND PACKAGES ${PACKAGE_NAME})
endforeach()

# ----------------------------------------------------------------------------
# PACKAGES INCLUDE DIRECTORIES
# ----------------------------------------------------------------------------

include_directories("code")

# file(GLOB INCLUDE_PATHS "code/*")
#
# foreach(INCLUDE_PATH ${INCLUDE_PATHS})
#   get_filename_component(INCLUDE_NAME ${INCLUDE_PATH} NAME_WE) # NAME_WE = name with extension
#   include_directories("code/${INCLUDE_NAME}")
# endforeach()

# ----------------------------------------------------------------------------
# LIBRARY OBJECTS
# ----------------------------------------------------------------------------

# object libraries
# Alternative: CMakeLists.txt with this code into source directory.
# add_subdirectory(source)
foreach(PACKAGE ${PACKAGES})
  file(GLOB_RECURSE PACKAGE_SOURCE "code/${PACKAGE}/*")
  add_library(${PACKAGE} OBJECT ${PACKAGE_SOURCE})
endforeach()

# list of objects
foreach(PACKAGE ${PACKAGES})
  list(APPEND OBJS $<TARGET_OBJECTS:${PACKAGE}>)
endforeach()

# ----------------------------------------------------------------------------
# DRUID ENGINE LIBRARY
# ----------------------------------------------------------------------------

# single library
add_library(${DRUID} ${OBJS} ${GLAD_SOURCE} ${SOIL_SOURCE})

# ----------------------------------------------------------------------------
# LINK LIBRARIES
# ----------------------------------------------------------------------------

# link libraries with druid library
target_link_libraries( ${DRUID} ${OPENGL_gl_LIBRARY})
target_link_libraries( ${DRUID} ${OPENGL_glu_LIBRARY})
target_link_libraries( ${DRUID} glfw)
target_link_libraries( ${DRUID} nlohmann_json)
target_link_libraries( ${DRUID} ${Python3_LIBRARIES})

#target_link_libraries( ${DRUID} "stdc++fs")

# SOIL
#target_link_libraries( ${DRUID} SOIL )

# ----------------------------------------------------------------------------
# TESTS
# ----------------------------------------------------------------------------

if(BUILD_UNIT_TESTS)

# UNIT TESTS

file(GLOB TEST_PATHS "test/unit/*")

foreach(TEST_PATH ${TEST_PATHS})

	file(GLOB_RECURSE TEST_SOURCES "${TEST_PATH}/*.cpp")

	foreach(TEST_PATH ${TEST_SOURCES})

	  get_filename_component(TEST_NAME ${TEST_PATH} NAME_WE) # NAME_WE = name with extension

	  set(TEST_FINAL_NAME "test_unit_${TEST_NAME}" )
	  add_executable( ${TEST_FINAL_NAME} ${TEST_SOURCES} )
	  target_link_libraries( ${TEST_FINAL_NAME} ${DRUID})
	  install(TARGETS ${TEST_FINAL_NAME} DESTINATION ${EXECUTABLE_OUTPUT_PATH})
	endforeach()

endforeach()

endif(BUILD_UNIT_TESTS)

# ----------------------------------------------------------------------------

if(BUILD_INTEGRATION_TESTS)

# INTEGRATION TESTS

file(GLOB TEST_PATHS "test/integration/*")

foreach(TEST_PATH ${TEST_PATHS})

	file(GLOB_RECURSE TEST_SOURCES "${TEST_PATH}/*.cpp" "${TEST_PATH}/*.hpp")

  include_directories("${TEST_PATH}")

  get_filename_component(TEST_NAME ${TEST_PATH} NAME_WE) # NAME_WE = name with extension

  set(TEST_FINAL_NAME "test_integration_${TEST_NAME}" )
  add_executable( ${TEST_FINAL_NAME} ${TEST_SOURCES} )
  target_link_libraries( ${TEST_FINAL_NAME} ${DRUID})
  install(TARGETS ${TEST_FINAL_NAME} DESTINATION ${EXECUTABLE_OUTPUT_PATH})

endforeach()

endif(BUILD_INTEGRATION_TESTS)

# ----------------------------------------------------------------------------
# TOOLS
# ----------------------------------------------------------------------------

if(BUILD_TOOLS)

file(GLOB TOOL_PATHS "tools/*")

foreach(TOOL_PATH ${TOOL_PATHS})

  file(GLOB_RECURSE TOOL_SOURCES "${TOOL_PATH}/*.cpp" "${TOOL_PATH}/*.hpp")

  include_directories("${TOOL_PATH}")

  get_filename_component(TOOL_NAME ${TOOL_PATH} NAME_WE) # NAME_WE = name with extension

  set(TOOL_FINAL_NAME "tool_${TOOL_NAME}" )
  add_executable( ${TOOL_FINAL_NAME} ${TOOL_SOURCES} )
  target_link_libraries( ${TOOL_FINAL_NAME} ${DRUID})
  install(TARGETS ${TOOL_FINAL_NAME} DESTINATION ${EXECUTABLE_OUTPUT_PATH})

endforeach()

endif(BUILD_TOOLS)
