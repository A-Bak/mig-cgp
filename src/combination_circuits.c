#include <inttypes.h>


void and(const uint64_t *a, const uint64_t *b, uint64_t *ab){

	*ab = *a & *b;
}

void halfAdder(const uint64_t *a, const uint64_t *b, uint64_t *s, uint64_t *c){

	*s = *a ^ *b;
	*c = *a & *b;
}

void fullAdder_1bit(const uint64_t *a, const uint64_t *b, const uint64_t *c_in, uint64_t *s, uint64_t *c_out){

	uint64_t s0;
	uint64_t c0;
	uint64_t c1;

	halfAdder(a, b, &s0, &c0);

	halfAdder(&s0, c_in, s, &c1);

	*c_out = c0 | c1;
}


void fullAdder_2bit(const uint64_t *a0, const uint64_t *a1, const uint64_t *b0, const uint64_t *b1,
				    const uint64_t *c_in, uint64_t *s0, uint64_t *s1, uint64_t *c_out){

	uint64_t c0;

	fullAdder_1bit(a0, b0, c_in, s0, &c0);
	fullAdder_1bit(a1, b1, &c0, s1, c_out);

}

void fullAdder_3bit(const uint64_t *a0, const uint64_t *a1, const uint64_t *a2,
					const uint64_t *b0, const uint64_t *b1, const uint64_t *b2, const uint64_t *c_in,
					uint64_t *s0, uint64_t *s1, uint64_t *s2, uint64_t *c_out){

	uint64_t c0;
	uint64_t c1;

	fullAdder_1bit(a0, b0, c_in, s0, &c0);
	fullAdder_1bit(a1, b1, &c0, s1, &c1);
	fullAdder_1bit(a2, b2, &c1, s2, c_out);
}

void fullAdder_4bit(const uint64_t *a0, const uint64_t *a1, const uint64_t *a2, const uint64_t *a3,
					const uint64_t *b0, const uint64_t *b1, const uint64_t *b2, const uint64_t *b3, const uint64_t *c_in,
					uint64_t *s0, uint64_t *s1, uint64_t *s2, uint64_t *s3, uint64_t *c_out){

	uint64_t c0;
	uint64_t c1;
	uint64_t c2;

	fullAdder_1bit(a0, b0, c_in, s0, &c0);
	fullAdder_1bit(a1, b1, &c0, s1, &c1);
	fullAdder_1bit(a2, b2, &c1, s2, &c2);
	fullAdder_1bit(a3, b3, &c2, s3, c_out);
}

void multiplier_2bit(const uint64_t *a0, const uint64_t *a1,
					 const uint64_t *b0, const uint64_t *b1,
				     uint64_t *m0, uint64_t *m1, uint64_t *m2, uint64_t *m3){
	uint64_t c0;

	uint64_t a0b1;
	uint64_t a1b0;
	uint64_t a1b1;	

	and(a0, b0, m0);
	and(a0, b1, &a0b1);
	and(a1, b0, &a1b0);
	and(a1, b1, &a1b1);

	halfAdder(&a0b1, &a1b0, m1, &c0);
	halfAdder(&a1b1, &c0, m2, m3);
}

void multiplier_3bit(const uint64_t *a0, const uint64_t *a1, const uint64_t *a2,
                     const uint64_t *b0, const uint64_t *b1, const uint64_t *b2,
				     uint64_t *m0, uint64_t *m1, uint64_t *m2, uint64_t *m3, uint64_t *m4, uint64_t *m5){

	uint64_t zero = 0;

	uint64_t c0;

	uint64_t s1;
	uint64_t s2;

	uint64_t a0b1;
	uint64_t a0b2;

	uint64_t a1b0;
	uint64_t a1b1;	
	uint64_t a1b2;	

	uint64_t a2b0;	
	uint64_t a2b1;	
	uint64_t a2b2;	

	// m0
	and(a0, b0, m0);
	and(a0, b1, &a0b1);
	and(a0, b2, &a0b2);

	and(a1, b0, &a1b0);
	and(a1, b1, &a1b1);
	and(a1, b2, &a1b2);

	and(a2, b0, &a2b0);
	and(a2, b1, &a2b1);
	and(a2, b2, &a2b2);

	// m1
	fullAdder_3bit(&a1b0, &a2b0, &zero,
				   &a0b1, &a1b1, &a2b1, &zero,
				   m1, &s1, &s2, &c0);

	fullAdder_3bit(&a0b2, &a1b2, &a2b2,
				   &s1, &s2, &c0, &zero,
				   m2, m3, m4, m5);
}

void multiplier_4bit(const uint64_t *a0, const uint64_t *a1, const uint64_t *a2, const uint64_t *a3,
                     const uint64_t *b0, const uint64_t *b1, const uint64_t *b2, const uint64_t *b3,
				     uint64_t *m0, uint64_t *m1, uint64_t *m2, uint64_t *m3, uint64_t *m4, uint64_t *m5, uint64_t *m6, uint64_t *m7){

	uint64_t zero = 0;

	uint64_t c0;
	uint64_t c1;

	uint64_t s1;
	uint64_t s2;
	uint64_t s3;

	uint64_t s4;
	uint64_t s5;
	uint64_t s6;

	uint64_t a0b1;
	uint64_t a0b2;
	uint64_t a0b3;

	uint64_t a1b0;
	uint64_t a1b1;	
	uint64_t a1b2;	
	uint64_t a1b3;	

	uint64_t a2b0;	
	uint64_t a2b1;	
	uint64_t a2b2;	
	uint64_t a2b3;	

	uint64_t a3b0;	
	uint64_t a3b1;	
	uint64_t a3b2;	
	uint64_t a3b3;	

	// m0
	and(a0, b0, m0);
	and(a0, b1, &a0b1);
	and(a0, b2, &a0b2);
	and(a0, b3, &a0b3);

	and(a1, b0, &a1b0);
	and(a1, b1, &a1b1);
	and(a1, b2, &a1b2);
	and(a1, b3, &a1b3);

	and(a2, b0, &a2b0);
	and(a2, b1, &a2b1);
	and(a2, b2, &a2b2);
	and(a2, b3, &a2b3);

	and(a3, b0, &a3b0);
	and(a3, b1, &a3b1);
	and(a3, b2, &a3b2);
	and(a3, b3, &a3b3);

	// m1
	fullAdder_4bit(&a0b1, &a1b1, &a2b1, &a3b1,
				   &a1b0, &a2b0, &a3b0, &zero, &zero,
				   m1, &s1, &s2, &s3, &c0);

	// m2
	fullAdder_4bit(&s1, &s2, &s3, &c0,
			   	   &a0b2, &a1b2, &a2b2, &a3b2, &zero,
			       m2, &s4, &s5, &s6, &c1);

	fullAdder_4bit(&s4, &s5, &s6, &c1,
			   	   &a0b3, &a1b3, &a2b3, &a3b3, &zero,
			       m3, m4, m5, m6, m7);
}