#! /bin/bash

cd "./test_bin/"

echo -e "\n-----------------------------------------------------\n"
echo -e "  RESULT \t TIME/OK/FAIL \t\t TEST NAME\n"
echo -e "-----------------------------------------------------"

for f in $(ls .)
do
	if [ ${f: -4} != ".txt" ]
	then
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
	fi
done

printf "\n"
