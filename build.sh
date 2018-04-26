#! /bin/bash

d=$1

if [ "$#" -eq 0 ]
then
  d="Debug"
fi

# sh clean.sh

mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=$d ..

make -j4
