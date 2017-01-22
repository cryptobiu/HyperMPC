#! /bin/bash
for i in `seq $1 1 $2`;
do
        ./Secret_Sharing $i $3 inputs10.txt output.txt $4 tcp://192.168.0.12:1883 $5 &
        echo "Running $i..."
done

