#ifndef CGPMIG
#define CGPMIG

#ifndef PRINT_UINT64
#define PRINT_UINT64 "%"PRIu64" "
#endif

#define BITVECTOR_MASK 0x3FFFFFFFFFFFF


// ====================================================================================================
/*
	Combination logic circuit function for CGP.
	Choose a name from "combination_circuits.h"
	for which you have created a truth table.
*/


// #define COMBINATION_FUNCTION halfAdder
// #define COMBINATION_FUNCTION fullAdder_1bit
// #define COMBINATION_FUNCTION fullAdder_2bit
// #define COMBINATION_FUNCTION fullAdder_3bit
#define COMBINATION_FUNCTION fullAdder_4bit

// #define COMBINATION_FUNCTION multiplier_2bit
// #define COMBINATION_FUNCTION multiplier_3bit
// #define COMBINATION_FUNCTION multiplier_4bit


// ====================================================================================================
// CGP PARAMETERS


#define USE_MAJ_GATES false				   	// Toggle between using majority gates in nodes / using and, or, xor

#define RANDOM_SEED 1234562789

#define NUM_NODES 100			       	// Number of function nodes in the combination circuit of candidate solutions

#define MU 1						   	// Number of chosen individuals for reproduction(mutation) - 0 < MU 
#define LAMBDA 4					   	// Total number of individuals in a population - MU < LAMBDA 

#define MUTATION_RATE 0.025			   	// Rate of mutation - <0.0,1.0>	
#define MUTATION_TYPE "point"          	// Type of mutation - probabilistic, point, onlyActive, single

#define TARGET_FITNESS 0
#define NUM_GENERATIONS 1000000    		// Total number of generations for CGP
#define NUM_OPTIM_GENERATIONS 2000000   	// Total number of generations for optimizing the number of nodes in chromosome
#define NUM_RUNS 5	  		   		   	// Total number of generations for optimizing the number of nodes in chromosome
#define UPDATE_FREQUENCY 10000         	// Frequency of update information about the population - 0<n<NUM_GENERATIONS

// Strange behaviour with the results when using multiple threads => Disabled openMP in CGP-Lib
#define NUM_THREADS 1


// ====================================================================================================

static uint64_t bitMask(uint64_t x, uint64_t mask);
static double _ones(const int numInputs, const double *inputs, const double *connectionWeights);
static double _neg(const int numInputs, const double *inputs, const double *connectionWeights);

static double _maj(const int numInputs, const double *inputs, const double *connectionWeights);
static double _majNeg_0(const int numInputs, const double *inputs, const double *connectionWeights);
static double _majNeg_1(const int numInputs, const double *inputs, const double *connectionWeights);
static double _majNeg_2(const int numInputs, const double *inputs, const double *connectionWeights);
static double _majNeg_01(const int numInputs, const double *inputs, const double *connectionWeights);
static double _majNeg_02(const int numInputs, const double *inputs, const double *connectionWeights);
static double _majNeg_12(const int numInputs, const double *inputs, const double *connectionWeights);
static double _majNeg_012(const int numInputs, const double *inputs, const double *connectionWeights);

static double _and(const int numInputs, const double *inputs, const double *connectionWeights);
static double _or(const int numInputs, const double *inputs, const double *connectionWeights);
static double _xor(const int numInputs, const double *inputs, const double *connectionWeights);
static double _nand(const int numInputs, const double *inputs, const double *connectionWeights);
static double _nor(const int numInputs, const double *inputs, const double *connectionWeights);
static double _xnor(const int numInputs, const double *inputs, const double *connectionWeights);

double customFitness(struct parameters *params, struct chromosome *chromo, struct dataSet *data);
double customFitnessOptimization(struct parameters *params, struct chromosome *chromo, struct dataSet *data);
double calculateError(struct chromosome *chromo, struct dataSet *data);
double sampleError(double value, double expected, uint64_t mask);

void setInitialParamsCGP(struct parameters *params);
void setOptimizationParamsCGP(struct parameters *params);

#endif