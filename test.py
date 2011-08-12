#!/usr/bin/python

import os  
import commands

def system(files):
   os.system(files) 

print system('ls -l')
print os.getcwd() 
