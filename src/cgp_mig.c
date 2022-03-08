#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <inttypes.h>
#include <time.h>

#include <assert.h>

#include "../lib/cgp_lib/cgp.h"

#include "cgp_mig.h"
#include "combination_circuits.h"



#define NODE_ARITY 3	  // 3 due to using MAJ logic function nodes
#define WEIGHT_RANGE 0	  // Not necessary, only used when using CGP on neural networks

#define DATA_SAMPLE_BITS 50

struct results{
	int numRuns;
	struct chromosome **bestChromosomes;
};

// ====================================================================================================


static uint64_t bitMask(uint64_t x, uint64_t mask){

	return x & mask;
}

static double _zeros(const int numInputs, const double *inputs, const double *connectionWeights){

	return (double) 0;
}

static double _ones(const int numInputs, const double *inputs, const double *connectionWeights){

	return (double) BITVECTOR_MASK;
}

static double _neg(const int numInputs, const double *inputs, const double *connectionWeights){

	uint64_t result = ~((uint64_t) inputs[0]);

	return (double) bitMask(result, BITVECTOR_MASK);
}

static double _maj(const int numInputs, const double *inputs, const double *connectionWeights){

	uint64_t a = inputs[0];
	uint64_t b = inputs[1];
	uint64_t c = inputs[2];

	uint64_t result = (a&b)|(a&c)|(b&c);

	return (double) bitMask(result, BITVECTOR_MASK);
}

static double _majNeg_0(const int numInputs, const double *inputs, const double *connectionWeights){

	uint64_t a = ~ (uint64_t) inputs[0];
	uint64_t b = inputs[1];
	uint64_t c = inputs[2];

	uint64_t result = (a&b)|(a&c)|(b&c);

	return (double) bitMask(result, BITVECTOR_MASK);
}

static double _majNeg_1(const int numInputs, const double *inputs, const double *connectionWeights){

	uint64_t a = inputs[0];
	uint64_t b = ~ (uint64_t) inputs[1];
	uint64_t c = inputs[2];

	uint64_t result = (a&b)|(a&c)|(b&c);

	return (double) bitMask(result, BITVECTOR_MASK);
}

static double _majNeg_2(const int numInputs, const double *inputs, const double *connectionWeights){

	uint64_t a = inputs[0];
	uint64_t b = inputs[1];
	uint64_t c = ~ (uint64_t) inputs[2];

	uint64_t result = (a&b)|(a&c)|(b&c);

	return (double) bitMask(result, BITVECTOR_MASK);
}

static double _majNeg_01(const int numInputs, const double *inputs, const double *connectionWeights){

	uint64_t a = ~ (uint64_t) inputs[0];
	uint64_t b = ~ (uint64_t) inputs[1];
	uint64_t c = inputs[2];

	uint64_t result = (a&b)|(a&c)|(b&c);

	return (double) bitMask(result, BITVECTOR_MASK);
}

static double _majNeg_02(const int numInputs, const double *inputs, const double *connectionWeights){

	uint64_t a = ~ (uint64_t) inputs[0];
	uint64_t b = inputs[1];
	uint64_t c = ~ (uint64_t) inputs[2];

	uint64_t result = (a&b)|(a&c)|(b&c);

	return (double) bitMask(result, BITVECTOR_MASK);
}

static double _majNeg_12(const int numInputs, const double *inputs, const double *connectionWeights){

	uint64_t a = inputs[0];
	uint64_t b = ~ (uint64_t) inputs[1];
	uint64_t c = ~ (uint64_t) inputs[2];

	uint64_t result = (a&b)|(a&c)|(b&c);

	return (double) bitMask(result, BITVECTOR_MASK);
}

static double _majNeg_012(const int numInputs, const double *inputs, const double *connectionWeights){

	uint64_t a = ~ (uint64_t) inputs[0];
	uint64_t b = ~ (uint64_t) inputs[1];
	uint64_t c = ~ (uint64_t) inputs[2];

	uint64_t result = (a&b)|(a&c)|(b&c);

	return (double) bitMask(result, BITVECTOR_MASK);
}

