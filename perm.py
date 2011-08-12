#!/usr/bin/python

import os
import stat

#print os.stat('cmd.py')
print oct(stat.S_IMODE(os.stat('cmd.py').st_mode))
#os.mkfifo('hello')
