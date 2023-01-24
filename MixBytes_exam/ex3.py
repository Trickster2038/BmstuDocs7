import sys

def f(k):
	# k = int(sys.argv[1])
	a1 = 1
	d = 1
	s = (a1*k) + d*k*(k-1)/2
	return s
	
x = int(sys.argv[1])
print("x:", x)
print(int(f(x)))
print("7590488951404466049403 - in search")
