@echo off

SET buildDir="build"
SET buildType="Debug"
SET buildUnitTests=false
SET buildIntegrationTests=false
SET buildTools=false
SET enableLogs=false

mkdir %buildDir%
cd %buildDir%

cmake -DCMAKE_BUILD_TYPE=$buildType -DBUILD_UNIT_TESTS=$buildUnitTests -DBUILD_INTEGRATION_TESTS=$buildIntegrationTests -DBUILD_TOOLS=$buildTools -DENABLE_LOGS=$enableLogs ..

"C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\MSBuild\15.0\Bin\MSBuild.exe" ALL_BUILD.vcxproj /p:configuration=debug