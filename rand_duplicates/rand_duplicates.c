/**
* Using histogram to find the number of duplicate entries in an array
* Uses C99 dialect, so compile with -std=gnu99 or -std=c99
* or via IDE-s Visual Studio 2013 (and higher) / DevC++5.7 (and higher)
*/
#include <stdlib.h> // system("pause")
#include <stdio.h>  // printf
#include <string.h> // memset
#include <limits.h> // INT_MAX


// optimized histogram approach, Theta(3n)
static int hist_duplicates(int* values, int count)
{
	int min = INT_MAX, max = 0;

	// find the min-max values to calculate the psan
	for (int i = 0; i < count; ++i)
	{
		int value = values[i];
		if (value < min)      min = value;
		else if (value > max) max = value;
	}
	int size = (max - min) + 1; // the number of elements in the histogram


								// initialize histogram and set all elements to 0
	int* histogram = malloc(sizeof(int) * size);
	for (int i = 0; i < size; ++i)
		histogram[i] = 0;


	// construct the histogram and use it to find duplicate values
	int duplicates = 0;
	for (int i = 0; i < count; ++i)
	{
		// increase histogram value at the specified index
		int normalizedIndex = values[i] - min;
		if (++histogram[normalizedIndex] > 1) // if histogram value at index > 1
			++duplicates;                     // there's a duplicate
	}

	free(histogram);
	return duplicates;
}


// classic O(n^2) approach;
static int bubble_duplicates(int* values, int count)
{
	// this is so slow we might as well report progress
	printf("%3d%%", 0);
	int percentstep = count / 100;

	int duplicates = 0;
	for (int i = 0; i < count; ++i)
	{
		int value = values[i];
		for (int j = i + 1; j < count; ++j)
		{
			if (value == values[j])
			{
				++duplicates; // there's a duplicate
				break; // stop here to avoid reading more than 1 duplicate at a time
			}
		}
		if (i % percentstep == 0)
			printf("\b\b\b\b%3d%%", (i*100)/count);
	}
	printf("\b\b\b\b"); // erase 100%
	return duplicates;
}

int main(int argc, char** argv)
{
	#define NUM_ELEMENTS 500000
	static int values[NUM_ELEMENTS];
	for (int i = 0; i < NUM_ELEMENTS; ++i)
		values[i] = rand();

	// find duplicates using the histogram method
	printf("Histogram Duplicates: ");
	int duplicates1 = hist_duplicates(values, NUM_ELEMENTS);
	printf("%d / %d (%.2g%%)\n", duplicates1, NUM_ELEMENTS, 100.f * duplicates1 / NUM_ELEMENTS);

	// compared against a classical O(n^2) approach
	printf("Bubble Duplicates:    ");
	int duplicates2 = bubble_duplicates(values, NUM_ELEMENTS);
	printf("%d / %d (%.2g%%)\n", duplicates2, NUM_ELEMENTS, 100.f * duplicates2 / NUM_ELEMENTS);

	system("pause");
	return 0;
}