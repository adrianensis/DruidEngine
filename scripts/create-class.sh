#! /bin/bash

# REMEMBER $1 is the option itself : -c, -h, ...

className=$2
# outputDir=$2

headerFile="$className.h"
sourceFile="$className.cpp"

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

# cd $outputDir

set -f # it disables the * wildcard expansion for multiline comments

while getopts ":cmr" opt; do
  case $opt in
    h)
      echo
      echo "Options"
      echo
      # echo "-h Show help."
      # echo "-r Compile Release, Debug by default."
      # echo "-t Compile Tests suite."
      # echo "-c Clean the project."
      echo
      exit
      ;;
    c)
      touch $headerFile
      touch $sourceFile
      appendHeader "#ifndef DE_${className^^}_H"
      appendHeader "#define DE_${className^^}_H"
      appendHeader
      appendHeader '#include "DE_Class.h"'
      appendHeader
      appendHeader "namespace DE {"
      appendHeader
      appendHeader "class $className : public DE_Class{"
      appendHeader "// properties"
      appendHeader "private:"
      appendHeader
      appendHeader "public:"
      appendHeader
      appendHeader "  DE_CLASS($className, DE_Class);"
      appendHeader "// methods"
      appendHeader "};"
      appendHeader
      appendHeader "} /* namespace DE */"
      appendHeader
      appendHeader "#endif /* DE_${className^^}_H */"

      exit
      ;;
    r)
      rm $headerFile
      rm $sourceFile
      exit
      ;;
    m)

      sed -E -i '/\/\/ methods/a Hello World' $headerFile


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





#
#
# appendSource '#include "'$className'.h"'
# appendSource '#include "Debug.h"'
# appendSource
# appendSource '#include "GameObject.h"'
# appendSource '#include "Transform.h"'
# appendSource '#include "Vector3.h"'
# appendSource '#include "Time.h"'
# appendSource
# appendSource "namespace DE {"
# appendSource
# appendSource "// ---------------------------------------------------------------------------"
# appendSource
# appendSource "$className::$className() : Script(){"
# appendSource
# appendSource "}"
# appendSource
# appendSource "// ---------------------------------------------------------------------------"
# appendSource
# appendSource "$className::~$className() = default;"
# appendSource
# appendSource "// ---------------------------------------------------------------------------"
# appendSource
# appendSource "void $className::init(){"
# appendSource
# appendSource "}"
# appendSource
# appendSource "// ---------------------------------------------------------------------------"
# appendSource
# appendSource "void $className::step(){"
# appendSource
# appendSource "}"
# appendSource
# appendSource "// ---------------------------------------------------------------------------"
# appendSource
# appendSource "void $className::terminate() {"
# appendSource
# appendSource "}"
# appendSource
# appendSource "// ---------------------------------------------------------------------------"
# appendSource
# appendSource "} /* namespace DE */"
