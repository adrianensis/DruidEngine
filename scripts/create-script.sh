#! /bin/bash

scriptClassName=$1
outputDir=$2

headerFile="$scriptClassName.h"
sourceFile="$scriptClassName.cpp"

# Funtions

appendHeader()
{
  str=$1
  echo $str >> $headerFile
}

appendSource()
{
  str=$1
  echo $str >> $sourceFile
}

# main

cd $outputDir

rm $headerFile
rm $sourceFile

touch $headerFile
touch $sourceFile

set -f # it disables the * wildcard expansion for multiline comments

appendHeader "#ifndef DE_${scriptClassName^^}_H"
appendHeader "#define DE_${scriptClassName^^}_H"
appendHeader
appendHeader '#include "Script.h"'
appendHeader
appendHeader "namespace DE {"
appendHeader
appendHeader "class $scriptClassName : public Script{"
appendHeader "private:"
appendHeader
appendHeader "public:"
appendHeader
appendHeader "  DE_CLASS($scriptClassName, Script);"
appendHeader
appendHeader "  void init() override;"
appendHeader "  void step() override;"
appendHeader "  void terminate() override;"
appendHeader "};"
appendHeader
appendHeader "} /* namespace DE */"
appendHeader
appendHeader "#endif /* DE_${scriptClassName^^}_H */"

appendSource '#include "'$scriptClassName'.h"'
appendSource '#include "Log.h"'
appendSource
appendSource '#include "GameObject.h"'
appendSource '#include "Transform.h"'
appendSource '#include "Vector3.h"'
appendSource '#include "Time.h"'
appendSource
appendSource "namespace DE {"
appendSource
appendSource "// ---------------------------------------------------------------------------"
appendSource
appendSource "$scriptClassName::$scriptClassName() : Script(){"
appendSource
appendSource "}"
appendSource
appendSource "// ---------------------------------------------------------------------------"
appendSource
appendSource "$scriptClassName::~$scriptClassName() = default;"
appendSource
appendSource "// ---------------------------------------------------------------------------"
appendSource
appendSource "void $scriptClassName::init(){"
appendSource
appendSource "}"
appendSource
appendSource "// ---------------------------------------------------------------------------"
appendSource
appendSource "void $scriptClassName::step(){"
appendSource
appendSource "}"
appendSource
appendSource "// ---------------------------------------------------------------------------"
appendSource
appendSource "void $scriptClassName::terminate() {"
appendSource
appendSource "}"
appendSource
appendSource "// ---------------------------------------------------------------------------"
appendSource
appendSource "} /* namespace DE */"
