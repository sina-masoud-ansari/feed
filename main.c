#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "data.h"
#include "nn.h"

int main (int argc, char** argv);

int main (int argc, char** argv)
{	
	// network params	
	network* net = (network*) malloc( sizeof(network) );
	net->learningRate = 0.1;
	net->numInputs = 1;
	net->numOutputs = net->numInputs;
	// specify one hidden layer of 5 units
	net->numHiddenLayers = 1;
	int* numPerHiddenLayer = (int*) malloc(sizeof(int) * net->numHiddenLayers);
	numPerHiddenLayer[0] = 2;
	net->numPerHiddenLayer = numPerHiddenLayer;

	// construct network
	initNetwork(net);
		
	// init data
	data* dp = readData(argv[1]);
	//printData(dp);

	// training/test parametres
	srand(123); // srand(time(NULL))
	CVSuite* tests = (CVSuite*) malloc( sizeof(CVSuite) );
	tests->numFolds = 1;
	tests->numPerFold = 1;
	tests->samples = getRandomSamples(dp, (tests->numFolds * tests->numPerFold), net->numInputs, net->numOutputs);
	tests->examples = generateTrainingExamples(tests->samples, (tests->numFolds * tests->numPerFold), net, mapHighLow);

	//train network
	//backPropogation(tests->examples, 0, tests->numPerFold, net); // first fold

	//printNetworkGraph(net);

	return 0;
}












