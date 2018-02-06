#ifndef GOR_BIGNUM_H_INCLUDED
#define GOR_BIGNUM_H_INCLUDED

/*
	ABOUT:
		Big number library - arithmetic on multiple-precision unsigned integers.

		This library is an implementation of arithmetic on arbitrarily large integers.

		The difference between this and other implementations, is that the data structure
		has optimal memory utilization (i.e. a 1024 bit integer takes up 128 bytes RAM),
		and all memory is allocated statically: no dynamic allocation for better or worse.

		Primary goals are correctness, clarity of code and clean, portable implementation.
		Secondary goal is a memory footprint small enough to make it suitable for use in
		embedded applications.

		The current state is correct functionality and adequate performance.
		There may well be room for performance-optimizations and improvements.

	USAGE:
		

	LICENCE:
		This is free and unencumbered software released into the public domain.

		Anyone is free to copy, modify, publish, use, compile, sell, or
		distribute this software, either in source code form or as a compiled
		binary, for any purpose, commercial or non-commercial, and by any
		means.

		In jurisdictions that recognize copyright laws, the author or authors
		of this software dedicate any and all copyright interest in the
		software to the public domain. We make this dedication for the benefit
		of the public at large and to the detriment of our heirs and
		successors. We intend this dedication to be an overt act of
		relinquishment in perpetuity of all present and future rights to this
		software under copyright law.

		THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
		EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
		MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
		IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
		OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
		ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
		OTHER DEALINGS IN THE SOFTWARE.

		For more information, please refer to <https://unlicense.org>

	AUTHOR:
		Gorevoy Dmitry - github.com/gorevojd

	OTHER:
		Basic code was taken from https://github.com/kokke/tiny-bignum-c
		and then modified.
*/

#include <stdint.h>

#define GORBN_SZWORD 2
#define GORBN_SZARR (256 / GORBN_SZWORD)

#ifndef GORBN_SZWORD
#error GORBN_SZWORD must be defined
#elif (GORBN_SZWORD == 1)
#define gorbn_t uint8_t
#define gorbn_utmp_t uint32_t
#define gorbn_stmp_t int32_t
#define GORBN_MAX_VAL 0xFF
#define GORBN_SZWORD_BITS 8
#define GORBN_SZWORD_BITS_MINUS_ONE 7

#elif (GORBN_SZWORD == 2)
#define gorbn_t uint16_t
#define gorbn_utmp_t uint32_t
#define gorbn_stmp_t int32_t
#define GORBN_MAX_VAL 0xFFFF
#define GORBN_SZWORD_BITS 16
#define GORBN_SZWORD_BITS_MINUS_ONE 15

#elif (GORBN_SZWORD == 4)
#define gorbn_t uint32_t
#define gorbn_utmp_t uint64_t
#define gorbn_stmp_t int64_t
#define GORBN_MAX_VAL 0xFFFFFFFF
#define GORBN_SZWORD_BITS 32
#define GORBN_SZWORD_BITS_MINUS_ONE 31

#elif
#error GORBN_SZWORD must be defined to 1, 2 or 4
#endif

typedef struct gorbn_bignum{
	int sign;

	gorbn_t number[GORBN_SZARR];
} gorbn_bignum;

/* Custom macro for getting absolute value of the signed integer*/
#define GORBN_ABS(val) (((val) >= 0) ? (val) : (-(val)))

/*Custom macro for getting the biggest number from two numbers*/
#define GORBN_MAX(a, b) (((a) > (b)) ? (a) : (b))

/* Custom assert macro */
#ifdef GOR_BIGNUM_ENABLE_ASSERTS
#define GORBN_ASSERT(cond) if(!(cond)){*(int*)0 = 0;}
#else
#define GORBN_ASSERT(...)
#endif

/*Tokens returned by gorbn_cmp() for value comparison*/
#define GORBN_CMP_LARGER 1
#define GORBN_CMP_SMALLER -1
#define GORBN_CMP_EQUAL 0

