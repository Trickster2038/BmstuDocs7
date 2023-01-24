import sys

def f(x): 
	accum = 0 
	for i in range(x): 
		accum = accum + (x - i) 
		if accum % 1000000 == 0:
			print("acc: ", accum)
	return accum 
	
def main(): 
	x = int(sys.argv[1]) # read unsigned integer from command line parameter 
	y = f(x) 
	print(y) 
	
main() 
