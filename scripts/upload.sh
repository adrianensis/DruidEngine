#! /bin/bash

currentDir=$(dirname "$BASH_SOURCE")"/.."
cd $currentDir

git add -A

git commit -m "$1"

git push
