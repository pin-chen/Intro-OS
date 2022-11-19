#!/bin/bash
echo "Single thread:"
time ./ST < input.txt > output.txt
echo "Multi thread:"
for i in {1..19}
do
	echo "-------------------------------------"
	echo "Thread number: $((i+1))"
	time ./"MT$i" < input.txt > output.txt
done
