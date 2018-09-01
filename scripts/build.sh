#! /bin/bash

buildDir="build"
d=$1

if [ "$#" -eq 0 ]
then
  d="Debug"
fi

# sh clean.sh

mkdir $buildDir 2> /dev/null
cd $buildDir 2> /dev/null
cmake -DCMAKE_BUILD_TYPE=$d .. # Debug or Release

make -j4
