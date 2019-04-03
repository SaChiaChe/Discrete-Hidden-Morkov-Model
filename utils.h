#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printModel(const HMM Model){
	printf("Name: %s\n", Model.model_name);
	printf("State num: %d\n", Model.state_num);
	printf("Observ_num: %d\n", Model.observ_num);
	printf("Initial:\n");
	double SumofRow = 0.0;
	for(int i = 0; i < Model.state_num; i++){
		printf("%lf ", Model.initial[i]);
		SumofRow += Model.initial[i];
	}
	printf("%lf \n\n", SumofRow);
	printf("Transition:\n");
	for(int i = 0; i < Model.state_num; i++){
		SumofRow = 0.0;
		for(int j = 0; j < Model.observ_num; j++){
			printf("%lf ", Model.transition[i][j]);
			SumofRow += Model.transition[i][j];
		}
		printf("%lf \n", SumofRow);
	}
	printf("\n");
	printf("Observation:\n");
	double SumofCol[Model.state_num];
	for(int i = 0; i < Model.state_num; i++){
		SumofCol[i] = 0.0;
	}
	for(int i = 0; i < Model.state_num; i++){
		for(int j = 0; j < Model.state_num; j++){
			printf("%lf ", Model.observation[i][j]);
			SumofCol[j] += Model.observation[i][j];
		}
		printf("\n");
	}
	for(int i = 0; i < Model.state_num; i++){
		printf("%lf ", SumofCol[i]);
	}
	printf("\n");

	return;
}

int State2Num(char State){
	return (State - 'A');
}

int MaxID(const double *P, const int Num){
	int BestID = 0;
	for(int i = 1; i < Num; i++){
		if(P[i] > P[BestID]){
			BestID = i;
		}
	}
	return BestID;
}