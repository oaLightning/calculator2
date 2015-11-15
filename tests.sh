#!/usr/bin/env bash

for i in {1..5}
do 
    cat ./tests/test$i.in | ./ex1 | diff - ./expected/expected$i.out || echo There is a problem with test $i
done
echo Finished all the tests