#ifdef GOR_BIGNUM_STATIC
#define GORBN_DEF static
#else
#define GORBN_DEF extern
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*Initialization functions*/
GORBN_DEF void gorbn_init(gorbn_bignum* n);
GORBN_DEF void gorbn_copy(gorbn_bignum* dst, gorbn_bignum* src);
GORBN_DEF void gorbn_from_uint(gorbn_bignum* n, gorbn_utmp_t i);
GORBN_DEF void gorbn_from_int(gorbn_bignum* n, gorbn_stmp_t i);
GORBN_DEF void gorbn_from_data(gorbn_bignum* n, void* data, uint32_t data_size);
GORBN_DEF void gorbn_to_data(void* data, uint32_t data_size, gorbn_bignum* n);

/* Arithmetic operations: */
GORBN_DEF void gorbn_add(gorbn_bignum* r, gorbn_bignum* a, gorbn_bignum* b); /* r = a + b */
GORBN_DEF void gorbn_sub(gorbn_bignum* r, gorbn_bignum* a, gorbn_bignum* b); /* r = a - b */
GORBN_DEF void gorbn_mul(gorbn_bignum* r, gorbn_bignum* a, gorbn_bignum* b); /* r = a * b */
GORBN_DEF void gorbn_sqr(gorbn_bignum* r, gorbn_bignum* a);                  /* r = a ^ 2 */
GORBN_DEF void gorbn_div(gorbn_bignum* r, gorbn_bignum* a, gorbn_bignum* b); /* r = a / b */
GORBN_DEF void gorbn_mod(gorbn_bignum* r, gorbn_bignum* a, gorbn_bignum* b); /* r = a % b */
GORBN_DEF void gorbn_pow(gorbn_bignum* r, gorbn_bignum* a, gorbn_bignum* b);
GORBN_DEF void gorbn_mul_pow2(gorbn_bignum* r, gorbn_bignum* a, int k); /* r = a * (2 ^ k) */

/* Bitwise operations: */
GORBN_DEF void gorbn_and(gorbn_bignum* r, gorbn_bignum* a, gorbn_bignum* b); /* r = a & b */
GORBN_DEF void gorbn_or(gorbn_bignum* r, gorbn_bignum* a, gorbn_bignum* b); /* r = a | b */
GORBN_DEF void gorbn_xor(gorbn_bignum* r, gorbn_bignum* a, gorbn_bignum* b); /* r = a ^ b */
GORBN_DEF void gorbn_lshift(gorbn_bignum* r, gorbn_bignum* a, int nbits); /* r = a << nbits */
GORBN_DEF void gorbn_rshift(gorbn_bignum* r, gorbn_bignum* b, int nbits); /* r = a >> nbits */

/* Special operators and comparison */
GORBN_DEF int gorbn_cmp(gorbn_bignum* a, gorbn_bignum* b);
GORBN_DEF int gorbn_cmp_mod(gorbn_bignum* a, gorbn_bignum* b);
GORBN_DEF int gorbn_cmp_zero(gorbn_bignum* a);
GORBN_DEF int gorbn_is_zero(gorbn_bignum* a);


#ifdef __cplusplus
}
#endif

#endif

#if defined(GOR_BIGNUM_IMPLEMENTATION) && !defined(GOR_BIGNUM_IMPLEMENTATION_DONE)
#define GOR_BIGNUM_IMPLEMENTATION_DONE

void gorbn_init(gorbn_bignum* n){
	GORBN_ASSERT(n);

	int i = 0;
	for(i = 0; i < GORBN_SZARR; i++){
		n->number[i] = 0;
	}
	n->sign = 1;
}

void gorbn_copy(gorbn_bignum* dst, gorbn_bignum* src){
	GORBN_ASSERT(dst);
	GORBN_ASSERT(src);

	int i;
	for(i = 0; i < GORBN_SZARR; i++){
		dst->number[i] = src->number[i];
	}
	dst->sign = src->sign;
}

