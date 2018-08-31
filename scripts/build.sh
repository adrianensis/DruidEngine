#! /bin/bash

buildDir="build"
d=$1

if [ "$#" -eq 0 ]
then
  d="Debug"
fi

# sh clean.sh

mkdir $buildDir
cd $buildDir
cmake -DCMAKE_BUILD_TYPE=$d .. # Debug or Release

make -j4
