#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data.h"

// Print all data points
void printData (data* dp)
{
	int i;

	for (i = 0; i < dp->length; i++)	
	{
		printf("%.2f ", dp->rows[i].open);		
		printf("%.2f ", dp->rows[i].high);
		printf("%.2f ", dp->rows[i].low);
		printf("%.2f\n", dp->rows[i].close);
	}
}

// Print all data points within a range (start inclusive, end not inclusive)
void printDataRange (data* dp, int start, int end)
{
	int i;
	
	// range check start
	if (start < 0)
	{
		start = 0;
	} else if (start > dp->length){
		start = dp->length;
	}

	// range check end
	if (end < 0)
	{
		end = 0;
	} else if (end > dp->length){
		end = dp->length;
	}

	// ensure start <= end
	if (start > end)
	{
		start = end;
	}

	for (i = start; i < end; i++)	
	{
		printf("%.2f ", dp->rows[i].open);		
		printf("%.2f ", dp->rows[i].high);
		printf("%.2f ", dp->rows[i].low);
		printf("%.2f\n", dp->rows[i].close);
	}
}

// Read a CSV file into a ohlc array
data* readData (char* fname)
{	
	data* d = (data*) malloc ( sizeof(data) );
	int n = 0;
	int i;
	char line [128];
	char* tok;

	// read the file and count the number of rows

	FILE* fp = NULL;
	fp = fopen(fname, "r");

	if (fp != NULL) 
	{
		while (fgets(line, 128, fp) != NULL )
		{
				n++;
		}
	} else {
		perror("Failed to open input file");
	}
	
	// costruct and populate the data record 

	d->length = n - 1; // ignore header line
	d->rows = (ohlc*) malloc ( sizeof(ohlc) * d->length);
	
	n = 0;
	rewind(fp);
	fgets(line, 128, fp); // skip header 

	while (fgets(line, 128, fp) != NULL )
	{
		tok = strtok (line,",");
		i = 1;
		while (tok != NULL)
		{
			tok = strtok (NULL, ",");
			switch (i)
			{
				case 1:
					d->rows[n].open = atof(tok);
					break;
				case 2:
					d->rows[n].high = atof(tok);
					break;
				case 3:
					d->rows[n].low = atof(tok);
					break;
				case 4:
					d->rows[n].close = atof(tok);
					break;
			}

			i++;
			if (i > 4)
			{
				break;	
			}
		}
		n++;	
	}

	return d;
}

dataSample* getRandomSamples(data* dp, int numSamples, int sampleInputLength, int sampleOutputLength)
{
	int i;
	int valid;

	// create set of random samples
	dataSample* samples = (dataSample*) malloc (sizeof(dataSample) * numSamples);
	for (i = 0; i < numSamples; i++)
	{
		valid = 0;
		while(!valid){
			samples[i].dp = dp;
			samples[i].inputStart = rand() % dp->length;
			samples[i].inputLength = sampleInputLength;
			samples[i].outputStart = samples[i].inputStart + sampleInputLength + 1;
			samples[i].outputLength = sampleOutputLength;
			if (samples[i].outputStart + samples[i].outputLength < dp->length) {
				valid = 1;
			}
		}
	}

	return samples;
}	

int check_data(data* dp, int print_summary)
{
	int i, f;
	double o, h, l, c;
	int num_failed = 0;
	int* failed;
	ohlc* rows = dp->rows;

	// count the number of failures
	for (i = 0; i < dp->length; i++)
	{
		num_failed += check_ohlc(&rows[i]); // returns 1 on failure		
	}

	// record the failures
	f = 0;
	failed = (int*) malloc(sizeof(int) * num_failed);	
	for (i = 0; i < dp->length; i++)
	{
		if (check_ohlc(&rows[i]))
		{
			failed[f] = i;
			f++;
		}
	}
	
	// print summary
	if (print_summary == 1)
	{
		printf("Data checked: %d failures\n", num_failed);
		for (i = 0; i < num_failed; i++)
		{
			f = failed[i];
			o = rows[f].open;
			h = rows[f].high;
			l = rows[f].low;
			c = rows[f].close;
			printf("Row: %d\tO: %.2f H: %.2f L: %.2f C: %.2f Reason: ", f, o, h, l, c);
			print_reason(&rows[f]);
		}	
	}
	
	return num_failed;
	
}

int check_ohlc(ohlc* row)
{
	double o = row->open;
	double h = row->high;
	double l = row->low;
	double c = row->close;

	// check open
	if (o < l || o > h)
	{
		return 1;
	}	

	// check high and low
	if (h < l)
	{
		return 1;
	}	
	
	// check close
	if (c < l || c > h)
	{
		return 1;
	}
	
	return 0;
}

void print_reason(ohlc* row)
{
	double o = row->open;
	double h = row->high;
	double l = row->low;
	double c = row->close;
	
	// check open
	if (o < l) {
		puts("Open < Low ");
		
	}
	if (o > h)
	{
		puts("Open > High ");
	}	

	// check high and low
	if (h < l)
	{
		puts("High < Low ");
	}	
	
	// check close
	if (c < l)
	{
		puts("Close < Low ");
	}
 	if (c > h)
	{
		puts("Close > High ");
	}
	puts("\n");
}