void gorbn_from_uint(gorbn_bignum* n, gorbn_utmp_t i){
	GORBN_ASSERT(n);

	gorbn_init(n);

#if (GORBN_SZWORD == 1)
	n->number[0] = (i & 0x000000ff);
	n->number[1] = (i & 0x0000ff00) >> 8;
	n->number[2] = (i & 0x00ff0000) >> 16;
	n->number[3] = (i & 0xff000000) >> 24;
#elif (GORBN_SZWORD == 2)
	n->number[0] = i;
	n->number[1] = i >> 16;
#elif (GORBN_SZWORD == 4)
	n->number[0] = i;
	n->number[1] = i >> 32;
#endif

	n->sign = 1;
}

void gorbn_from_int(gorbn_bignum* n, gorbn_stmp_t i){
	GORBN_ASSERT(n);

	gorbn_init(n);

	n->sign = 1;
	if(i < 0){
		n->sign = -1;
	}

	i = GORBN_ABS(i);
	
#if (GORBN_SZWORD == 1)
	n->number[0] = (i & 0x000000ff);
	n->number[1] = (i & 0x0000ff00) >> 8;
	n->number[2] = (i & 0x00ff0000) >> 16;
	n->number[3] = (i & 0xff000000) >> 24;
#elif (GORBN_SZWORD == 2)
	n->number[0] = i;
	n->number[1] = i >> 16;
#elif (GORBN_SZWORD == 4)
	n->number[0] = i;
	n->number[1] = i >> 32;
#endif
}

void gorbn_from_data(gorbn_bignum* n, void* data, uint32_t data_size){
	GORBN_ASSERT(n);
	GORBN_ASSERT(data);

	int i;

	uint8_t* at = (uint8_t*)data;
	uint8_t* to = (uint8_t*)n->number;

	gorbn_init(n);

	for(i = 0; i < data_size; i++){
		*to++ = *at++;
	}
}

void gorbn_to_data(void* data, uint32_t data_size, gorbn_bignum* n){
	GORBN_ASSERT(data);
	GORBN_ASSERT(n);
	GORBN_ASSERT((GORBN_SZARR * GORBN_SZWORD) <= data_size); /*data must be big enough to hold big number inside*/

	int i;

	gorbn_t* at = n->number;
	gorbn_t* to = (gorbn_t*)data;

	for(i = 0; i < GORBN_SZARR; i++){
		*to++ = *at++;
	}
}

static inline void _gorbn_lshift_one_bit(gorbn_bignum* a) {
	GORBN_ASSERT(a);

	int i;
	for (i = (GORBN_SZARR - 1); i > 0; --i) {
		a->number[i] = (a->number[i] << 1) | (a->number[i - 1] >> GORBN_SZWORD_BITS_MINUS_ONE);
	}
	a->number[0] <<= 1;
}

static inline void _gorbn_rshift_one_bit(gorbn_bignum* a) {
	GORBN_ASSERT(a);

	int i;
	for (i = 0; i < (GORBN_SZARR - 1); i++) {
		a->number[i] = (a->number[i] >> 1) | (a->number[i + 1] << GORBN_SZWORD_BITS_MINUS_ONE);
	}
	a->number[GORBN_SZARR - 1] >>= 1;
}

static inline void _gorbn_lshift_word(gorbn_bignum* a, int nwords) {
	GORBN_ASSERT(a);
	GORBN_ASSERT(nwords >= 0);

	int i;
	for (i = (GORBN_SZARR - 1); i >= nwords; i--) {
		a->number[i] = a->number[i - nwords];
	}

	for (; i >= 0; --i) {
		a->number[i] = 0;
	}
}

static inline void _gorbn_rshift_word(gorbn_bignum* a, int nwords) {
	GORBN_ASSERT(a);
	GORBN_ASSERT(nwords >= 0);

	int i;
	for (i = 0; i < nwords; i++) {
		a->number[i] = a->number[i + nwords];
	}

	for (; i < GORBN_SZARR; i++) {
		a->number[i] = 0;
	}
}

static inline int _gorbn_get_ndigits(gorbn_bignum* a) {
	int result = 0;

	for (int i = GORBN_SZARR - 1; i >= 0; i--) {
		if (a->number[i] != 0) {
			result = i + 1;
			break;
		}
	}

	return(result);
}

