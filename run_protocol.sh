#! /bin/bash
for i in `seq $1 1 $2`;
do
	./HyperMPC -partyID ${i} -partiesNumber ${3} -inputFile ${4} -outputFile output.txt -circuitFile ${5} -partiesFile ${6} -fieldType ${7} -internalIterationsNumber ${8}  -NG ${9} &
	echo "Running $i..."
done

#sleep 20
#killall -9 Secret_Sharing
