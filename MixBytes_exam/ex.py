import time
import sys

def f(n):
	for i in range(n):
		f(n-i-1)
		
def benchmark(n):
	ts1 = time.time()
	f(n)
	ts2 = time.time()
	print(n, ts2-ts1)
	
sys.setrecursionlimit(2000)
print("begin of script")
for x in range(25):
	benchmark(x)
print("end of script")