static inline void _gorbn_internal_addition(
	gorbn_bignum* dst,
	gorbn_bignum* a, int a_sign,
	gorbn_bignum* b, int b_sign)
{
	gorbn_utmp_t res;
	gorbn_utmp_t tmp1;
	gorbn_utmp_t tmp2;
	int borrow = 0;
	int carry = 0;
	int i;

	gorbn_bignum* big;
	gorbn_bignum* low;

	if(a_sign == b_sign){
		/* Just sum those 2 values */
		for(i = 0; i < GORBN_SZARR; i++){
			tmp1 = a->number[i] + b->number[i] + carry;
			carry = (tmp1 > GORBN_MAX_VAL);
			dst->number[i] = (tmp1 & GORBN_MAX_VAL);
		}

		/* Set the initial sign */
		dst->sign = a_sign;
	}
	else{
		/* Detecting which number is bigger */
		int comp_res = gorbn_cmp_mod(a, b);
		if(comp_res >= 0){
			big = a;
			low = b;
		}
		else{
			big = b;
			low = a;
		}

		/* Subtracting: dst = big - small */
		for (i = 0; i < GORBN_SZARR; ++i) {
			tmp1 = (gorbn_utmp_t)big->number[i] + ((gorbn_utmp_t)GORBN_MAX_VAL + 1);
			tmp2 = (gorbn_utmp_t)low->number[i] + borrow;
			res = (tmp1 - tmp2);
			dst->number[i] = (gorbn_t)(res & GORBN_MAX_VAL);
			borrow = (res <= GORBN_MAX_VAL);
		}

		/* Detecting the result sign */
		if(a_sign > 0 && comp_res >= 0){
			dst->sign = 1;
		}
		else{
			dst->sign = -1;
		}
	}
}

void gorbn_add(gorbn_bignum* r, gorbn_bignum* a, gorbn_bignum* b){
	GORBN_ASSERT(r);
	GORBN_ASSERT(a);
	GORBN_ASSERT(b);

	_gorbn_internal_addition(r, a, a->sign, b, b->sign);
}

void gorbn_sub(gorbn_bignum* r, gorbn_bignum* a, gorbn_bignum* b){
	GORBN_ASSERT(r);
	GORBN_ASSERT(a);
	GORBN_ASSERT(b);

	_gorbn_internal_addition(r, a, a->sign, b, b->sign * -1);
}

/* 
	NOTE: 
		Performs multiplication on 'a' and 'b' of size 'n' and 't',
		and stores the result in 'r'. 'r' must be cleared to zero 
		before this function call.
*/
static inline void _gorbn_internal_mul(
	gorbn_t* r, 
	gorbn_t* x, int n, 
	gorbn_t* y, int t)
{
	GORBN_ASSERT(r);
	GORBN_ASSERT(a);
	GORBN_ASSERT(b);

	GORBN_ASSERT(n + t + 1 <= GORBN_SZARR)

	int i, j;

	for (i = 0; i < t; i++) {
		gorbn_utmp_t uv;
		gorbn_utmp_t c = 0;

		for (j = 0; j < n; j++) {
			uv = (gorbn_utmp_t)r[i + j] + 
				(gorbn_utmp_t)x[j] * 
				(gorbn_utmp_t)y[i] + 
				c;
			r[i + j] = uv & GORBN_MAX_VAL;
			c = (uv >> GORBN_SZWORD_BITS) & GORBN_MAX_VAL;
		}

		r[i + n + 1] = c;
	}
}

