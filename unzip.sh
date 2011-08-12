#!/bin/bash 

umask 266
dir="temp"
if [ $# -eq 0 ]; then
    echo "Argument too less!"
fi
if ! [ -d ${dir} ]; then
    mkdir ~/${dir}
fi
unzip $1 -d ~/${dir}
cd ~/${dir}/lib/armeabi/
GOAL=`ls | grep ".*.so"`
if [ -z ${#GOAL} ];then
    echo -e "\tNo .so file !"
    exit 1
fi
echo -e "`basename $1`" >> ~/log.bak
for file in ${GOAL}
do
    echo -e "${file}"
    readelf -A ${file} | grep -w "Tag_CPU_arch" 
    readelf -A ${file} | grep -w "Tag_ARM_ISA_use" 
    readelf -A ${file} | grep -w "Tag_THUMB_ISA_use" 
done >> ~/log.bak
echo "" >> ~/log.bak
rm -rf ~/${dir}
umask 022
