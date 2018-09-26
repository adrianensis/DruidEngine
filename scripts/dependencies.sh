#! /bin/bash

tmpDir=".tmp"
destiny="dependencies"

rm -R $destiny 2> /dev/null

sudo apt-get update
sudo apt-get install build-essential cmake python-pip libglu1-mesa-dev freeglut3-dev mesa-common-dev xorg-dev doxygen graphviz
sudo apt-get install wget unzip

# GLFW
wget -P $tmpDir https://github.com/glfw/glfw/releases/download/3.2.1/glfw-3.2.1.zip
unzip $tmpDir/glfw-3.2.1.zip -d $destiny

# glad
pip install --user glad

# generate glad source
python -m glad --generator=c --extensions=GL_EXT_framebuffer_multisample,GL_EXT_texture_filter_anisotropic --out-path=$destiny/glad

rm -R $tmpDir 2> /dev/null