void gorbn_mul(gorbn_bignum* r, gorbn_bignum* a, gorbn_bignum* b) {
	GORBN_ASSERT(r);
	GORBN_ASSERT(a);
	GORBN_ASSERT(b);

	int i, j;
	int a_ndigits;
	int b_ndigits;

	a_ndigits = _gorbn_get_ndigits(a);
	b_ndigits = _gorbn_get_ndigits(b);

	gorbn_bignum buf;
	gorbn_init(&buf);
	
#if 0
	for (i = 0; i < a_ndigits; ++i) {
		gorbn_utmp_t carry_lo = 0;
		gorbn_utmp_t carry_hi = 0;
		gorbn_utmp_t sum_hi = 0;
		gorbn_utmp_t sum_lo;

		for (j = 0; j < b_ndigits; ++j) {
			sum_lo =
				(gorbn_utmp_t)a->number[i] *
				(gorbn_utmp_t)b->number[j] +
				carry_lo;

			sum_hi =
				(gorbn_utmp_t)buf.number[i + j] +
				(sum_lo & GORBN_MAX_VAL) +
				carry_hi;

			buf.number[i + j] = sum_hi & GORBN_MAX_VAL;

			carry_lo = (sum_lo >> GORBN_SZWORD_BITS) & GORBN_MAX_VAL;
			carry_hi = (sum_hi >> GORBN_SZWORD_BITS) & GORBN_MAX_VAL;
		}

		/* Continue processing carry values */
#if 0
		do {
			/*TODO: Should we increment here????*/
			sum_hi = (gorbn_utmp_t)buf.number[i + j] + carry_lo + carry_hi;
			buf.number[i + j] = sum_hi & GORBN_MAX_VAL;
			carry_hi = (sum_hi >> GORBN_SZWORD_BITS) & GORBN_MAX_VAL;
			carry_lo = 0;
		} while (carry_hi != 0);
#else
		sum_hi = (gorbn_utmp_t)buf.number[i + j] + carry_lo + carry_hi;
		buf.number[i + j] = sum_hi & GORBN_MAX_VAL;
#endif
	}
#else
	for (i = 0; i < b_ndigits; i++) {
		gorbn_utmp_t uv;
		gorbn_utmp_t c = 0;

		for (j = 0; j < a_ndigits; j++) {
			uv = (gorbn_utmp_t)buf.number[i + j] + 
				(gorbn_utmp_t)a->number[j] * 
				(gorbn_utmp_t)b->number[i] + 
				c;
			buf.number[i + j] = uv & GORBN_MAX_VAL;
			c = (uv >> GORBN_SZWORD_BITS) & GORBN_MAX_VAL;
		}

		buf.number[i + a_ndigits + 1] = c;
	}
#endif

	gorbn_copy(r, &buf);
}

#if 0
void gorbn_sqr(gorbn_bignum* r, gorbn_bignum* a) {
	GORBN_ASSERT(r);
	GORBN_ASSERT(a);

	gorbn_bignum buf;
	gorbn_init(&buf);

	int t = _gorbn_get_ndigits(a);

	int i;
	for (i = 0; i < t; i++) {
		gorbn_utmp_t carry;
		gorbn_utmp_t temp_val = a->number[i];
		gorbn_utmp_t sum = (gorbn_utmp_t)buf.number[2 * i] + temp_val * temp_val;
		buf.number[2 * i] = sum & GORBN_MAX_VAL;
		carry = (sum >> GORBN_SZWORD_BITS) & GORBN_MAX_VAL;

		int j;
		for (j = i + 1; j < t; j++) {
			sum = 
				(gorbn_utmp_t)buf.number[i + j] + 2 * 
				(gorbn_utmp_t)a->number[i] * 
				(gorbn_utmp_t)a->number[j] + 
				carry;
			buf.number[i + j] = sum & GORBN_MAX_VAL;
			carry = (sum >> GORBN_SZWORD_BITS) & GORBN_MAX_VAL;
		}

		buf.number[i + t] = carry;
	}

	gorbn_copy(r, &buf);
}
#endif

