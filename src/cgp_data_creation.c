#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <math.h>

#include "cgp_data_creation.h"
#include "combination_circuits.h"
#include "../lib/cgp_lib/cgp.h"

typedef struct bitVectorArray{
	int numBitVectors;
	int numVariables;
	uint64_t **array; 
	uint64_t * array_data;
} bitVectorArray;

/*
	Allocates and initializes a bitVectorArray of size numBitVectors X numVariables.
	Returns pointer to this bitVectorArray
*/
bitVectorArray *initializeBitVectorArray(int numBitVectors, int numVariables){

	bitVectorArray *bva = (bitVectorArray*) malloc(sizeof(bitVectorArray));

	if(bva == NULL)
		return NULL;

	bva->array = (uint64_t**) malloc(sizeof(uint64_t*) * numBitVectors);

	if(bva->array == NULL){
		free(bva);
		return NULL;
	}

	bva->array_data = (uint64_t*) malloc(sizeof(uint64_t) * numBitVectors * numVariables);

	if(bva->array_data == NULL){
		free(bva);
		free(bva->array);
		return NULL;

	}

	bva->numBitVectors = numBitVectors;
	bva->numVariables = numVariables;
	memset(bva->array_data, 0, sizeof(uint64_t) * numBitVectors * numVariables);

	for(int i = 0; i < numBitVectors; ++i)
		bva->array[i] = bva->array_data + i*numVariables;

	return bva;
}

/*
	Frees memory allocated for the bitVectorArray.
*/
void freeBitVectorArray(bitVectorArray *bva){

	free(bva->array_data);
	free(bva->array);
	free(bva);
}

/*	
	Creates bit vectors for the inputs of the combination circuit.
*/
void generateInputVectors(bitVectorArray *inputs){

	uint64_t  permutation;
	uint64_t permutationNumber = pow(2, inputs->numVariables);

	int bitVector = -1;

	// Given N input variables,  bottom N bits of the uint64_t permutation
	// represent the state of the input variables. All possible vraiable
	// states 2^N can be reached by incrementing permutation.
	for(permutation = 0; permutation < permutationNumber; ++permutation){

		// A bit vector can hold maximum of 64 = (2^6) values.
		// However, CGP_LIB uses double in it's implementation.
		// Double can accurately hold up to 2^53 integer values
		// => Start filing new bitVector after 50 bits to be safe
		if(permutation % 50 == 0){
			bitVector++;
		}

		uint64_t inputPosition = 1;

		for(int i = 0; i < inputs->numVariables; ++i){

			inputs->array[bitVector][i] = updateBitVector(permutation, inputPosition, inputs->array[bitVector][i]);

			inputPosition = inputPosition << 1;
		}
	}

}

/*
	Adds new bit value to the bit vector.
*/
uint64_t updateBitVector(uint64_t value, uint64_t bitPosition, uint64_t bitVector){

		uint64_t result = bitVector << 1;

		uint64_t bitSet = value & bitPosition;

		if(bitSet > 0)
			result += 1;	

		return result;
}

/*	
	Creates bit vectors for the outputs of the combination circuit from the input bit vectors and a combination function.
*/
void generateOutputVectors(bitVectorArray *inputs, bitVectorArray *outputs){

	if(inputs->numBitVectors != outputs->numBitVectors){
		printf("Error: Mismatched number of bitVectors in inputs and outputs");

		freeBitVectorArray(inputs);
		freeBitVectorArray(outputs);
		exit(0);
	}

	for(int i = 0; i < inputs->numBitVectors; ++i){

		switch(COMBINATION_FUNCTION_CODE){

			case halfAdder_FUNCTION_CODE:
				_halfAdder(inputs->array[i], outputs->array[i]);
				break;

			case fullAdder_1bit_FUNCTION_CODE:
				_fullAdder_1bit(inputs->array[i], outputs->array[i]);
				break;

			case fullAdder_2bit_FUNCTION_CODE:
				_fullAdder_2bit(inputs->array[i], outputs->array[i]);
				break;

			case fullAdder_3bit_FUNCTION_CODE:
				 _fullAdder_3bit(inputs->array[i], outputs->array[i]);
				break;

			case fullAdder_4bit_FUNCTION_CODE:
				_fullAdder_4bit(inputs->array[i], outputs->array[i]);
				break;	

			case multiplier_2bit_FUNCTION_CODE:
				_multiplier_2bit(inputs->array[i], outputs->array[i]);
				break;	

			case multiplier_3bit_FUNCTION_CODE:
				_multiplier_3bit(inputs->array[i], outputs->array[i]);
				break;	

			case multiplier_4bit_FUNCTION_CODE:
				_multiplier_4bit(inputs->array[i], outputs->array[i]);
				break;	
		}
	}
}

