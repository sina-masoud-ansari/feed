#ifndef NN_H
#define NN_H

#include <math.h> 

#include "data.h"

typedef struct
{
	double* inputValues;
	double* outputValues;
} trainingExample;

typedef struct {
	int numFolds;
	int numPerFold;
	dataSample* samples;
	trainingExample* examples;	
} CVSuite;

typedef struct node node;
typedef struct edge edge;

struct edge
{	
	node* from;
	node* to;
	double weight;
	double delta;
};

struct node 
{
	int layer;
	int index;
	double inputValue;
	double outputValue;
	int numInputEdges;
	int numOutputEdges;
	edge** inputEdges;
	edge* outputEdges;
};

typedef struct {
	double learningRate;

	int numInputs;
	int numInputFeatures;
	int totalInputs;

	int numOutputs;
	int numOutputFeatures;
	int totalOutputs;

	int numHiddenLayers;
	int numLayers;
	int* numPerHiddenLayer;
	int* numPerLayer;
	node** nodes;
	
} network;

void mapHighLow(dataSample* sample, trainingExample* example, network* net);
trainingExample* generateTrainingExamples(dataSample* samples, int numSamples, network* net, void (*map)(dataSample* sample, trainingExample* example, network* net));
void initNetwork(network* net);
void printNetworkGraph(network* net);
void backPropogation(trainingExample* examples, int startIndex, int numExamples, network* network);
void mapHighLow(dataSample* sample, trainingExample* example, network* net);


#endif
