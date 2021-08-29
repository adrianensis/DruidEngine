#! /bin/bash

currentDir=$(dirname "$BASH_SOURCE")"/.."
cd $currentDir

cd $currentDir
electron .

#cd ../../
#./binaries/test_integration_playground &


