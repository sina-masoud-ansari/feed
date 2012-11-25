#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#include "nn.h"

void initNetwork(network* net)
{
	int i, j, k;
	int valid;
	// edges
	int numInputEdges;
	int numOutputEdges;	
	edge** inputEdges;
	edge* outputEdges;
	double weight;

	// nodes	
	net->totalInputs = net->numInputs * net->numInputFeatures;
	net->totalOutputs = net->numOutputs * net->numOutputFeatures;
	int numLayers = net->numHiddenLayers + 2; // hidden + input and output
	node** nodes = (node**) malloc( sizeof(node*) * numLayers ); 
	
	// assign number of nodes per layer
	int* numPerLayer = (int*) malloc(sizeof(int) * numLayers);
	numPerLayer[0] = net->totalInputs;
	numPerLayer[numLayers-1] = net->totalOutputs;
	for (i = 1; i < numLayers - 1 ; i++)
	{
		numPerLayer[i] = net->numPerHiddenLayer[i - 1];
	}
	
	// iterate through nodes and assign output edges
	for (i = 0; i < numLayers; i++)
	{	
		nodes[i] = (node*) malloc( sizeof(node) * numPerLayer[i] );

		// setup layer parameters
		if (i == 0)
		{
			//input layer
			numInputEdges = 0;
			numOutputEdges = numPerLayer[i + 1];
		} else if ( i == (numLayers - 1) )
		{
			//output layer
			numInputEdges = numPerLayer[i - 1];
			numOutputEdges = 0;			
		} else 
		{
			//hidden layer
			numInputEdges = numPerLayer[i - 1];
			numOutputEdges = numPerLayer[i + 1];
		}

		// define nodes in the layer
		for (j = 0; j < numPerLayer[i]; j++)
		{
			inputEdges = (edge**) malloc(sizeof(edge*) * numInputEdges);
			outputEdges = (edge*) malloc(sizeof(edge) * numOutputEdges);

			// assign random output edge weights for layers below the output layer
			//if (i < (numLayers - 1))
			//{
				for (k = 0; k < numOutputEdges; k++)
				{
					// define the output edge
					valid = 0;
					while (!valid)
					{
						// find a small random weight
						weight = (double) rand() / (double) RAND_MAX;
						if (fabs(weight < 0.05))
						{
							valid = 1;
						}

					}

					// randomly assign +/- sign to weight
					if (rand() < RAND_MAX/2)
					{	
						weight = weight * -1.0;
					}
					outputEdges[k].weight = weight;
					outputEdges[k].delta = 0;
					outputEdges[k].from = &nodes[i][j];
				}
			//}
	
			nodes[i][j].layer = i;
			nodes[i][j].index = j;
			nodes[i][j].inputValue = 0;
			nodes[i][j].outputValue = 0; 
			nodes[i][j].numInputEdges = numInputEdges;
			nodes[i][j].numOutputEdges = numOutputEdges;
			nodes[i][j].inputEdges = inputEdges;
			nodes[i][j].outputEdges = outputEdges;		
		}
	}

	// iterate through nodes and assign input edges
	for (i = 0; i < numLayers; i++)
	{	
		// for nodes in the layer
		for (j = 0; j < numPerLayer[i]; j++)
		{	
			inputEdges = nodes[i][j].inputEdges;
			for (k = 0; k < numPerLayer[i - 1]; k++)
			{
				inputEdges[k] = &nodes[i - 1][k].outputEdges[j];
				inputEdges[k]->to = &nodes[i][j];			
			}
		}	
	}
	
	// assgin properties to network
	net->nodes = nodes;
	net->numLayers = numLayers;
	net->numPerLayer = numPerLayer;

}

