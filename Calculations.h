#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void CalAlpha(const HMM *Model, 
			  double Alpha[MAX_LINE][MAX_STATE], 
			  const char *TrainData){
	// Set some parameters
	int DataLen = strlen(TrainData);
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

	// Initialize at t0
	int Time = 0;
	int O = State2Num(TrainData[Time]);
	for(int i = 0; i < StateNum; i++){
		Alpha[Time][i] = Pi[i] * B[O][i];
	}

	// Inducion from t1 to tn
	for(Time = 1; Time < DataLen; Time++){
		O = State2Num(TrainData[Time]);
		for(int j = 0; j < StateNum; j++){
			double temp = 0.0;
			for(int i = 0; i < StateNum; i++){
				temp += Alpha[Time-1][i] * A[i][j];
			}
			Alpha[Time][j] = temp * B[O][j];
		}
	}

	return;
}

void CalBeta(const HMM *Model, 
			 double Beta[MAX_LINE][MAX_STATE], 
			 const char *TrainData){
	// Set some parameters
	int DataLen = strlen(TrainData);
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

	// Initializtion at time n
	int Time = DataLen-1;
	int O;
	for(int i = 0; i < StateNum; i++){
		Beta[Time][i] = 1.0;
	}

	// Inducion from tn-1 to t0
	for(Time = DataLen-2; Time >= 0; Time--){
		O = State2Num(TrainData[Time+1]);
		for(int i = 0; i < StateNum; i++){
			double temp = 0.0;
			for(int j = 0; j < StateNum; j++){
				temp += A[i][j] * B[O][j] * Beta[Time+1][j];
			}
			Beta[Time][i] = temp;
		}
	}

	return;
}

void CalGamma(double Gamma[MAX_LINE][MAX_STATE],
			  double GammaO[MAX_OBSERV][MAX_STATE],
			  const double Alpha[MAX_LINE][MAX_STATE], 
			  const double Beta[MAX_LINE][MAX_STATE], 
			  const int DataLen, 
			  const int StateNum,
			  const char *TrainData){
	// Accumulate gamma
	for(int Time = 0; Time < DataLen; Time++){
		int O = State2Num(TrainData[Time]);
		for(int i = 0; i < StateNum; i++){
			double temp = 0.0;
			for(int j = 0; j < StateNum; j++){
				temp += Alpha[Time][j] * Beta[Time][j];
			}
			Gamma[Time][i] += Alpha[Time][i] * Beta[Time][i] / temp;
			GammaO[O][i] += Alpha[Time][i] * Beta[Time][i] / temp;
		}
	}

	return;
}

void CalEpsilon(const HMM *Model, 
				double Epsilon[MAX_LINE][MAX_STATE][MAX_STATE], 
				const double Alpha[MAX_LINE][MAX_STATE], 
				const double Beta[MAX_LINE][MAX_STATE], 
				const char *TrainData){
	// Set some parameters
	int DataLen = strlen(TrainData);
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

	// Accumulate epsilon
	for(int Time = 0; Time < DataLen-1; Time++){
		for(int i = 0; i < StateNum; i++){
			for(int j = 0; j < StateNum; j++){
				int O = State2Num(TrainData[Time+1]);
				double temp = 0.0;
				for(int i_ = 0; i_ < StateNum; i_++){
					for(int j_ = 0; j_ < StateNum; j_++){
						temp += Alpha[Time][i_] * A[i_][j_] * B[O][j_] * Beta[Time+1][j_];
					}
				}
				Epsilon[Time][i][j] += Alpha[Time][i] * A[i][j] * B[O][j] * Beta[Time+1][j] / temp;
			}
		}
	}

	return;
}

void Update(HMM *Model, 
			const double Gamma[MAX_LINE][MAX_STATE], 
			const double GammaO[MAX_OBSERV][MAX_STATE],
			const double Epsilon[MAX_LINE][MAX_STATE][MAX_STATE], 
			const int DataSize,
			const char *TrainData){
	// Set some parameters
	int DataLen = strlen(TrainData);
	int StateNum = Model->state_num;
	int ObservNum = Model->observ_num;

	// Update Initial
	for(int i = 0; i < StateNum; i++){
		Model->initial[i] = Gamma[0][i] / DataSize;
	}

	// Update transition
	for(int i = 0; i < StateNum; i++){
		for(int j = 0; j < StateNum; j++){
			double sum_e = 0.0, sum_g = 0.0;
			for(int Time = 0; Time < DataLen-1; Time++){
				sum_e += Epsilon[Time][i][j];
				sum_g += Gamma[Time][i];
			}
			Model->transition[i][j] = sum_e / sum_g;
		}
	}

	// Update observation
	for(int i = 0; i < StateNum; i++){
		for(int k = 0; k < ObservNum; k++){
			double sum_g = 0.0;
			for(int Time = 0; Time < DataLen; Time++){
				sum_g += Gamma[Time][i];
			}
			Model->observation[k][i] = GammaO[k][i] / sum_g;
		}
	}

}