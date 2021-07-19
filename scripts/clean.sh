#! /bin/bash

currentDir=$(dirname "$BASH_SOURCE")"/.."
cd $currentDir

rm -R binaries build 2> /dev/null
rm -R generated-code 2> /dev/null