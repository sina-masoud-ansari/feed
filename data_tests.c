#include <check.h>

#include "data_tests.h"
#include "data.h"

#define TEST_INPUT "test_input.txt"

START_TEST (test_read)
{
	char* fname = TEST_INPUT;
	data* dp = readData(fname);

	// check the nuber of rows are correct 
	fail_unless(dp->length == 6, "Data length was %d, should be 6", dp->length);
	ohlc* rows = dp->rows;

	// check start and end are mapped correctly
	ohlc start; start.open = 1.5; start.high = 5.1; start.low = 1.0; start.close = 3.4;	
	fail_unless(rows[0].open == start.open, "Start open incorrerct"); 
	fail_unless(rows[0].high == start.high, "Start high incorrerct"); 
	fail_unless(rows[0].low == start.low, "Start low incorrerct");
	fail_unless(rows[0].close == start.close, "Start close incorrerct");

	ohlc end; end.open = 2.1; end.high = 2.6; end.low = 2.1; end.close = 2.3;
	fail_unless(rows[5].open == end.open, "End open incorrerct"); 
	fail_unless(rows[5].high == end.high, "End high incorrerct"); 
	fail_unless(rows[5].low == end.low, "End low incorrerct");
	fail_unless(rows[5].close == end.close, "End close incorrerct");
	
}
END_TEST

START_TEST (test_ohlc)
{
	ohlc record;

	// all good
	record.open = 1.5;
	record.high = 4.0;
	record.low = 1.0;
	record.close = 3.0;
	fail_unless(check_ohlc(&record) == 0, "Valid record did not pass");

	// OPEN

	// bad open: greater than high
	record.open = 4.5;
	record.high = 4.0; 
	record.low = 1.0;
	record.close = 3.0;
	fail_unless(check_ohlc(&record) == 1, "Open > high should have failed");	

	// bad open: lower than low
	record.open = 0.5;
	record.high = 4.0; 
	record.low = 1.0;
	record.close = 3.0;
	fail_unless(check_ohlc(&record) == 1, "Open < low should have failed");

	// HIGH

	// bad high: lower than low
	record.open = 1.5;
	record.high = 0.5; 
	record.low = 1.0;
	record.close = 3.0;
	fail_unless(check_ohlc(&record) == 1, "High < low should have failed");

	// LOW

	// bad low: higher than high
	record.open = 1.5;
	record.high = 4.0; 
	record.low = 5.0;
	record.close = 3.0;
	fail_unless(check_ohlc(&record) == 1, "Low > high should have failed");

	// CLOSE

	// bad close: greater than high
	record.open = 1.5;
	record.high = 4.0; 
	record.low = 1.0;
	record.close = 5.0;
	fail_unless(check_ohlc(&record) == 1, "Close > high should have failed");	

	// bad close: lower than low
	record.open = 1.5;
	record.high = 4.0; 
	record.low = 1.0;
	record.close = 0.5;
	fail_unless(check_ohlc(&record) == 1, "Close < low should have failed");

}
END_TEST

START_TEST(test_dataset)
{
	char* fname = TEST_INPUT;
	data* dp = readData(fname);
	ohlc* rows = dp->rows;
	
	// check normal data passes	
	fail_unless(check_data(dp, 0) == 0, "Consistent data should have passed");

	// check bad data fails
	rows[0].open = 0.5; // add bad open
	rows[1].high = 0.5; // add bad high
	rows[2].low = 7.0; // add bad low
	rows[3].close = 7.0; // add bad close
	//fail_unless(check_data(dp, 0) == 4, "Inconsistent data should have failed");	
}
END_TEST

START_TEST(test_samples)
{
	int i;
	char* fname = TEST_INPUT;
	data* dp = readData(fname);
	dataSample* samples;
	
	// get random samples
	int input_length = 1;
	int output_length = 1;
	int num_samples = 5;
	samples = getRandomSamples(dp, num_samples, input_length, output_length);

	for (i = 0; i < num_samples; i++)
	{
		fail_unless(samples[i].inputLength == input_length, "Input length must %d", input_length);
		fail_unless(samples[i].inputStart < dp->length, "Start must be less than length");
		fail_unless( (samples[i].inputStart + samples[i].inputLength) < dp->length, "Input window must be less than length");

		fail_unless(samples[i].outputLength == output_length, "Output length must %d", output_length);
		fail_unless(samples[i].outputStart < dp->length, "Output start must be less than length");
		fail_unless( (samples[i].outputStart + samples[i].outputLength) < dp->length, "Output window must be less than length");

	}

	fail_unless(i == num_samples, "Number of samples was %d, should be %d", i, num_samples);
	
}
END_TEST

Suite* test_data(void)
{
	Suite* suite = suite_create("Data Tests");

	TCase* tc_read = tcase_create("Read data from file");
	tcase_add_test (tc_read, test_read);
	suite_add_tcase (suite, tc_read);

	TCase* tc_ohlc = tcase_create("Check record validity");
	tcase_add_test (tc_ohlc, test_ohlc);
	suite_add_tcase (suite, tc_ohlc);

	TCase* tc_data = tcase_create("Check data consistency");
	tcase_add_test (tc_data, test_dataset);	
	suite_add_tcase (suite, tc_data);

	TCase* tc_samples = tcase_create("Check data sample creation");
	tcase_add_test (tc_samples, test_samples);	
	suite_add_tcase (suite, tc_samples);

	return suite;
}

void run_data_tests(int* num_failed)
{
	Suite* s = test_data();
	SRunner* sr = srunner_create(s);
	srunner_run_all (sr, CK_NORMAL);
	srunner_free(sr);
	*num_failed += srunner_ntests_failed(sr); // update num_failed
}
