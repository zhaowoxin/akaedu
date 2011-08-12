#!/usr/bin/python

import os
import commands

cmd = 'ls -l'
values = os.popen(cmd).read()

for v in values.split(' '):
    print v

#a, b = commands.getstatusoutput('ls -l')

#print "quit status %s \n output results %s\n" %(a, b)
