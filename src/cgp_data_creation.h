#ifndef CGP_DATA_CREATION
#define CGP_DATA_CREATION

#ifndef PRINT_UINT64
#define PRINT_UINT64 "%"PRIu64" "
#endif


/*
	Change the name of this function to generate a truth table for
	a different combination logic function. 		
*/
// #define COMBINATION_FUNCTION halfAdder
// #define COMBINATION_FUNCTION fullAdder_1bit
// #define COMBINATION_FUNCTION fullAdder_2bit
// #define COMBINATION_FUNCTION fullAdder_3bit
// #define COMBINATION_FUNCTION fullAdder_4bit

// #define COMBINATION_FUNCTION multiplier_2bit
// #define COMBINATION_FUNCTION multiplier_3bit
#define COMBINATION_FUNCTION multiplier_4bit


typedef struct bitVectorArray bitVectorArray;


bitVectorArray *initializeBitVectorArray(int numBitVectors, int numVariables);
void freeBitVectorArray(bitVectorArray *bva);


void generateInputVectors(bitVectorArray *inputs);
uint64_t updateBitVector(uint64_t value, uint64_t bitPosition, uint64_t bitVector);


void generateOutputVectors(bitVectorArray *inputs, bitVectorArray *outputs);
void _halfAdder(uint64_t *inputs, uint64_t *outputs);
void _fullAdder_1bit(uint64_t *inputs, uint64_t *outputs);
void _fullAdder_2bit(uint64_t *inputs, uint64_t *outputs);
void _fullAdder_3bit(uint64_t *inputs, uint64_t *outputs);
void _fullAdder_4bit(uint64_t *inputs, uint64_t *outputs);

void _multiplier_2bit(uint64_t *inputs, uint64_t *outputs);
void _multiplier_3bit(uint64_t *inputs, uint64_t *outputs);
void _multiplier_4bit(uint64_t *inputs, uint64_t *outputs);

void saveTable(bitVectorArray *inputs, bitVectorArray *outputs, char *filename);

void printTable(bitVectorArray *array);

#endif