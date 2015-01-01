#ifndef DYABL_FPN_Q32_32_H__
#define DYABL_FPN_Q32_32_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

typedef int64_t Q32_32;

/* CONSTANTS */
#define Q32_32_BIT 64
#define Q32_32_HALF_BIT 32
#define Q32_32_HALF_MAX ((1L << Q32_32_HALF_BIT) - 1L)
#define Q32_32_MAX 0x7fffffffffffffff
#define Q32_32_MIN (-0x7fffffffffffffffLL - 1)
#define Q32_32_ONE (1L << Q32_32_HALF_BIT)
#define Q32_32_MASK 0x8000000000000000
#define Q32_32_PI 0x00000003243F6A89
#define Q32_32_2PI 0x00000006487ED511
#define Q32_32_PI_2 0x00000001921FB544
#define Q32_32_E 0x00000002B7E15163
#define Q32_32_D 4294967296.0 /* 2^32 */
#define Q32_32_IsNeg(a) ((a) < 0 ? true : false)

/* Conversion */
Q32_32 Q32_32_from_double(double v);
double Q32_32_to_double(Q32_32 v);
Q32_32 Q32_32_from_int(int32_t v);
int32_t Q32_32_to_int(Q32_32 v);
Q32_32 Q32_32_copySign(Q32_32 a, Q32_32 sign_of);
Q32_32 Q32_32_sign(Q32_32 a);
void Q32_32_modf(Q32_32 a, Q32_32 *int_part, Q32_32 *float_part);
Q32_32 Q32_32_ceil(Q32_32 a);
Q32_32 Q32_32_floor(Q32_32 a);
bool Q32_32_near(Q32_32 a, Q32_32 b, unsigned int tolerance);

/* Arithmetic */
Q32_32 Q32_32_abs(Q32_32 a);
Q32_32 Q32_32_add(Q32_32 a, Q32_32 b);
Q32_32 Q32_32_add_noof(Q32_32 a, Q32_32 b, bool *overflow);
Q32_32 Q32_32_sat_add(Q32_32 a, Q32_32 b);
Q32_32 Q32_32_sub(Q32_32 a, Q32_32 b);
Q32_32 Q32_32_sub_noof(Q32_32 a, Q32_32 b, bool *overflow);
Q32_32 Q32_32_sat_sub(Q32_32 a, Q32_32 b);
Q32_32 Q32_32_mul(Q32_32 a, Q32_32 b);
Q32_32 Q32_32_mul_noof(Q32_32 a, Q32_32 b, bool *overflow);
Q32_32 Q32_32_sat_mul(Q32_32 a, Q32_32 b);
Q32_32 Q32_32_div(Q32_32 a, Q32_32 b);
Q32_32 Q32_32_div_noof(Q32_32 a, Q32_32 b, bool *overflow);
/* using binary long division */
Q32_32 Q32_32_mod(Q32_32 a, Q32_32 b);
void Q32_32_divmod(Q32_32 a, Q32_32 b, Q32_32 *div, Q32_32 *mod);

/* Advance */
/* http://www.embedded.com/electronics-blogs/programmer-s-toolbox/4219659/Integer-Square-Roots
 */
Q32_32 Q32_32_sqrt(Q32_32 a);
Q32_32 Q32_32_ipow(Q32_32 a, unsigned int p);
Q32_32 Q32_32_pow(Q32_32 a, unsigned int p);
Q32_32 Q32_32_ipow_noof(Q32_32 a, unsigned int p, bool *overflow);
Q32_32 Q32_32_pow_noof(Q32_32 a, unsigned int p, bool *overflow);
Q32_32 Q32_32_fact(Q32_32 a);
Q32_32 Q32_32_fact_noof(Q32_32 a, bool *overflow);
Q32_32 Q32_32_sq(Q32_32 a);   /* square */
Q32_32 Q32_32_cube(Q32_32 a); /* cube */
Q32_32 Q32_32_exp(Q32_32 a);
Q32_32 Q32_32_ilog(Q32_32 a, Q32_32 base); /* integer log base */
Q32_32 Q32_32_ilg(Q32_32 a);               /* integer log 10 */
Q32_32 Q32_32_log(Q32_32 a, Q32_32 base);  /* log base */
Q32_32 Q32_32_lg(Q32_32 a);                /* log 10 */
Q32_32 Q32_32_lb(Q32_32 a);                /* log 2 */
Q32_32 Q32_32_ln(Q32_32 a);                /* log neperien */

Q32_32 Q32_32_sat_log2(Q32_32 a);

/* Trigonometry */
#define Q32_32_RAD2DEG 0
#define Q32_32_DEG2RAD 0
void Q32_32_cordic(Q32_32 angle, unsigned int n, Q32_32 *s, Q32_32 *c);
Q32_32 Q32_32_sin(Q32_32 a);
Q32_32 Q32_32_cos(Q32_32 a);
Q32_32 Q32_32_tan(Q32_32 a);
bool Q32_32_asin(Q32_32 a, Q32_32 *r);
bool Q32_32_acos(Q32_32 a, Q32_32 *r);
Q32_32 Q32_32_atan(Q32_32 a);
bool Q32_32_atan2(Q32_32 y, Q32_32 x, Q32_32 *r);
/* hyperbolic */
Q32_32 Q32_32_sinh(Q32_32 a);
Q32_32 Q32_32_cosh(Q32_32 a);
Q32_32 Q32_32_tanh(Q32_32 a);
Q32_32 Q32_32_atanh(Q32_32 a);

#ifdef __cplusplus
}

#endif

#endif
