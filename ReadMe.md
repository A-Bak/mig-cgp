# Majority Inverter Graph CGP
## Overview
***

This program implements Cartesian Genetic Programming (CGP) for evolutionary design of logic circuits. Is is implemented using the open-source [CGP-Library](http://www.cgplibrary.co.uk/files2/About-txt.html) in C. The goal is to design logic circuits in the form of Majority Inverter Graph (MIG) which is a directed acyclic graph consisting of Majority nodes (MAJ) with 3 inputs, which output 'High' if majority of inputs are 'High' and 'Low' otherwise. In order to be able to compare use of MAJ to regular logic gates, the program also implements AND, OR, XOR, NAND, NOR, NXOR logic functions.

The inputs of the program are in the form of a truth table of the desired logic function. The generation of these truth tables is included, as the program uses bit parallel simulation on 50 bits to accelerate calculations. The 50b are used instead of traditional 64b due to the CGL-Library using `double` variables in all it's calculations, which are only able to exactly represent integers up to 2^53. Generation of truthtables for a few simple adders and multipliers is included.

The program outputs the best found chromosome for each run of the CGP algorithm and creates it's representation in the form of
a `.dot` file, which is subsequently converted to a `.svg` figure.

## Generating Truth Tables
***
The truth tables are created in the form of 50b bitvectors. Each input and output of the logic function is assigned a bitvector. If the truth table has 50 or fewer rows, each variable requires a single bitvector. Otherwise multiple bitvectors are required for each of the inputs/outputs. 

Some pre-generated truth tables are in folder `data`. If you want to create one for a different logic function, it is necessary to implement the function in `src/combination_circuits.c` and define number of inputs/outputs and function code in its header file. Then you need to create a wrapper for the logic function in `src/cgp_data_creation.c` and add it in function `generateOutputVectors`.

## Parameters of CGP
***
- `NUM_NODES` - Number of function nodes in the combination circuit of candidate solutions
-  `MU` - Number of chosen individuals for reproduction(mutation) - 0 < MU 
- `LAMBDA` - Total number of individuals in a population - MU < LAMBDA 
- `MUTATION_RATE`-  Rate of mutation - <0.0,1.0>	
- `MUTATION_TYPE` - Type of mutation - probabilistic, point, onlyActive, single
- `TARGET_FITNES` - Target value of the fitness function, 0 by default
- `NUM_GENERATIONS` - Total number of generations for CGP
- `NUM_OPTIM_GENERATIONS` - Total number of generations for optimizing the number of nodes in chromosome
- `NUM_RUNS` - Total number of generations for optimizing the number of nodes in chromosome
- `UPDATE_FREQUENCY` - Frequency of update information about the population - 0 < n < `NUM_GENERATIONS`

- `NUM_THREADS` - Number of threads for the computation. 
NOTE: Multiple threads currently not in use, strange behaviour with CGP-Library, producing duplicit results.

## Running the Program

1. Create truth table for your logic function
    * Choose one of the pre-defined functions in `cgp_data_creation.h`
    * (Optionally) Create your own logic function
    * Run `make dataset`
    * New truth table file with bitvectors is created in `data/*logic_function_name*.data`
2. Set parameters for CGP in `cgp_mig.h` and choose your logic function
3. Run `make run`
4. View statistics of the runs. Created solutions can be seen in `out/chromo_N.svg`