void _halfAdder(uint64_t *inputs, uint64_t *outputs){

	uint64_t *a0 = &inputs[0];
	uint64_t *b0 = &inputs[1];

	uint64_t *s0 = &outputs[0];
	uint64_t *c_out = &outputs[1];

	halfAdder(a0,b0,
		      s0,c_out);
}

void _fullAdder_1bit(uint64_t *inputs, uint64_t *outputs){

	uint64_t *a0 = &inputs[0];
	uint64_t *b0 = &inputs[1];
	uint64_t *c_in = &inputs[2];

	uint64_t *s0 = &outputs[0];
	uint64_t *c_out = &outputs[1];

	fullAdder_1bit(a0,b0,c_in,s0,c_out);
}

void _fullAdder_2bit(uint64_t *inputs, uint64_t *outputs){

	uint64_t *a0 = &inputs[0];
	uint64_t *a1 = &inputs[1];
	uint64_t *b0 = &inputs[2];
	uint64_t *b1 = &inputs[3];
	uint64_t *c_in = &inputs[4];

	uint64_t *s0 = &outputs[0];
	uint64_t *s1 = &outputs[1];
	uint64_t *c_out = &outputs[2];

	fullAdder_2bit(a0,a1,
		           b0,b1,c_in,
		           s0,s1,c_out);
}

void _fullAdder_3bit(uint64_t *inputs, uint64_t *outputs){

	uint64_t *a0 = &inputs[0];
	uint64_t *a1 = &inputs[1];
	uint64_t *a2 = &inputs[2];
	uint64_t *b0 = &inputs[3];
	uint64_t *b1 = &inputs[4];
	uint64_t *b2 = &inputs[5];
	uint64_t *c_in = &inputs[6];

	uint64_t *s0 = &outputs[0];
	uint64_t *s1 = &outputs[1];
	uint64_t *s2 = &outputs[2];
	uint64_t *c_out = &outputs[3];

	fullAdder_3bit(a0,a1,a2,
		           b0,b1,b2,c_in,
		           s0,s1,s2,c_out);
}

void _fullAdder_4bit(uint64_t *inputs, uint64_t *outputs){

	uint64_t *a0 = &inputs[0];
	uint64_t *a1 = &inputs[1];
	uint64_t *a2 = &inputs[2];
	uint64_t *a3 = &inputs[3];
	uint64_t *b0 = &inputs[4];
	uint64_t *b1 = &inputs[5];
	uint64_t *b2 = &inputs[6];
	uint64_t *b3 = &inputs[7];
	uint64_t *c_in = &inputs[8];

	uint64_t *s0 = &outputs[0];
	uint64_t *s1 = &outputs[1];
	uint64_t *s2 = &outputs[2];
	uint64_t *s3 = &outputs[3];
	uint64_t *c_out = &outputs[4];

	fullAdder_4bit(a0,a1,a2,a3,
		           b0,b1,b2,b3,c_in,
		           s0,s1,s2,s3,c_out);
}


void _multiplier_2bit(uint64_t *inputs, uint64_t *outputs){

	uint64_t *a0 = &inputs[0];
	uint64_t *a1 = &inputs[1];
	uint64_t *b0 = &inputs[2];
	uint64_t *b1 = &inputs[3];

	uint64_t *m0 = &outputs[0];
	uint64_t *m1 = &outputs[1];
	uint64_t *m2 = &outputs[2];
	uint64_t *m3 = &outputs[3];

	multiplier_2bit(a0,a1,
		            b0,b1,
		            m0,m1,m2,m3);
}


