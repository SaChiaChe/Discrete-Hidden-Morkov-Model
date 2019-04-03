#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "utils.h"

double Viterbi(const HMM *Model, const char *TestData){
	// Set some parameters
	int DataLen = strlen(TestData);
	int StateNum = Model->state_num;
	int ObservNum = Model->observ_num;
	double Pi[StateNum];
	memcpy(Pi, Model->initial, sizeof(double) * StateNum);
	double A[StateNum][StateNum];
	for(int i = 0; i < StateNum; i++)
		memcpy(A[i], Model->transition[i], sizeof(double) * StateNum);
	double B[ObservNum][StateNum];
	for(int i = 0; i < ObservNum; i++)
		memcpy(B[i], Model->observation[i], sizeof(double) * StateNum);

	// Create delta table
	double Delta[DataLen][StateNum];

	// Initialize at t0
	int Time = 0;
	int O = State2Num(TestData[Time]);
	for(int i = 0; i < StateNum; i++){
		Delta[Time][i] = Pi[i] * B[O][i];
	}

	// Recursion from t1 to tn
	for(Time = 1; Time < DataLen; Time++){
		O = State2Num(TestData[Time]);
		for(int j = 0; j < StateNum; j++){
			double BestP = -1.0;
			for(int i = 0; i < StateNum; i++){
				double CurP = Delta[Time-1][i] * A[i][j];
				if(CurP > BestP){
					BestP = CurP;
				}
			}
			Delta[Time][j] = BestP * B[O][j];
		}
	}

	return Delta[DataLen-1][MaxID(Delta[DataLen-1], StateNum)];
}