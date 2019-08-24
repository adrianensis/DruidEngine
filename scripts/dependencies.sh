#! /bin/bash

tmpDir=".tmp"
destiny="dependencies"

projectDir="$PWD"

# ------------------------------------------------------------------------

rm -R $destiny 2> /dev/null

# ------------------------------------------------------------------------

sudo apt-get update
sudo apt-get -y install build-essential wget unzip cmake cmake-data python-pip libglu1-mesa-dev freeglut3-dev mesa-common-dev xorg-dev libsoil-dev doxygen graphviz

# ------------------------------------------------------------------------

# GLFW
wget -P $tmpDir https://github.com/glfw/glfw/releases/download/3.2.1/glfw-3.2.1.zip
unzip $tmpDir/glfw-3.2.1.zip -d $destiny

# ------------------------------------------------------------------------

# SOIL
wget -P $tmpDir www.lonesock.net/files/soil.zip
unzip $tmpDir/soil.zip -d $destiny

mv "$destiny/Simple OpenGL Image Library" $destiny/SOIL

# cd $destiny/SOIL/projects/makefile

# mkdir "obj"
# make
# sudo make install

# cd $projectDir

# ------------------------------------------------------------------------

# glad
pip install --user glad

# generate glad source
python -m glad --generator=c --extensions=GL_EXT_framebuffer_multisample,GL_EXT_texture_filter_anisotropic --out-path=$destiny/glad

# ------------------------------------------------------------------------

rm -R $tmpDir 2> /dev/null

# ------------------------------------------------------------------------
