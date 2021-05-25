#! /bin/bash

currentDir=$(dirname "$BASH_SOURCE")"/.."
cd $currentDir

doxygen
