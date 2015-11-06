/**
* Simple vernam cypher example
* Uses C99 dialect, so compile with -std=gnu99 or -std=c99
* or via IDE-s Visual Studio 2013 (and higher) / DevC++5.7 (and higher)
*/
#include <stdlib.h>
#include <stdio.h>


int get_input(char* buffer, int maxCount)
{
	fgets(buffer, maxCount, stdin); // read up to maxCount chars from console
	fflush(stdin);                  // flush standard input (if some chars were left over)
	int size = strlen(buffer);      // get the length of the input string
	if (buffer[size - 1] == '\n')
		buffer[--size] = '\0';      // remove trailing \n
	return size;
}


int vernam_cypher(int argc, char** argv)
{
	char input[128] = { 0 };
	char cypher[128] = { 0 };

	printf("Text to Encode:  ");
	int inputSize = get_input(input, sizeof input);
	printf("Encoding Cypher: ");
	int cypherSize = get_input(cypher, sizeof cypher);

	// encode the input with a simple xor
	for (int i = 0; i < inputSize; ++i)
		input[i] ^= cypher[i];

	// print encoded text as HEX
	printf("Encoded HEX:  ");
	for (int i = 0; i < inputSize; ++i) printf("0x%02x ", input[i]);
	printf("\n");

	// decode input with the same cypher
	for (int i = 0; i < inputSize; ++i)
		input[i] ^= cypher[i];
	printf("Decoded Text: '%.*s'\n", inputSize, input);

	system("pause");
	return 0;
}