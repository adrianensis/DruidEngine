#! /bin/bash

destiny="dependencies"

projectDir="$PWD"

# ------------------------------------------------------------------------

rm -R $destiny 2> /dev/null

# ------------------------------------------------------------------------

sudo apt-get -y update
sudo apt-get -y install build-essential wget unzip cmake clang cmake-data libglu1-mesa-dev freeglut3-dev mesa-common-dev xorg-dev doxygen graphviz

# ------------------------------------------------------------------------

# GLFW
unzip scripts/dependencies-zip/glfw-3.2.1.zip -d $destiny

# ------------------------------------------------------------------------

# SOIL
unzip scripts/dependencies-zip/soil.zip -d $destiny
mv "$destiny/soil-master" $destiny/SOIL

# ------------------------------------------------------------------------

# glad
unzip scripts/dependencies-zip/glad.zip -d $destiny/glad

# ------------------------------------------------------------------------

# JSON https://github.com/nlohmann/json
unzip scripts/dependencies-zip/json-3.9.1.zip -d $destiny

# ------------------------------------------------------------------------