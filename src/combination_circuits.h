#ifndef COMBINATION_CIRCUITS
#define COMBINATION_CIRCUITS

#ifndef JOIN
#define JOIN1(x, y) x ## y
#define JOIN(x, y) JOIN1(x, y)
#endif

#define COMBINATION_FUNCTION_CODE JOIN(COMBINATION_FUNCTION, _FUNCTION_CODE)
#define NUM_INPUTS JOIN(COMBINATION_FUNCTION, _NUM_INPUTS)
#define NUM_OUTPUTS JOIN(COMBINATION_FUNCTION, _NUM_OUTPUTS)

#define halfAdder_FUNCTION_CODE 0
#define halfAdder_NUM_INPUTS 2
#define halfAdder_NUM_OUTPUTS 2

#define fullAdder_1bit_FUNCTION_CODE 1
#define fullAdder_1bit_NUM_INPUTS 3
#define fullAdder_1bit_NUM_OUTPUTS 2

#define fullAdder_2bit_FUNCTION_CODE 2
#define fullAdder_2bit_NUM_INPUTS 5
#define fullAdder_2bit_NUM_OUTPUTS 3

#define fullAdder_3bit_FUNCTION_CODE 3
#define fullAdder_3bit_NUM_INPUTS 7
#define fullAdder_3bit_NUM_OUTPUTS 4

#define fullAdder_4bit_FUNCTION_CODE 4
#define fullAdder_4bit_NUM_INPUTS 9
#define fullAdder_4bit_NUM_OUTPUTS 5

#define multiplier_2bit_FUNCTION_CODE 5
#define multiplier_2bit_NUM_INPUTS 4
#define multiplier_2bit_NUM_OUTPUTS 4

#define multiplier_3bit_FUNCTION_CODE 6
#define multiplier_3bit_NUM_INPUTS 6
#define multiplier_3bit_NUM_OUTPUTS 6

#define multiplier_4bit_FUNCTION_CODE 7
#define multiplier_4bit_NUM_INPUTS 8
#define multiplier_4bit_NUM_OUTPUTS 8


#ifndef TO_STR
#define TO_STR1(x) #x
#define TO_STR(x) TO_STR1(x)
#endif

#define SAVEFILE_PATH "data/"TO_STR(COMBINATION_FUNCTION)".data"


void halfAdder(const uint64_t *a, const uint64_t *b, uint64_t *s, uint64_t *c);

void fullAdder_1bit(const uint64_t *a, const uint64_t *b, const uint64_t *c_in, uint64_t *s, uint64_t *c_out);

void fullAdder_2bit(const uint64_t *a0, const uint64_t *a1,
					const uint64_t *b0, const uint64_t *b1, const uint64_t *c_in,
					uint64_t *s0, uint64_t *s1, uint64_t *c_out);

void fullAdder_3bit(const uint64_t *a0, const uint64_t *a1, const uint64_t *a2,
					const uint64_t *b0, const uint64_t *b1, const uint64_t *b2, const uint64_t *c_in,
					uint64_t *s0, uint64_t *s1, uint64_t *s2, uint64_t *c_out);

void fullAdder_4bit(const uint64_t *a0, const uint64_t *a1, const uint64_t *a2, const uint64_t *a3,
					const uint64_t *b0, const uint64_t *b1, const uint64_t *b2, const uint64_t *b3, const uint64_t *c_in,
					uint64_t *s0, uint64_t *s1, uint64_t *s2, uint64_t *s3, uint64_t *c_out);

void multiplier_2bit(const uint64_t *a0, const uint64_t *a1,
					 const uint64_t *b0, const uint64_t *b1,
				     uint64_t *m0, uint64_t *m1, uint64_t *m2, uint64_t *m3);

void multiplier_3bit(const uint64_t *a0, const uint64_t *a1, const uint64_t *a2,
                     const uint64_t *b0, const uint64_t *b1, const uint64_t *b2,
				     uint64_t *m0, uint64_t *m1, uint64_t *m2, uint64_t *m3, uint64_t *m4, uint64_t *m5);

void multiplier_4bit(const uint64_t *a0, const uint64_t *a1, const uint64_t *a2, const uint64_t *a3,
                     const uint64_t *b0, const uint64_t *b1, const uint64_t *b2, const uint64_t *b3,
				     uint64_t *m0, uint64_t *m1, uint64_t *m2, uint64_t *m3, uint64_t *m4, uint64_t *m5, uint64_t *m6, uint64_t *m7);

#endif