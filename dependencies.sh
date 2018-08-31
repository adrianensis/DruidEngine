#! /bin/bash

sudo apt-get update
sudo apt-get install build-essential cmake parallel libglu1-mesa-dev freeglut3-dev mesa-common-dev doxygen graphviz

sudo apt-get install wget unzip

tmpDir=".tmp"

wget -P $tmpDir https://github.com/glfw/glfw/releases/download/3.2.1/glfw-3.2.1.zip

unzip $tmpDir/glfw-3.2.1.zip -d libs

rm -R $tmpDir
