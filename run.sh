#!/bin/bash

cd scratch/android-arm/
BASE=`basename "$@"`
cp  "$@" /arm/apps/${BASE} 
./profile.sh /arm/apps/${BASE}

