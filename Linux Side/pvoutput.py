#!/usr/bin/python
import sys
import requests

url = 'https://pvoutput.org/service/r2/addstatus.jsp'
headers = {
    'X-Pvoutput-SystemId': sys.argv[1],
    'X-Pvoutput-Apikey': sys.argv[2]
}
params = {
    'd': sys.argv[3],
    't': sys.argv[4],
    'v1': sys.argv[5],
    'v2': sys.argv[6]
}

resp = requests.post(url, headers=headers, data=params, timeout=10)
print(resp.status_code, resp.reason)