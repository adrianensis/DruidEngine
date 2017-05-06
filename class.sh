#! /bin/bash

className=$1
classNameUppercase=$(echo $className| tr '[:lower:]' '[:upper:]')
headerName=$className".h"
sourceName=$className".cpp"

new () {
  cat /dev/null > $1
}

line () {
  echo "$1" >> $2
}

blank () {
  echo >> $1
}

# HEADER

new $headerName

line "#ifndef INCLUDE_"$classNameUppercase"_H_" $headerName
line "#define INCLUDE_"$classNameUppercase"_H_" $headerName
blank $headerName

line "namespace DE {" $headerName
blank $headerName

line "class "$className" : {" $headerName
blank $headerName

line "private:" $headerName
blank $headerName

line "public:" $headerName
blank $headerName

line "};" $headerName
blank $headerName

line "} /* namespace DE */" $headerName
blank $headerName

line "#endif /* INCLUDE_"$classNameUppercase"_H_ */" $headerName

# SOURCE

new $sourceName

line "#include \"$headerName\"" $sourceName
blank $sourceName

line "namespace DE {" $sourceName
blank $sourceName

line "} /* namespace DE */" $sourceName