void _multiplier_3bit(uint64_t *inputs, uint64_t *outputs){

	uint64_t *a0 = &inputs[0];
	uint64_t *a1 = &inputs[1];
	uint64_t *a2 = &inputs[2];
	uint64_t *b0 = &inputs[3];
	uint64_t *b1 = &inputs[4];
	uint64_t *b2 = &inputs[5];

	uint64_t *m0 = &outputs[0];
	uint64_t *m1 = &outputs[1];
	uint64_t *m2 = &outputs[2];
	uint64_t *m3 = &outputs[3];
	uint64_t *m4 = &outputs[4];
	uint64_t *m5 = &outputs[5];

	multiplier_3bit(a0,a1,a2,
		            b0,b1,b2,
		            m0,m1,m2,m3,m4,m5);
}


void _multiplier_4bit(uint64_t *inputs, uint64_t *outputs){

	uint64_t *a0 = &inputs[0];
	uint64_t *a1 = &inputs[1];
	uint64_t *a2 = &inputs[2];
	uint64_t *a3 = &inputs[3];
	uint64_t *b0 = &inputs[4];
	uint64_t *b1 = &inputs[5];
	uint64_t *b2 = &inputs[6];
	uint64_t *b3 = &inputs[7];

	uint64_t *m0 = &outputs[0];
	uint64_t *m1 = &outputs[1];
	uint64_t *m2 = &outputs[2];
	uint64_t *m3 = &outputs[3];
	uint64_t *m4 = &outputs[4];
	uint64_t *m5 = &outputs[5];
	uint64_t *m6 = &outputs[6];
	uint64_t *m7 = &outputs[7];

	multiplier_4bit(a0,a1,a2,a3,
		            b0,b1,b2,b3,
		            m0,m1,m2,m3,m4,m5,m6,m7);

}


void saveTable(bitVectorArray *inputs, bitVectorArray *outputs, char *filename){

	FILE *f = fopen(filename, "w");

	if(f == NULL){
		printf("Error: Failed to open file");

		freeBitVectorArray(inputs);
		freeBitVectorArray(outputs);
		exit(0);
	}

	if(inputs->numBitVectors != outputs->numBitVectors){
		printf("Error: Mismatched number of bitVectors in inputs and outputs");

		freeBitVectorArray(inputs);
		freeBitVectorArray(outputs);
		exit(0);
	}

	fprintf(f, "%d,%d,%d,\n", inputs->numVariables, outputs->numVariables, inputs->numBitVectors);
	for(int i = 0; i < inputs->numBitVectors; ++i){

		for (int j = 0; j < inputs->numVariables; ++j)
		{
			fprintf(f, "%"PRIu64",", inputs->array[i][j]);
		}

		for (int j = 0; j < outputs->numVariables; ++j)
		{
			fprintf(f, "%"PRIu64",", outputs->array[i][j]);
		}

		fprintf(f, "\n");
	}
}

void printTable(bitVectorArray *bva){

	for(int i = 0; i < bva->numBitVectors; ++i){

		for(int j = 0; j < bva->numVariables; ++j)
			printf("%"PRIu64" ", bva->array[i][j]);
		
		printf("\n");
	}
}


// =========================================================================================================


int main(){

	int numInputs = NUM_INPUTS;
	int numOutputs = NUM_OUTPUTS;

	int numBitVectors = ceil(pow(2,numInputs) / 50);

	bitVectorArray *inputs = initializeBitVectorArray(numBitVectors,numInputs);
	bitVectorArray *outputs = initializeBitVectorArray(numBitVectors,numOutputs);

	generateInputVectors(inputs);
	generateOutputVectors(inputs, outputs);

	// printTable(inputs);
	// printTable(outputs);

	saveTable(inputs, outputs, SAVEFILE_PATH);

	freeBitVectorArray(inputs);
	freeBitVectorArray(outputs);
	return 0;
}