static double _and(const int numInputs, const double *inputs, const double *connectionWeights){

	uint64_t a = inputs[0];
	uint64_t b = inputs[1];

	uint64_t result = a & b;

	return (double) bitMask(result, BITVECTOR_MASK);
}

static double _or(const int numInputs, const double *inputs, const double *connectionWeights){

	uint64_t a = inputs[0];
	uint64_t b = inputs[1];

	uint64_t result = a | b;

	return (double) bitMask(result, BITVECTOR_MASK);	
}

static double _xor(const int numInputs, const double *inputs, const double *connectionWeights){

	uint64_t a = inputs[0];
	uint64_t b = inputs[1];

	uint64_t result = a ^ b;

	return (double) bitMask(result, BITVECTOR_MASK);	
}

static double _nand(const int numInputs, const double *inputs, const double *connectionWeights){

	uint64_t a = inputs[0];
	uint64_t b = inputs[1];

	uint64_t result = ~(a & b);

	return (double) bitMask(result, BITVECTOR_MASK);
}

static double _nor(const int numInputs, const double *inputs, const double *connectionWeights){

	uint64_t a = inputs[0];
	uint64_t b = inputs[1];

	uint64_t result = ~(a | b);

	return (double) bitMask(result, BITVECTOR_MASK);	
}

static double _xnor(const int numInputs, const double *inputs, const double *connectionWeights){

	uint64_t a = inputs[0];
	uint64_t b = inputs[1];

	uint64_t result = ~(a ^ b);

	return (double) bitMask(result, BITVECTOR_MASK);	
}


// ====================================================================================================


/*
	Custom fitness function for CGP. Calculates the number of erroneous bits of the combination logic function
	represented by the chromosome compared to referential combination logic function.
*/
double customFitness(struct parameters *params, struct chromosome *chromo, struct dataSet *data){

	if(getNumChromosomeInputs(chromo) !=getNumDataSetInputs(data)){
	 	printf("Error: the number of chromosome inputs must match the number of inputs specified in the dataSet.\n");
		printf("Terminating.\n");
	 	exit(0);
	}

	if(getNumChromosomeOutputs(chromo) != getNumDataSetOutputs(data)){
		printf("Error: the number of chromosome outputs must match the number of outputs specified in the dataSet.\n");
		printf("Terminating.\n");
		exit(0);
	}

	double error = calculateError(chromo, data);

	return error;
}

/*
	Custom fitness function for CGP chromosome optimization. Calculates the number of erroneous bits of the
	combination logic function represented by the chromosome compared to referential combination logic function.
	If the number is zero, the function takes into account the number of active nodes in the chromosome.
*/
double customFitnessOptimization(struct parameters *params, struct chromosome *chromo, struct dataSet *data){

	if(getNumChromosomeInputs(chromo) !=getNumDataSetInputs(data)){
	 	printf("Error: the number of chromosome inputs must match the number of inputs specified in the dataSet.\n");
		printf("Terminating.\n");
	 	exit(0);
	}

	if(getNumChromosomeOutputs(chromo) != getNumDataSetOutputs(data)){
		printf("Error: the number of chromosome outputs must match the number of outputs specified in the dataSet.\n");
		printf("Terminating.\n");
		exit(0);
	}

	double error = calculateError(chromo, data);
		
	if(error == TARGET_FITNESS){

		return error -= NUM_NODES - getNumChromosomeActiveNodes(chromo);
	}

	return error;
}

/*
	Function calculates the total number of mismatched bits between the bitvector outputs of the chromosome and
	expected bitvector outputs.
*/
double calculateError(struct chromosome *chromo, struct dataSet *data){

	// Total number of entries(rows) of the truth table
	int totalDataBits = pow(2, NUM_INPUTS);
	// Functions process bit vectors of size at most 50
	// as double can hold accurate integer values up to 2^53
	int	sampleDataBits = DATA_SAMPLE_BITS;
	uint64_t sampleBitMask;
	
	double error = 0;

	for(int i = 0; i < getNumDataSetSamples(data); ++i){

		if(sampleDataBits*(i+1) < totalDataBits){
			sampleBitMask = BITVECTOR_MASK;
		}
		else{
			sampleBitMask = ((uint64_t)1 << totalDataBits % 50) - 1;
		}

		executeChromosome(chromo, getDataSetSampleInputs(data, i));
		double *expected_outputs = getDataSetSampleOutputs(data, i);

		for(int j = 0; j < getNumChromosomeOutputs(chromo); ++j){

			error += sampleError(getChromosomeOutput(chromo, j), expected_outputs[j], sampleBitMask);
		}
	}

	return error;
}

