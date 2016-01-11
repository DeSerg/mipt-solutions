#!/usr/bin/python2

import sys

for x in sys.stdin.readlines():
	for i in x.split():
		q = ''
		for c in i:
			if str.isalpha(c):
				q = q + c
		if len(q) > 0 and q[0].isupper():
			print(q + '\t' + str(1))
