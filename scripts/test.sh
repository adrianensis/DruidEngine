#! /bin/bash

currentDir=$(dirname "$BASH_SOURCE")"/.."
cd $currentDir

outputDir="output"

function readTest(){
	f=$1

	output=$(tail -1 "$outputDir/$f.txt")
	time=$(echo $output | cut -d" " -f1)
	ok=$(echo $output | cut -d" " -f2)
	fail=$(echo $output | cut -d" " -f3)

	printf "\n> "

	if [ $fail = "0" ]
	then
		printf "OK"
	else
		printf "FAIL"
	fi

	printf "\t\t$output\t\t$f\n"
}

cd "binaries"

echo -e "\n-----------------------------------------------------\n"
echo -e "  RESULT \t TIME(ms)/OK/FAIL \t TEST NAME\n"
echo -e "-----------------------------------------------------"

rm -R "output" 2> /dev/null
tests=$(ls)
mkdir $outputDir

for f in $tests
do
    if [[ $f = *"test_unit_"* ]]
    then
      ./$f >> "$outputDir/$f.txt"
	    readTest $f
    fi
done

printf "\n"
