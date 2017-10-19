#! /bin/bash
for i in `seq $1 1 $2`;
do
	./Secret_Sharing -partyID $i -partiesNumber $3 -inputFile $4 -outputFile output.txt -circuitFile $5 -partiesFile Parties.txt -fieldType $6 -internalIterationsNumber $7 &
	echo "Running $i..."
done
