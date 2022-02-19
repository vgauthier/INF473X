#!/bin/bash
x=1
while [ $x -le 100 ]
do
    echo "num thread running: $x"
    x=$(( $x + 1 ))
    echo -n "Test 1" |netcat -u localhost 2048 -c
    echo -n "Test 2" |netcat -u localhost 2048 -c
    echo -n "Test 3" |netcat -u localhost 2048 -c
done