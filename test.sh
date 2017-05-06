#! /bin/bash

cd "./bin/"

for f in $(ls .)
do
	if [ ${f: -4} != ".txt" ]
	then
		output=$("./$f" | tail -1)
		time=$(echo $output | cut -d" " -f1 )
		ok=$(echo $output | cut -d" " -f2 )
		fail=$(echo $output | cut -d" " -f3 )

		if [ $fail = "0" ]
		then
			echo "$f $output OK"
		else
			echo "$f $output FAIL"
		fi
	fi
done
