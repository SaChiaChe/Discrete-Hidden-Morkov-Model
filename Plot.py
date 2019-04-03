import sys
import matplotlib.pyplot as plt

if __name__ == "__main__":
	if len(sys.argv) != 2:
		print("Format: python Plot.py TestResult")
		exit(0)

	X, Y = [], []
	with open(sys.argv[1]) as f:
		for line in f:
			Line = line.split()
			X.append(int(Line[0]))
			Y.append(float(Line[1]))

	plt.plot(X, Y)
	plt.show()