/*
	Function compares two bitvectors and returns the total number of mismatched bits.
*/
double sampleError(double value, double expected, uint64_t mask){

	uint64_t error = bitMask((uint64_t)value ^ (uint64_t)expected, mask);

	return __builtin_popcount(error);
}

/*
	Set parameters of the CGP. 
*/
void setInitialParamsCGP(struct parameters *params){

	setMu(params, MU);
	setLambda(params, LAMBDA);

	addCustomNodeFunction(params, _zeros, "ZEROS", 0);
	addCustomNodeFunction(params, _ones, "ONES", 0);
	addCustomNodeFunction(params, _neg, "NEG", 1);
	
	if(USE_MAJ_GATES){
		addCustomNodeFunction(params, _maj, "MAJ", 3);
		addCustomNodeFunction(params, _majNeg_0, "MAJ!0", 3);
		addCustomNodeFunction(params, _majNeg_1, "MAJ!1", 3);
		addCustomNodeFunction(params, _majNeg_2, "MAJ!2", 3);
		addCustomNodeFunction(params, _majNeg_01, "MAJ!01", 3);
		addCustomNodeFunction(params, _majNeg_02, "MAJ!02", 3);
		addCustomNodeFunction(params, _majNeg_12, "MAJ!12", 3);
		addCustomNodeFunction(params, _majNeg_012, "MAJ!012", 3);
	}
	else{
		addCustomNodeFunction(params, _and, "AND", 2);
		addCustomNodeFunction(params, _or, "OR", 2);
		addCustomNodeFunction(params, _xor, "XOR", 2);

		// addCustomNodeFunction(params, _nand, "NAND", 2);
		// addCustomNodeFunction(params, _nor, "NOR", 2);
		// addCustomNodeFunction(params, _xnor, "XNOR", 2);
	}

	setTargetFitness(params, TARGET_FITNESS);
	setCustomFitnessFunction(params, customFitness, "customFitness");
	
	setMutationRate(params, MUTATION_RATE);
	setMutationType(params, MUTATION_TYPE);

	setUpdateFrequency(params, UPDATE_FREQUENCY);
	setConnectionWeightRange(params, WEIGHT_RANGE);
}

// ====================================================================================================


int main(){

	// setRandomNumberSeed(RANDOM_SEED);

	// int seed = time(NULL);
	// setRandomNumberSeed(seed);

	// printf("----------------------------------------------------\n");
	// printf("Random seed: %d\n", seed);

	struct parameters *params = initialiseParameters(NUM_INPUTS, NUM_NODES, NUM_OUTPUTS, NODE_ARITY); 	
	struct dataSet *data = initialiseDataSetFromFile(SAVEFILE_PATH);;

	setNumThreads(params, NUM_THREADS);
	setInitialParamsCGP(params);

	// Find solution with the same logic function
	struct results *res = repeatCGP_WithOptim(params, data, NUM_GENERATIONS, NUM_OPTIM_GENERATIONS, NUM_RUNS, customFitnessOptimization);
	char output_file_name[25];
	char dot_shell_command[100];

	printf("Converting chromosome '.dot' files to '.svg'.\n");
	printf("----------------------------------------------------\n");
	for(int i = 0; i < NUM_RUNS; i++){
		
		sprintf(output_file_name, "out/chromo_%d.dot", i);
		
		// printChromosome(res->bestChromosomes[i], 1);
		
		removeInactiveNodes(res->bestChromosomes[i]);
		
		sprintf(dot_shell_command, "dot -Tsvg out/chromo_%d.dot -o out/chromo_%d.svg", i, i);
		
		saveChromosomeDot(res->bestChromosomes[i], 0, output_file_name);
		system(dot_shell_command);

		// freeChromosome(res->bestChromosomes[i]);
	}

	freeResults(res);
	freeParameters(params);
	freeDataSet(data);
	return 0;
}



