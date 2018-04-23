#! /bin/bash
for i in `seq $1 1 $2`;
do
	./Secret_Sharing -partyID ${i} -partiesNumber ${3} -inputFile ${4} -outputFile output.txt -circuitFile ${5} -partiesFile ${6} -fieldType ${7} -internalIterationsNumber ${8} &
	echo "Running $i..."
done