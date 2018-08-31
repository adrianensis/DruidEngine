#! /bin/bash

function runTest() {
	f=$1

	output=$("./$f" | tail -1)
	time=$(echo $output | cut -d" " -f1 )
	ok=$(echo $output | cut -d" " -f2 )
	fail=$(echo $output | cut -d" " -f3 )

	printf "\n> "

	if [ $fail = "0" ]
	then
		printf "OK"
	else
		printf "FAIL"
	fi

	printf "\t\t$output\t\t$f\n"
}

export -f runTest

cd "./testOutput/"

echo -e "\n-----------------------------------------------------\n"
echo -e "  RESULT \t TIME(ms)/OK/FAIL \t TEST NAME\n"
echo -e "-----------------------------------------------------"

array=()

for f in $(ls)
do
		if [ ${f: -4} != ".txt" ]
		then
			# array+=($f)
			runTest $f
		fi
done

# parallel -j 4 runTest ::: "${array[@]}"

printf "\n"
