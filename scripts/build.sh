#! /bin/bash

buildDir="build"
buildType="Debug"
buildTests=false
enableLogs=false

while getopts ":rtlch" opt; do
  case $opt in
    h)
      echo
      echo "Options"
      echo
      echo "-h Show help."
      echo "-r Compile Release, Debug by default."
      echo "-t Compile Tests suite."
      echo "-l Enable Logs."
      echo "-c Clean the project."
      echo
      exit
      ;;
    c)
      ./scripts/clean.sh
      exit
      ;;
    r)
      buildType="Release"
      ;;
    t)
      buildTests=true
      ;;
    l)
      enableLogs=true
      ;;
    \?)
      echo "Invalid option: -$OPTARG" >&2
      exit 1
      ;;
    :)
      echo "Option -$OPTARG requires an argument." >&2
      exit 1
      ;;
  esac
done

# sh clean.sh

mkdir $buildDir 2> /dev/null
cd $buildDir 2> /dev/null
cmake -DCMAKE_BUILD_TYPE=$buildType -DBUILD_TESTS=$buildTests -DENABLE_LOGS=$enableLogs ..

make -j8