void gorbn_div(gorbn_bignum* r, gorbn_bignum* a, gorbn_bignum* b) {
	GORBN_ASSERT(r);
	GORBN_ASSERT(a);
	GORBN_ASSERT(b);

#if 0
	gorbn_bignum current, denom, tmp;

	gorbn_from_uint(&current, 1);
	gorbn_copy(&denom, b);
	gorbn_copy(&tmp, a);

	gorbn_utmp_t half_max = 1 + (GORBN_MAX_VAL >> 1);
	int overflow = 0;
	while (gorbn_cmp(&denom, a) != GORBN_CMP_LARGER) {
		if (denom.number[GORBN_SZARR - 1] >= half_max) {
			overflow = 1;
			break;
		}
		_gorbn_lshift_one_bit(&current);
		_gorbn_lshift_one_bit(&denom);
	}

	if (!overflow) {
		_gorbn_rshift_one_bit(&denom);
		_gorbn_rshift_one_bit(&current);
	}

	gorbn_init(r);

	while (!gorbn_is_zero(&current)) {
		if (gorbn_cmp(&tmp, &denom) != GORBN_CMP_SMALLER) {
			gorbn_sub(&tmp, &tmp, &denom);
			gorbn_or(r, r, &current);
		}

		_gorbn_rshift_one_bit(&current);
		_gorbn_rshift_one_bit(&denom);
	}
#else

#endif
}

void gorbn_and(gorbn_bignum* r, gorbn_bignum* a, gorbn_bignum* b) {
	GORBN_ASSERT(r);
	GORBN_ASSERT(a);
	GORBN_ASSERT(b);

	int walk_count = GORBN_SZARR;

	int i;
	for (i = 0; i < walk_count; i++) {
		r->number[i] = a->number[i] | b->number[i];
	}
}

void gorbn_or(gorbn_bignum* r, gorbn_bignum* a, gorbn_bignum* b) {
	GORBN_ASSERT(r);
	GORBN_ASSERT(a);
	GORBN_ASSERT(b);

	int walk_count = GORBN_SZARR;

	int i;
	for (i = 0; i < walk_count; i++) {
		r->number[i] = a->number[i] ^ b->number[i];
	}
}

void gorbn_xor(gorbn_bignum* r, gorbn_bignum* a, gorbn_bignum* b) {
	GORBN_ASSERT(r);
	GORBN_ASSERT(a);
	GORBN_ASSERT(b);

	int walk_count = GORBN_SZARR;

	int i;
	for (i = 0; i < walk_count; i++) {
		r->number[i] = a->number[i] & b->number[i];
	}
}

void gorbn_lshift(gorbn_bignum* r, gorbn_bignum* a, int nbits) {
	GORBN_ASSERT(r);
	GORBN_ASSERT(a);

	
}

void gorbn_rshift(gorbn_bignum* r, gorbn_bignum* b, int nbits) {
	GORBN_ASSERT(r);
	GORBN_ASSERT(a);


}

int gorbn_cmp(gorbn_bignum* a, gorbn_bignum* b){
	GORBN_ASSERT(a);
	GORBN_ASSERT(b);

	int i;
	int res = GORBN_CMP_EQUAL;
	int mod_res = 0;

	if(a->sign != b->sign){
		res = (a->sign > b->sign) ? GORBN_CMP_LARGER : GORBN_CMP_SMALLER;
	}
	else{
		i = GORBN_SZARR;
		do{
			i -= 1; /* Decrement first, to start with last array element */
			if (a->number[i] > b->number[i])
			{
				mod_res = GORBN_CMP_LARGER;
				break;
			}
			else if (a->number[i] < b->number[i])
			{
				mod_res = GORBN_CMP_SMALLER;
				break;
			}
		} while(i != 0);

		res = mod_res * a->sign;
	}

	return(res);
}

int gorbn_cmp_mod(gorbn_bignum* a, gorbn_bignum* b){
	GORBN_ASSERT(a);
	GORBN_ASSERT(b);

	int i = GORBN_SZARR;
	do{
		i -= 1; /* Decrement first, to start with last array element */
		if (a->number[i] > b->number[i])
		{
			return GORBN_CMP_LARGER;
		}
		else if (a->number[i] < b->number[i])
		{
			return GORBN_CMP_SMALLER;
		}
	} while(i != 0);

	return(GORBN_CMP_EQUAL);
}

int gorbn_is_zero(gorbn_bignum* a) {
	GORBN_ASSERT(a);

	int res = 1;

	int i;
	for (i = 0; i < GORBN_SZARR; i++) {
		if (a->number[i]) {
			res = 0;
			break;
		}
	}

	return(res);
}

#endif