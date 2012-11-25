#ifndef DATA_H
#define DATA_H

typedef struct 
{
	double open;
	double high;
	double low;
	double close;
} ohlc;

typedef struct 
{
	int length;
	ohlc* rows;
} data;

typedef struct
{
	data* dp;
	int inputStart;
	int inputLength;
	int outputStart;
	int outputLength;
} dataSample;

void printData (data* dp); // untested
void printDataRange (data* dp, int start, int end); // untested

data* readData (char* fname); // test_read
dataSample* getRandomSamples(data* dp, int numSamples, int sampleInputLength, int sampleOutputLength); // TODO
int check_data(data* dp, int print_summary); // test_data
int check_ohlc(ohlc* row); // test_ohlc

#endif
