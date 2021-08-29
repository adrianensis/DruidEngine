#! /bin/bash

currentDir=$(dirname "$BASH_SOURCE")"/.."
cd $currentDir

rm -R node_modules 2> /dev/null