void printNetworkGraph(network* net)
{
	int i, j, k;

	// forward links
	FILE* fp = fopen("forward.dot", "w");
	if (fp != NULL)
	{
		fputs("digraph Forward {\n\t", fp);
		for (i = 0; i < net->numLayers; i++)
		{
			for (j = 0; j < net->numPerLayer[i]; j++)
			{
				for (k = 0; k < net->nodes[i][j].numOutputEdges; k++)
				{
					fprintf(fp, "%d%d -> %d%d [ label = \"%.4f\"]\n\t", net->nodes[i][j].layer, net->nodes[i][j].index, net->nodes[i][j].outputEdges[k].to->layer, net->nodes[i][j].outputEdges[k].to->index, net->nodes[i][j].outputEdges[k].weight);
				}
			}
		}
		fputs("\n}", fp);
	} else
	{
		perror("Error writing graph");
	}

	// backward links
	fp = fopen("backward.dot", "w");
	if (fp != NULL)
	{
		fputs("digraph Backward {\n\trankdir=BT\n\t", fp);
		for (i = 0; i < net->numLayers; i++)
		{
			for (j = 0; j < net->numPerLayer[i]; j++)
			{
				for (k = 0; k < net->nodes[i][j].numInputEdges; k++)
				{
					fprintf(fp, "%d%d -> %d%d [ label = \"%.4f\"]\n\t", net->nodes[i][j].layer, net->nodes[i][j].index, net->nodes[i][j].inputEdges[k]->from->layer, net->nodes[i][j].inputEdges[k]->from->index, net->nodes[i][j].inputEdges[k]->weight);
				}
			}
		}
		fputs("\n}", fp);
	} else
	{
		perror("Error writing graph");
	}
}

void backPropogation(trainingExample* examples, int startIndex, int numExamples, network* net)
{
	//termination condition
	int steps;
	int maxSteps = 1000;
	
	int i, j;
	
	// untill termination condition is met
	while (steps < maxSteps)
	{
		// for each example case
		for(i = startIndex; i < numExamples; i++)
		{

			// initiate network input
			for(j= 0; j < net->totalInputs; j++)
			{	

				net->nodes[0][j].outputValue = examples[i].inputValues[j]; 
			}
	
			// compute output of every output unit in the network
		}

		steps++;
	}
	
}

trainingExample* generateTrainingExamples(dataSample* samples, int numSamples, network* net, void (*map)(dataSample* sample, trainingExample* example, network* net))
{
	int i;
	trainingExample* examples = (trainingExample*) malloc( sizeof(trainingExample) * numSamples );

	for(i = 0; i < numSamples; i++)
	{
		map(&(samples[i]), &(examples[i]), net);
	}

	return examples;
}

void mapHighLow(dataSample* sample, trainingExample* example, network* net)
{
	int i, start;
	double max, min, range, temp, high, low;	
	ohlc* rows;
	ohlc t;
	
	int numInputFeatures = 2;
	int numOutputFeatures = 2;

	// allocate memory
	double* input = (double*) malloc(sizeof(double) * net->numInputs * numInputFeatures);
	double* output = (double*) malloc(sizeof(double) * net->numOutputs * numOutputFeatures);

	// map inputs
	rows = (*sample).dp->rows;
	// find max and min from the sample 
	start = (*sample).inputStart;
	t = rows[start];
	max = t.high;
	min = t.low;
	for (i = 1; i < (*sample).inputLength; i++)
	{
		t = rows[start + i];

		temp = t.high;
		if (temp > max)
		{
			max = temp;
		}

		temp = t.low;
		if (temp < min)
		{
			min = temp;
		}		
	}
	range = max - min;	
	
	// for each instance in sample input
	for (i = 0; i < (*sample).inputLength; i++)
	{
		t = rows[start + i];

		// get high and low and normalise against range
		high = (t.high - min) / range;
		low = (t.low - min) / range;
		// assign to input array
		input[2*i] = high;
		input[2*i + 1] = low;
	}

	// map outputs
	// for each instance in sample output
	start = (*sample).outputStart;
	for (i = 0; i < (*sample).outputLength; i++)
	{
		t = rows[start + i];

		// get high and low and normalise against range
		high = (t.high - min) / range;
		low = (t.low - min) / range;
		// assign to input array
		output[2 * i] = high;
		output[2 * i + 1] = low;
	}

	// assign network properties
	net->numInputFeatures = numInputFeatures;
	net->numOutputFeatures = numOutputFeatures;

	example->inputValues = input;
	example->outputValues = output; 
}
