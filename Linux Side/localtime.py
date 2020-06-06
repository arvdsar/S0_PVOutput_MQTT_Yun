#!/usr/bin/python
import time
from time import gmtime,mktime

#Get localtime 
current_local = time.localtime() 

dst = current_local.tm_isdst
current_utc = mktime(current_local)

if dst == 1:
    local_seconds = current_utc + 7200
else:
    local_seconds = current_utc + 3600

print(int(local_seconds))

