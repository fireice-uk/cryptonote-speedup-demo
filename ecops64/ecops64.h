/* 
 * (c) 2017 fireice-uk
 * All rights reserved.
 * Based on Dr Bernstein's public domain code.
 * 
 */

#pragma once
#include <stdlib.h>
#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef uint64_t fe64[4];

typedef struct
{
	fe64 x;
	fe64 y;
	fe64 z;
	fe64 t;
} ge64_p3;

typedef struct
{
	fe64 x;
	fe64 z;
	fe64 y;
	fe64 t;
} ge64_p1p1;

typedef struct
{
	fe64 x;
	fe64 y;
	fe64 z;
} ge64_p2;

typedef struct
{
	fe64 ysubx;
	fe64 xaddy;
	fe64 t2d;
} ge64_niels;

typedef struct
{
	fe64 ysubx;
	fe64 xaddy;
	fe64 z;
	fe64 t2d;
} ge64_pniels;

///// EXTERN ASM
void sV_fe64_mul(fe64 r, const fe64 x, const fe64 y);
void sV_fe64_square(fe64 r, const fe64 x);
void sV_fe64_add(fe64 r, const fe64 x, const fe64 y);
void sV_fe64_sub(fe64 r, const fe64 x, const fe64 y);
void sV_fe64_reduce(fe64 r);
void sV_ge64_add(ge64_p1p1* r, const ge64_p3* p, const ge64_p3* q);
void sV_ge64_p1p1_to_p2(ge64_p2* r, const ge64_p1p1* p);
void sV_ge64_p1p1_to_p3(ge64_p3* r, const ge64_p1p1* p);
void sV_ge64_nielsadd2(ge64_p3 *r, const ge64_niels *q);
void sV_ge64_pnielsadd_p1p1(ge64_p1p1 *r, const ge64_p3 *p, const ge64_pniels *q);
void sV_ge64_p2_dbl(ge64_p1p1 *r, const ge64_p2 *p);

#ifdef _WIN32
void win64_wrapper(void* f, ...);
#define fe64_mul(r, x, y) win64_wrapper(sV_fe64_mul, r, x, y)
#define fe64_square(r, x) win64_wrapper(sV_fe64_square, r, x)
#define fe64_add(r, x, y) win64_wrapper(sV_fe64_add, r, x, y)
#define fe64_sub(r, x, y) win64_wrapper(sV_fe64_sub, r, x, y)
#define fe64_reduce(r) win64_wrapper(sV_fe64_reduce, r)
#define ge64_add(r, p, q) win64_wrapper(sV_ge64_add, r, p, q)
#define ge64_p1p1_to_p2(r, p) win64_wrapper(sV_ge64_p1p1_to_p2, r, p)
#define ge64_p1p1_to_p3(r, p) win64_wrapper(sV_ge64_p1p1_to_p3, r, p)
#define ge64_nielsadd2(r, q) win64_wrapper(sV_ge64_nielsadd2, r, q)
#define ge64_pnielsadd_p1p1(r, p, q) win64_wrapper(sV_ge64_pnielsadd_p1p1, r, p, q)
#define ge64_p2_dbl(r, p) win64_wrapper(sV_ge64_p2_dbl, r, p)
#else
#define fe64_mul sV_fe64_mul
#define fe64_square sV_fe64_square
#define fe64_add sV_fe64_add
#define fe64_sub sV_fe64_sub
#define fe64_reduce sV_fe64_reduce
#define ge64_add sV_ge64_add
#define ge64_p1p1_to_p2 sV_ge64_p1p1_to_p2
#define ge64_p1p1_to_p3 sV_ge64_p1p1_to_p3
#define ge64_nielsadd2 sV_ge64_nielsadd2
#define ge64_pnielsadd_p1p1 sV_ge64_pnielsadd_p1p1
#define ge64_p2_dbl sV_ge64_p2_dbl
#endif // !_WIN32

///// C FUNCTIONS
void ge64_p3_to_p2(ge64_p2* r, const ge64_p3* p);
void ge64_mul8(ge64_p1p1 *r, const ge64_p2 *t);
int  ge64_frombytes_vartime(ge64_p3* h, const unsigned char p[32]);
void ge64_tobytes(unsigned char s[32], const ge64_p2* h);
void sc64_reduce32(unsigned char x[32]);
void ge64_scalarmult_base(ge64_p3 *r, const unsigned char s[32]);
void ge64_scalarmult(ge64_p2 *r, const unsigned char a[32], const ge64_p3 *A);

#ifdef __cplusplus
}
#endif // __cplusplus
