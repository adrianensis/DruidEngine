#! /bin/bash

buildDir="build"
buildType="Debug"
buildUnitTests=false
buildIntegrationTests=false
buildTools=false
enableLogs=false

while getopts ":ruixlch" opt; do
  case $opt in
    h)
      echo
      echo "Options"
      echo
      echo "-h Show help."
      echo "-r Compile Release, Debug by default."
      echo "-u Compile Unit Tests suite."
      echo "-i Compile Integration Tests suite."
      echo "-x Compile Tools."
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
    u)
      buildUnitTests=true
      ;;
    i)
      buildIntegrationTests=true
      ;;
    x)
      buildTools=true
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
# -DCMAKE_C_COMPILER=/usr/bin/clang -DCMAKE_CXX_COMPILER=/usr/bin/clang++
cmake -DCMAKE_BUILD_TYPE=$buildType -DBUILD_UNIT_TESTS=$buildUnitTests -DBUILD_INTEGRATION_TESTS=$buildIntegrationTests -DBUILD_TOOLS=$buildTools -DENABLE_LOGS=$enableLogs ..

make -j8
