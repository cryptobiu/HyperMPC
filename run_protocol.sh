#! /bin/bash
for i in `seq 1 1 $1`;
do	
	./Secret_Sharing $i $1 $2 "/home/hila/ClionProjects/Secret-Sharing/output.txt" $3 "tcp://localhost:1883" &
	echo "Running $i..."
done
