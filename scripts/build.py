import sys
import os
import getopt

cwd = os.path.dirname(os.path.realpath(__file__))
cwd = cwd + "/.."
os.chdir(cwd)
#print(cwd) 
  
buildDir="build"
buildType="Debug"
buildUnitTests=False
buildIntegrationTests=False
buildTools=False
enableLogs=False

argv = sys.argv[1:]

try:
  opts, args = getopt.getopt(argv, ":ruixlch")
except:
  print("Error parsing opts")

for opt, arg in opts:
    if opt in ['-c']:
      os.system('./scripts/clean.sh')
    elif opt in ['-r']:
      buildType="Release"
    elif opt in ['-u']:
      buildUnitTests=True
    elif opt in ['-i']:
      buildIntegrationTests=True
    elif opt in ['-x']:
      buildTools=True
    elif opt in ['-l']:
      enableLogs=True

# -B : prevents from writing .pyc or .pyo files
os.system('python3 -B ./scripts/generateInstanceByName.py')

if not os.path.isdir(buildDir):
      os.mkdir(buildDir)

os.chdir(buildDir)

# -DCMAKE_C_COMPILER=/usr/bin/clang -DCMAKE_CXX_COMPILER=/usr/bin/clang++
buildCommand = 'cmake -DCMAKE_BUILD_TYPE={buildType} -DBUILD_UNIT_TESTS={buildUnitTests} -DBUILD_INTEGRATION_TESTS={buildIntegrationTests} -DBUILD_TOOLS={buildTools} -DENABLE_LOGS={enableLogs} ..'.format(
  buildType = buildType,
  buildUnitTests = buildUnitTests,
  buildIntegrationTests = buildIntegrationTests,
  buildTools = buildTools,
  enableLogs = enableLogs
)
os.system(buildCommand)

os.system('make -j8')