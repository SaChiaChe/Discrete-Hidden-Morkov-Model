#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hmm.h"
#include "utils.h"
#include "Calculations.h"

void trainModel(HMM *Model, const char TrainDataPath[], const int Iteration, const char *OutputModelPath){
	int StateNum = Model->state_num;
	int ObservNum = Model->observ_num;

	for(int iter = 0; iter < Iteration; iter++){
		printf("==========Iteration %d==========\n", iter);
		FILE *TrainDataFile = open_or_die(TrainDataPath, "r");

		double Gamma[MAX_LINE][MAX_STATE] = {0.0};
		double GammaO[MAX_OBSERV][MAX_STATE] = {0.0};
		double Epsilon[MAX_LINE][MAX_STATE][MAX_STATE] = {0.0};

		char TrainData[MAX_LINE];
		int DataSize = 0;
		while(fscanf(TrainDataFile, "%s", TrainData) != EOF){
			DataSize++;
			int DataLen = strlen(TrainData);
			double Alpha[MAX_LINE][MAX_STATE] = {0.0};
			double Beta[MAX_LINE][MAX_STATE] = {0.0};

			// Calculate alpha beta
			CalAlpha(Model, Alpha, TrainData);
			CalBeta(Model, Beta, TrainData);
			
			// Accumulate gamma epsilon
			CalGamma(Gamma, GammaO, Alpha, Beta, DataLen, StateNum, TrainData);
			CalEpsilon(Model, Epsilon, Alpha, Beta, TrainData);
		}
		// Update model
		Update(Model, Gamma, GammaO, Epsilon, DataSize, TrainData);

		// Close train data
		fclose(TrainDataFile);

		// if(iter % 10 == 0){
		// 	char tempModelName[1024] = {'\0'};
		// 	strncpy(tempModelName, OutputModelPath, 17);
		// 	sprintf(tempModelName, "%s_%d.txt", tempModelName, iter+1);
		// 	printf("Save current model to: %s\n", tempModelName);
		// 	FILE *fp = open_or_die(tempModelName, "w+");
		// 	dumpHMM(fp, Model);
		// 	fclose(fp);
		// }
	}
	
	return;
}

int main(int argc, char *argv[]){
	if(argc != 5){
		printf("Format: ./train Iteration InitModelPath TrainDataPath OutputModelPath\n");
		return 0;
	}

	// Load parameters
	int Iteration = atoi(argv[1]);
	char *InitModelPath = argv[2];
	char *TrainDataPath = argv[3];
	char *OutputModelPath = argv[4];

	// Load initial model
	HMM Model;
	loadHMM(&Model, InitModelPath);
	strcpy(Model.model_name, OutputModelPath);

	// Print initial model
	// printf("Initial Model:\n");
	// printModel(Model);

	// Start training
	trainModel(&Model, TrainDataPath, Iteration, OutputModelPath);

	// Print final model
	// printf("Final Model:\n");
	// printModel(Model);

	// Save Model
	FILE *OutputModel = open_or_die(OutputModelPath, "w+");
	dumpHMM(OutputModel, &Model);
	fclose(OutputModel);

	return 1;
}