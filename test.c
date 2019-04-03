#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "hmm.h"
#include "Viterbi.h"

void testModel(const HMM *Models, 
			   const int ModelNum, 
			   const char *TestDataPath, 
			   const char *OutputResultPath){
	// Load test data
	FILE *TestDataFile = open_or_die(TestDataPath, "r");

	// Open output file
	FILE *OutputResultFile = open_or_die(OutputResultPath, "w+");

	// Start testing
	char TestData[MAX_LINE];
	while(fscanf(TestDataFile, "%s", TestData) != EOF){
		double Probabilities[ModelNum];
		for(int i = 0; i < ModelNum; i++){
			Probabilities[i] = Viterbi(&Models[i], TestData);
		}
		int BestModel = MaxID(Probabilities, ModelNum);
		fprintf(OutputResultFile, "%s\t%e\n", Models[BestModel].model_name, Probabilities[BestModel]);
	}

	return;
}

int main(int argc, char *argv[]){
	if(argc != 4){
		printf("Format: ./test ModelListPath TestDataPath OutputResultPath\n");
		return 0;
	}

	// Load parameters
	char *ModelListPath = argv[1];
	char *TestDataPath = argv[2];
	char *OutputResultPath = argv[3];

	// Load models
	HMM Models[5];
	load_models(ModelListPath, Models, 5);

	// Start testing
	testModel(Models, 5, TestDataPath, OutputResultPath);

	return 0;
}