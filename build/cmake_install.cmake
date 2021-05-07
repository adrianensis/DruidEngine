# Install script for directory: /home/adri/Dropbox/Projects/CPP/NewEngine

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}/home/adri/Dropbox/Projects/CPP/NewEngine/binaries/test_integration_mage" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/home/adri/Dropbox/Projects/CPP/NewEngine/binaries/test_integration_mage")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}/home/adri/Dropbox/Projects/CPP/NewEngine/binaries/test_integration_mage"
         RPATH "")
  endif()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/home/adri/Dropbox/Projects/CPP/NewEngine/binaries/test_integration_mage")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/home/adri/Dropbox/Projects/CPP/NewEngine/binaries" TYPE EXECUTABLE FILES "/home/adri/Dropbox/Projects/CPP/NewEngine/binaries/test_integration_mage")
  if(EXISTS "$ENV{DESTDIR}/home/adri/Dropbox/Projects/CPP/NewEngine/binaries/test_integration_mage" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/home/adri/Dropbox/Projects/CPP/NewEngine/binaries/test_integration_mage")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}/home/adri/Dropbox/Projects/CPP/NewEngine/binaries/test_integration_mage")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}/home/adri/Dropbox/Projects/CPP/NewEngine/binaries/test_integration_playground" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/home/adri/Dropbox/Projects/CPP/NewEngine/binaries/test_integration_playground")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}/home/adri/Dropbox/Projects/CPP/NewEngine/binaries/test_integration_playground"
         RPATH "")
  endif()
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/home/adri/Dropbox/Projects/CPP/NewEngine/binaries/test_integration_playground")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/home/adri/Dropbox/Projects/CPP/NewEngine/binaries" TYPE EXECUTABLE FILES "/home/adri/Dropbox/Projects/CPP/NewEngine/binaries/test_integration_playground")
  if(EXISTS "$ENV{DESTDIR}/home/adri/Dropbox/Projects/CPP/NewEngine/binaries/test_integration_playground" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/home/adri/Dropbox/Projects/CPP/NewEngine/binaries/test_integration_playground")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}/home/adri/Dropbox/Projects/CPP/NewEngine/binaries/test_integration_playground")
    endif()
  endif()
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/adri/Dropbox/Projects/CPP/NewEngine/build/dependencies/glfw-3.2.1/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/home/adri/Dropbox/Projects/CPP/NewEngine/build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
