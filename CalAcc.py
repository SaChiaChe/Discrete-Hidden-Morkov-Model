import sys

if __name__ == "__main__":
	if len(sys.argv) != 3:
		print("Format: python ResultFilePath AnswerFilePath")
		exit(0)

	Result = []
	with open(sys.argv[1], "r") as f:
		for line in f:
			Line = line.split()
			Result.append(Line[0].split("/")[-1][:8] + ".txt")

	Answer = []
	with open(sys.argv[2], "r") as f:
		for line in f:
			Line = line.split()
			Answer.append(Line[0])

	DataLen = len(Result)
	ErrorCount = 0
	for i in range(DataLen):
		if Result[i] != Answer[i]:
			ErrorCount += 1

	print(1 - ErrorCount/DataLen)