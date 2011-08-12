#!/bin/bash

a=1
cd /etc/
while read line;
do 
    file=`echo $line | awk -F":" '{print $1}'`
    echo "The usr No is $a:" $file
    a=`expr $a + 1`
done<passwd

