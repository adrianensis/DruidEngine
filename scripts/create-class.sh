#! /bin/bash

className=$1
# outputDir=$2

headerFile="$className.h"
sourceFile="$className.cpp"

exitRequest=false


# Functions

appendHeader()
{
  str=$1
  echo $str >> $headerFile
}

appendHeaderProperties()
{
  str=$1
  echo $str >> $headerFile"Properties"
}

appendHeaderMethods()
{
  str=$1
  echo $str >> $headerFile"Methods"
}

appendSource()
{
  str=$1
  echo $str >> $sourceFile
}

appendSourceMethods()
{
  str=$1
  echo $str >> $sourceFile"Methods"
}

appendSourceGettersSetters()
{
  str=$1
  echo $str >> $sourceFile"GettersSetters"
}

# main

# Create files

touch $headerFile
touch $sourceFile
touch $headerFile"Properties"
touch $headerFile"Methods"
touch $sourceFile"Methods"
touch $sourceFile"GettersSetters"

set -f # it disables the * wildcard expansion for multiline comments

options="mpcsq"

while [ $exitRequest = false ]
do
  read -p "Choose an option ($options): "  opt

  # while getopts $options opt; do
    case $opt in
      m) # method
        read -p "Return type: "  returnType
        read -p "Method name: "  methodName
        read -p "Method params: "  methodParams

        appendHeaderMethods "$returnType $methodName($methodParams);"

        appendSourceMethods
        appendSourceMethods "$returnType $className::$methodName($methodParams){"
        appendSourceMethods
        appendSourceMethods "}"
        appendSourceMethods
        appendSourceMethods "// ---------------------------------------------------------------------------"

        ;;
      p) # property
        read -p "Property type: "  propertyType
        read -p "Property name: "  propertyName

        propertyFinalName="m${propertyName^}"

        appendHeaderProperties "$propertyType $propertyFinalName;"

        appendHeaderMethods "$propertyType get${propertyName^}() const ;"
        appendHeaderMethods "void set${propertyName^}($propertyType $propertyName);"

        appendSourceGettersSetters "$propertyType $className::get${propertyName^}() const { return $propertyFinalName; }"
        appendSourceGettersSetters "void $className::set${propertyName^}($propertyType $propertyName){ $propertyFinalName = $propertyName; }"
        ;;
      q)
        exitRequest=true
        ;;
	  c)
  		rm $headerFile"Properties"
  		rm $headerFile"Methods"
  		rm $sourceFile"Methods"
  		rm $sourceFile"GettersSetters"
      ;;
	  s)
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

  		cat $headerFile"Properties" >> $headerFile

  		appendHeader
  		appendHeader "public:"
  		appendHeader
  		appendHeader "  DE_CLASS($className, DE_Class);"
  		appendHeader

  		cat $headerFile"Methods" >> $headerFile

  		appendHeader
  		appendHeader "};"
  		appendHeader "} /* namespace DE */"
  		appendHeader "#endif /* DE_${className^^}_H */"



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

  		cat $sourceFile"Methods" >> $sourceFile

  		appendSource
  		appendSource "// ---------------------------------------------------------------------------"
  		appendSource

  		cat $sourceFile"GettersSetters" >> $sourceFile

  		appendSource
  		appendSource "} /* namespace DE */"
      ;;
    esac
  # done
done
