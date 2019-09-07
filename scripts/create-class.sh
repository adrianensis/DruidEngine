#! /bin/bash

className=$1
# outputDir=$2

headerFile="$className.h"
sourceFile="$className.cpp"

exitRequest=false


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

# Create files

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
appendHeader "private:"
appendHeader
appendHeader "public:"
appendHeader
appendHeader "  DE_CLASS($className, DE_Class);"
appendHeader


appendSource '#include "'$className'.h"'
appendSource
appendSource "namespace DE {"
appendSource
appendSource "// ---------------------------------------------------------------------------"
appendSource
appendSource "$className::$className() : DE_Class(){"
appendSource
appendSource "}"
appendSource
appendSource "// ---------------------------------------------------------------------------"
appendSource
appendSource "$className::~$className() = default;"
appendSource
appendSource "// ---------------------------------------------------------------------------"
appendSource

# main

# cd $outputDir

set -f # it disables the * wildcard expansion for multiline comments

options="fmq"

while [ $exitRequest = false ]
do
  read -p "Choose an option ($options): "  opt

  # while getopts $options opt; do
    case $opt in
      f)
        read -p "Return type: "  returnType
        read -p "Method name: "  methodName
        read -p "Method params: "  methodParams

        appendHeader "$returnType $methodName($methodParams);"

        appendSource
        appendSource "$returnType $className::$methodName($methodParams){"
        appendSource
        appendSource "}"
        appendSource
        appendSource "// ---------------------------------------------------------------------------"

        # exit
        ;;
      m)
        read -p "Member type: "  memberType
        read -p "Member name: "  memberName

        memberFinalName="m${memberName^}"

        appendHeader "$memberType $memberFinalName;"

        appendHeader "$memberType get${memberName^}() const ;"
        appendHeader "void set${memberName^}($memberType $memberName);"

        appendSource "$memberType $className::get${memberName^}() const { return $memberFinalName; }"
        appendSource "void $className::set${memberName^}($memberType $memberName) { $memberFinalName = $memberName; }"

        # sed -E -i '/\/\/ methods/a '"HHHH" $headerFile

        # exit
        ;;
      q)
        exitRequest=true
      # exit
      ;;
    esac
  # done
done

appendHeader
appendHeader "};"
appendHeader "} /* namespace DE */"
appendHeader "#endif /* DE_${className^^}_H */"

appendSource
appendSource "} /* namespace DE */"
