#include "Q32_32.h"
#include <math.h>
#include <stdio.h>

static int64_t Q32_32_hi(Q32_32 v) { return (v) >> Q32_32_HALF_BIT; }

static uint64_t Q32_32_lo(Q32_32 v) { return (v)&Q32_32_HALF_MAX; }

Q32_32
Q32_32_from_double(double v) {
  return (Q32_32)(v * Q32_32_D + (v < 0 ? -0.5 : 0.5));
}

double Q32_32_to_double(Q32_32 v) { return ((double)v / Q32_32_D); }

Q32_32
Q32_32_from_int(int32_t v) { return (Q32_32)(v) << Q32_32_HALF_BIT; }

int32_t Q32_32_to_int(Q32_32 v) { return (int32_t)(v >> Q32_32_HALF_BIT); }

static Q32_32 Q32_32_float_part(Q32_32 a) { return (a) & (Q32_32_HALF_MAX); }

Q32_32
Q32_32_ceil(Q32_32 a) {
  Q32_32 part = Q32_32_float_part(a) ? Q32_32_ONE : 0;

  return Q32_32_add(Q32_32_floor(a), part);
}

Q32_32
Q32_32_floor(Q32_32 a) { return (a & ~(Q32_32_HALF_MAX)); }

void Q32_32_modf(Q32_32 a, Q32_32 *int_part, Q32_32 *float_part) {
  Q32_32 f = Q32_32_float_part(a);

  if (Q32_32_IsNeg(a)) {
    *int_part = Q32_32_ceil(a);
    *float_part = f > 0 ? f - Q32_32_ONE : 0;
  } else {
    *int_part = Q32_32_floor(a);
    *float_part = f;
  }
}

/* Arithmetic */
Q32_32
Q32_32_abs(Q32_32 a) { return ((a < 0) ? -a : a); }

Q32_32
Q32_32_add(Q32_32 a, Q32_32 b) { return a + b; }

Q32_32
Q32_32_add_noof(Q32_32 a, Q32_32 b, bool *overflow) {
  uint64_t _a = (uint64_t)a;
  uint64_t _b = (uint64_t)b;
  uint64_t sum = _a + _b;

  *overflow = ((~(_a ^ _b)) & (_a ^ sum) & Q32_32_MASK);

  return a + b;
}

Q32_32
Q32_32_sat_add(Q32_32 a, Q32_32 b) {
  bool of;
  Q32_32 sum = Q32_32_add_noof(a, b, &of);

  if (of) {
    return (a > 0 ? Q32_32_MAX : Q32_32_MIN);
  }
  return sum;
}

Q32_32
Q32_32_sub(Q32_32 a, Q32_32 b) { return a - b; }

Q32_32
Q32_32_sub_noof(Q32_32 a, Q32_32 b, bool *overflow) {
  uint64_t _a = (uint64_t)a;
  uint64_t _b = (uint64_t)b;
  uint64_t diff = _a - _b;

  *overflow = (((_a ^ _b)) & (_a ^ diff) & Q32_32_MASK);

  return a - b;
}

Q32_32
Q32_32_sat_sub(Q32_32 a, Q32_32 b) {
  bool of;
  Q32_32 diff = Q32_32_sub_noof(a, b, &of);

  if (of) {
    return (a > 0 ? Q32_32_MAX : Q32_32_MIN);
  }
  return diff;
}

Q32_32
Q32_32_mul_noof(Q32_32 a, Q32_32 b, bool *overflow) {
  *overflow = false;
  /* Using Long multiplication */
  /* Split to both 32bits half-words */
  /* lo(a)|hi(a) -> TY */
  /* lo(b)|hi(b) -> GH */

  /* h*y */
  /* h*t + g*y */
  /* g*t */
  /* hi = gt + hi(ht_gy) */
  /* lo = hy + lo(ht_gy) */
  /* prod = hi|lo */
  int64_t h = Q32_32_hi(b);
  int64_t y = Q32_32_hi(a);
  uint64_t g = Q32_32_lo(b);
  uint64_t t = Q32_32_lo(a);
  uint64_t gt = g * t; /* lowbits */
  int64_t hy = h * y;  /* highbits */

  int64_t ht_gy = (int64_t)((uint64_t)h * t + g * (uint64_t)y);

  int64_t hi = hy + (ht_gy >> Q32_32_HALF_BIT);

  uint64_t ht_gy_temp = (uint64_t)ht_gy << Q32_32_HALF_BIT;
  uint64_t lo = gt + ht_gy_temp;

  /* handle carry */
  if (lo < gt) {
    hi++;
  }

  if ((hi >> (Q32_32_BIT - 1)) != (hi >> (Q32_32_HALF_BIT - 1))) {
    *overflow = true;
  }
  return (hi << Q32_32_HALF_BIT) | (lo >> Q32_32_HALF_BIT);
}

Q32_32
Q32_32_mul(Q32_32 a, Q32_32 b) {
  bool of;

  return Q32_32_mul_noof(a, b, &of);
}

Q32_32
Q32_32_sat_mul(Q32_32 a, Q32_32 b) {
  bool of;
  Q32_32 mul = Q32_32_mul_noof(a, b, &of);

  int64_t a_sign = (int64_t)((uint64_t)a & Q32_32_MASK);
  int64_t b_sign = (int64_t)((uint64_t)b & Q32_32_MASK);
  bool is_neg = (a_sign ^ b_sign) != 0;

  if (of) {
    return (is_neg ? Q32_32_MIN : Q32_32_MAX);
  }

  return mul;
}

Q32_32
Q32_32_div_noof(Q32_32 a, Q32_32 b, bool *overflow) {
  *overflow = false;
  /* adapted from libfixmath */
  int64_t a_sign = (int64_t)((uint64_t)a & Q32_32_MASK);
  int64_t b_sign = (int64_t)((uint64_t)b & Q32_32_MASK);
  bool is_neg = (a_sign ^ b_sign) != 0;

  if (b == 0) {
    return is_neg ? Q32_32_MIN : Q32_32_MAX;
  }

  Q32_32 r = Q32_32_abs(a);
  Q32_32 d = Q32_32_abs(b);

  Q32_32 q = Q32_32_from_int(0);
  Q32_32 bit = 0x100000000;

  while (d < r) {
    d <<= 1;
    bit <<= 1;
    if (!bit) {
      *overflow = true;
      if ((a >= 0) == (b >= 0)) {
        return Q32_32_MAX;
      }
      { return Q32_32_MIN; }
    }
  }

  if ((int64_t)((uint64_t)d & Q32_32_MASK)) {
    if (r >= d) {
      q |= bit;
      r -= d;
    }
    d >>= 1;
    bit >>= 1;
  }
  /* main division loop */
  while (bit && r) {
    if (r >= d) {
      q |= bit;
      r -= d;
    }
    r <<= 1;
    bit >>= 1;
  }
  return is_neg ? -q : q;
}

Q32_32
Q32_32_div(Q32_32 a, Q32_32 b) {
  bool of;

  return Q32_32_div_noof(a, b, &of);
}

Q32_32
Q32_32_mod(Q32_32 a, Q32_32 b) {
  Q32_32 m = a;
  Q32_32 d = b;

  while (d <= m && d <= (Q32_32)Q32_32_HALF_MAX) {
    d <<= 1;
  }

  while (m >= b) {
    while (d > m) {
      d >>= 1;
    }
    m = Q32_32_sub(m, d);
  }

  return m;
}

void Q32_32_divmod(Q32_32 a, Q32_32 b, Q32_32 *div, Q32_32 *mod) {
  Q32_32 q = 0;
  Q32_32 m = a;
  Q32_32 d = b;

  while (d <= m && d <= (Q32_32)Q32_32_HALF_MAX) {
    d <<= 1;
  }

  while (m >= b) {
    while (d > m) {
      d >>= 1;
      q <<= 1;
    }

    m = Q32_32_sub(m, d);
    q = Q32_32_add(q, Q32_32_ONE);
  }

  while (d != b) {
    q <<= 1;
    d >>= 1;
  }

  *div = q;
  *mod = m;
}

/* Advance */
Q32_32
Q32_32_sqrt(Q32_32 a) {
  if (a < 0) {
    return Q32_32_MIN;
  }
  Q32_32 num = a;
  Q32_32 result = 0;
  Q32_32 bit = 1LL << (Q32_32_BIT - 2);

  while (bit > num) {
    bit >>= 2;
  }

  for (int n = 0; n < 2; n++) {
    while (bit) {
      if (num >= result + bit) {
        num -= result + bit;
        result = (result >> 1) + bit;
      } else {
        result = (result >> 1);
      }
      bit >>= 2;
    }
    if (n == 0) {
      if (num > (Q32_32)Q32_32_HALF_MAX) {
        num -= result;
        num = (num << Q32_32_HALF_BIT) - (Q32_32)Q32_32_HALF_MAX;
        result = (result << Q32_32_HALF_BIT) + (Q32_32)Q32_32_HALF_MAX;
      } else {
        num <<= Q32_32_HALF_BIT;
        result <<= Q32_32_HALF_BIT;
      }
      bit = 1 << (Q32_32_HALF_BIT - 2);
    }
  }

  return result;
}

Q32_32
Q32_32_sq(Q32_32 a) { return Q32_32_mul(a, a); }

Q32_32
Q32_32_cube(Q32_32 a) { return Q32_32_mul(Q32_32_sq(a), a); }

Q32_32
Q32_32_pow_noof(Q32_32 a, unsigned int p, bool *overflow) {
  *overflow = false;
  if (p == 0) {
    return Q32_32_ONE;
  }

  Q32_32 res = a;

  for (unsigned int i = 1; i < p; i++) {
    Q32_32 tmp = Q32_32_mul_noof(res, a, overflow);
    if (*overflow) {
      return res;
    }
    res = tmp;
  }

  return res;
}

Q32_32
Q32_32_pow(Q32_32 a, unsigned int p) {
  if (p == 0) {
    return Q32_32_ONE;
  }

  Q32_32 res = a;

  for (unsigned int i = 1; i < p; i++) {
    res = Q32_32_mul(res, a);
  }

  return res;
}

Q32_32
Q32_32_ipow_noof(Q32_32 a, unsigned int p, bool *overflow) {
  *overflow = false;
  if (p == 0) {
    return Q32_32_ONE;
  }

  Q32_32 res = a;

  for (unsigned int i = 1; i < p; i++) {
    Q32_32 tmp = Q32_32_mul_noof(res, a, overflow);
    if (*overflow) {
      return res;
    }
    res = tmp;
  }

  return res;
}

Q32_32
Q32_32_ipow(Q32_32 a, unsigned int p) {
  if (p == 0) {
    return Q32_32_ONE;
  }

  Q32_32 res = a;

  for (unsigned int i = 1; i < p; i++) {
    res = Q32_32_mul(res, a);
  }

  return res;
}

Q32_32
Q32_32_fact(Q32_32 a) {
  if (a < 0) {
    return Q32_32_MIN;
  }
  if ((a == 0) || (a == Q32_32_ONE)) {
    return Q32_32_ONE;
  }

  Q32_32 res = Q32_32_ONE;
  Q32_32 i = Q32_32_ONE;

  while (i <= a) {
    res = Q32_32_mul(res, i);
    i = Q32_32_add(i, Q32_32_ONE);
  }
  return res;
}

Q32_32
Q32_32_fact_noof(Q32_32 a, bool *overflow) {
  *overflow = false;
  if (a < 0) {
    return Q32_32_MIN;
  }
  if ((a == 0) || (a == Q32_32_ONE)) {
    return Q32_32_ONE;
  }

  Q32_32 res = Q32_32_ONE;
  Q32_32 tmp;
  Q32_32 i = Q32_32_ONE;

  while (i <= a) {
    tmp = Q32_32_mul_noof(res, i, overflow);
    if (*overflow) {
      return res;
    }
    res = tmp;
    i = Q32_32_add(i, Q32_32_ONE);
  }
  return res;
}

Q32_32
Q32_32_exp(Q32_32 a) {
  if (a == 0) {
    return Q32_32_ONE;
  }
  if (a == Q32_32_ONE) {
    return Q32_32_E;
  }
  if (a == -Q32_32_ONE) {
    return Q32_32_div(Q32_32_ONE, Q32_32_E);
  }

  bool is_neg = Q32_32_IsNeg(a);

  if (is_neg) {
    a = -a;
  }

  Q32_32 res = Q32_32_E;

  Q32_32 l = Q32_32_floor(a);

  for (Q32_32 i = Q32_32_ONE; i < l; i = Q32_32_add(i, Q32_32_ONE)) {
    res = Q32_32_mul(Q32_32_E, res);
  }

  return is_neg ? Q32_32_div(Q32_32_ONE, res) : res;
}

Q32_32
Q32_32_ilg(Q32_32 a) {
  if (a <= 0) {
    return a;
  }

  Q32_32 res = 0;
  Q32_32 dix = Q32_32_from_int(10);

  while (a >= dix) {
    res = Q32_32_add(res, Q32_32_ONE);
    a = Q32_32_div(a, dix);
  }

  return res;
}

Q32_32
Q32_32_ilog(Q32_32 a, Q32_32 base) {
  if ((a <= 0) || (base <= 0)) {
    return a;
  }

  Q32_32 res = 0;

  while (a >= base) {
    res = Q32_32_add(res, Q32_32_ONE);
    a = Q32_32_div(a, base);
  }

  return res;
}

static Q32_32 Q32_32_ln_taylor(Q32_32 a) {
  Q32_32 sum = 0;
  int i = 1;

  while (i < 1000) {
    Q32_32 qi = Q32_32_from_int(i);
    Q32_32 t =
        Q32_32_mul(Q32_32_div(Q32_32_ONE, qi),
                   Q32_32_pow(Q32_32_sub(a, Q32_32_ONE), (unsigned int)i));

    if (i % 2) {
      sum = Q32_32_add(sum, t);
    } else {
      sum = Q32_32_sub(sum, t);
    }
    i++;
  }
  return sum;
}

Q32_32
Q32_32_ln(Q32_32 a) {
  if (a <= 0) {
    return a;
  }

  if (a == Q32_32_ONE) {
    return 0;
  }

  if (a < Q32_32_from_int(2)) {
    return Q32_32_ln_taylor(a);
  }

  Q32_32 div = -9889527670; /* ln(0.1) */
  unsigned int p = (unsigned int)Q32_32_to_int(Q32_32_ilg(a));
  Q32_32 loga = Q32_32_div(Q32_32_mul(Q32_32_ONE, a),
                           Q32_32_ipow(Q32_32_from_int(10), p + 1));
  unsigned int i = 0;
  Q32_32 d = div;

  while (i++ < p) {
    d = Q32_32_add(d, div);
  }

  return Q32_32_sub(Q32_32_ln_taylor(loga), d);
}

Q32_32
Q32_32_log(Q32_32 a, Q32_32 base) {
  return Q32_32_div(Q32_32_ln(a), Q32_32_ln(base));
}

Q32_32
Q32_32_lb(Q32_32 a) {
  return Q32_32_div(Q32_32_ln(a), Q32_32_ln(Q32_32_from_int(2)));
}

Q32_32
Q32_32_lg(Q32_32 a) {
  return Q32_32_div(Q32_32_ln(a), Q32_32_ln(Q32_32_from_int(10)));
}

/* t = 1.0
 * i = 0
 * while i < 32:
 *   atanTable.append(math.atan(t))
 *   t /= 2.0
 *   atanhTable.append(0.5*math.log((1.0+t)/(1.0-t)))
 *   i += 1
 */
static Q32_32 atanTable[Q32_32_HALF_BIT] = {
    3373259426, 1991351317, 1052175346, 534100634, 268086747, 134174062,
    67103403,   33553749,   16777130,   8388597,   4194302,   2097151,
    1048575,    524287,     262143,     131071,    65535,     32767,
    16383,      8191,       4095,       2047,      1023,      511,
    255,        127,        63,         32,        16,        8,
    4,          2};

static Q32_32 atanhTable[Q32_32_HALF_BIT] = {
    0,        2359251925, 1096989674, 539693625, 268785802, 134261444, 67114326,
    33555114, 16777301,   8388618,    4194305,   2097152,   1048576,   524288,
    262144,   131072,     65536,      32768,     16384,     8192,      4096,
    2048,     1024,       512,        256,       128,       64,        32,
    16,       8,          4,          2,
};

/* a = np.array([math.cos(math.atan(2**(-i))) for i in range(0,10000)])
r = np.cumprod(a)[len(a)-1]
# r = 0.60725293500888144482274810798117
K = int(r * (1<<32)) */

#define K 2608131496

/*
a = np.array([math.cosh(math.atanh(2**(-i))) for i in range(1,10000)])
r = np.cumprod(a)[len(a)-1]
r = 1.20513635844646094241738865093794
KH = int(r * (1<<32)) */

#define KH 5176021246

void Q32_32_cordic(Q32_32 angle, unsigned int n, Q32_32 *s, Q32_32 *c) {

  bool neg = Q32_32_IsNeg(angle);
  Q32_32 zi = Q32_32_abs(angle);

  while (zi > Q32_32_2PI) {
    zi = Q32_32_sub(zi, Q32_32_2PI);
  }
  if (neg) {
    zi = Q32_32_sub(Q32_32_2PI, zi);
  }

  int quadrant = 0;

  /* 270->360 */
  Q32_32 r270 = Q32_32_add(Q32_32_PI, Q32_32_PI_2);

  if (zi > r270) {
    quadrant = 4;
    zi = Q32_32_sub(zi, r270);
  } else if ((zi > Q32_32_PI) && (zi <= r270)) {
    quadrant = 3;
    zi = Q32_32_sub(zi, Q32_32_PI);
  } else if ((zi > Q32_32_PI_2) && (zi <= Q32_32_PI)) {
    quadrant = 2;
    zi = Q32_32_sub(zi, Q32_32_PI_2);
  } else if (zi <= Q32_32_PI_2) {
    quadrant = 1;
  }
  Q32_32 x = K;
  Q32_32 y = 0;
  Q32_32 x_temp;

  unsigned int i;

  n = ((n > Q32_32_HALF_BIT) ? Q32_32_HALF_BIT : n);

  for (i = 0; i < n; i++) {
    if (zi < 0) {
      x_temp = Q32_32_add(x, (y >> i));
      y = Q32_32_sub(y, (x >> i));
      zi = Q32_32_add(zi, atanTable[i]);
    } else {
      x_temp = Q32_32_sub(x, (y >> i));
      y = Q32_32_add(y, (x >> i));
      zi = Q32_32_sub(zi, atanTable[i]);
    }
    x = x_temp;
  }

  switch (quadrant) {
  case 1:
    *c = x;
    *s = y;
    break;
  case 2:
    *c = -y;
    *s = x;
    break;
  case 3:
    *c = -x;
    *s = -y;
    break;
  case 4:
    *c = y;
    *s = -x;
    break;
  default:
    break;
  }
}
/*
static void
Q32_32_cordich (Q32_32 angle, unsigned int n, Q32_32* s, Q32_32* c)
{

    bool neg  = Q32_32_IsNeg (angle);
    Q32_32 zi = angle;


    Q32_32 x = 0, y = KH;
    Q32_32 x_temp;

    unsigned int i;

    n = ((n > Q32_32_HALF_BIT) ? Q32_32_HALF_BIT : n);

    for (i = 0; i < n; i++) {

        if (zi > 0) {
            x_temp = Q32_32_add (x, (y >> i));
            y      = Q32_32_add (y, (x >> i));
            zi     = Q32_32_sub (zi, atanhTable[i]);
        } else {
            x_temp = Q32_32_sub (x, (y >> i));
            y      = Q32_32_sub (y, (x >> i));
            zi     = Q32_32_add (zi, atanhTable[i]);
        }
        x = x_temp;
    }


    *c = x;
    *s = y;
}*/

static void Q32_32_cordich(Q32_32 angle, unsigned int n, Q32_32 *s, Q32_32 *c) {
  Q32_32 x = KH;
  Q32_32 y = 0;
  Q32_32 z = angle;
  Q32_32 x_temp;

  unsigned int i;

  n = ((n > Q32_32_HALF_BIT) ? Q32_32_HALF_BIT : n);

  for (i = 1; i < n; i++) {
    Q32_32 sgn = Q32_32_sign(z);
    x_temp = Q32_32_add(x, Q32_32_mul(sgn, (y >> i)));
    y = Q32_32_add(y, Q32_32_mul(sgn, (x >> i)));
    z = Q32_32_sub(z, Q32_32_mul(sgn, atanhTable[i]));

    x = x_temp;
  }

  *c = x;
  *s = y;
}

#ifdef ALTCORDICH
static void Q32_32_cordich(Q32_32 *x0, Q32_32 *y0, Q32_32 *z0, int vecmode) {
  /* here's the hyperbolic methods. its very similar to
   * the circular methods, but with some small differences.
   *
   * the `x' iteration have the opposite sign.
   * iterations 4, 7, .. 3k+1 are repeated.
   * iteration 0 is not performed.
   */

  Q32_32 t;
  Q32_32 x, y, z;
  unsigned int i;

  t = Q32_32_from_double(0.5);
  x = *x0;
  y = *y0;
  z = *z0;

  int k = 3;
  for (i = 0; i < Q32_32_HALF_BIT; i++) {
    Q32_32 x1;
    int j;

    for (j = 0; j < 2; j++) {
      if ((vecmode >= 0 && y < 0) || (vecmode < 0 && z >= 0)) {
        x1 = Q32_32_add(x, Q32_32_mul(y, t));
        y = Q32_32_add(y, Q32_32_mul(x, t));
        z = Q32_32_sub(z, atanhTable[i]);
      } else {
        x1 = Q32_32_sub(x, Q32_32_mul(y, t));
        y = Q32_32_sub(y, Q32_32_mul(x, t));
        z = Q32_32_add(z, atanhTable[i]);
      }
      x = x1;

      if (k) {
        --k;
        break;
      } else
        k = 3;
    }

    t = Q32_32_div(t, Q32_32_from_int(2));
  }

  *x0 = x;
  *y0 = y;
  *z0 = z;
}

static Q32_32 sinhcoshCordic(Q32_32 a, Q32_32 *c) {
  /* |a| < 1.13 */
  Q32_32 y;
  *c = KH;
  y = 0;
  Q32_32_cordich(c, &y, &a, -1);
  return y;
}

Q32_32
Q32_32_cosh(Q32_32 a) {
  if (a == 0)
    return Q32_32_ONE;
  Q32_32 c;
  sinhcoshCordic(a, &c);

  return c;
}

Q32_32
Q32_32_sinh(Q32_32 a) {
  if (a == 0)
    return 0;
  Q32_32 c;
  return sinhcoshCordic(a, &c);
}

Q32_32
Q32_32_tanh(Q32_32 a) {
  /* |a| < 1.13 */
  Q32_32 s, c;
  c = KH;
  s = 0;
  Q32_32_cordich(&c, &s, &a, -1);
  return Q32_32_div(s, c);
}

Q32_32
Q32_32_atanh(Q32_32 a) {
  /* |a| < 1.13 */
  Q32_32 x, z;
  x = Q32_32_ONE;
  z = 0;
  Q32_32_cordich(&x, &a, &z, 0);
  return z;
}

#endif

/* Trigonometry */
Q32_32
Q32_32_sin(Q32_32 a) {
  Q32_32 s;
  Q32_32 c;

  Q32_32_cordic(a, Q32_32_HALF_BIT, &s, &c);

  return s;
}

Q32_32
Q32_32_cos(Q32_32 a) {
  Q32_32 s;
  Q32_32 c;

  Q32_32_cordic(a, Q32_32_HALF_BIT, &s, &c);

  return c;
}

Q32_32
Q32_32_tan(Q32_32 a) {
  Q32_32 s;
  Q32_32 c;

  Q32_32_cordic(a, Q32_32_HALF_BIT, &s, &c);

  return Q32_32_div(s, c);
}

bool Q32_32_asin(Q32_32 a, Q32_32 *r) {
  if (Q32_32_abs(a) > Q32_32_ONE) {
    return false;
  }
  /* asin(a) = 2*atan(a/(1+sqrt(1-a^2))) */
  Q32_32 _sq = Q32_32_sqrt(Q32_32_sub(Q32_32_ONE, Q32_32_ipow(a, 2)));
  Q32_32 _atan = Q32_32_atan(Q32_32_div(a, Q32_32_add(Q32_32_ONE, _sq)));

  *r = Q32_32_mul(Q32_32_from_int(2), _atan);

  return true;
}

bool Q32_32_acos(Q32_32 a, Q32_32 *r) {
  bool is_neg = Q32_32_IsNeg(a);
  Q32_32 absa = Q32_32_abs(a);

  if (absa > Q32_32_ONE) {
    return false;
  }
  /* To avoid division by zero I don't use this relationship */
  /* acos(a) = 2*atan(sqrt(1-a^2)/(1+x)) */
  /* but */
  /* acos(a) = asin(sqrt(1-a^2)), if 0 <= x <= 1 */
  /* for -1 <= x < 0 */
  /* just sub ret to PI */
  Q32_32 ret = Q32_32_MIN;

  Q32_32_asin(Q32_32_sqrt(Q32_32_sub(Q32_32_ONE, Q32_32_ipow(a, 2))), &ret);
  if (is_neg) {
    *r = Q32_32_sub(Q32_32_PI, ret);
  } else {
    *r = ret;
  }

  return true;
}

Q32_32
Q32_32_atan(Q32_32 a) {

  Q32_32 x = Q32_32_ONE;
  Q32_32 y = a;
  Q32_32 zi = 0;
  Q32_32 x_temp;

  unsigned int i;
  unsigned int n = 32;

  //  n = ((n > Q32_32_HALF_BIT) ? Q32_32_HALF_BIT : n);

  for (i = 0; i < n; i++) {
    if (y < 0) {
      x_temp = Q32_32_sub(x, (y >> i));
      y = Q32_32_add(y, (x >> i));
      zi = Q32_32_sub(zi, atanTable[i]);
    } else {
      x_temp = Q32_32_add(x, (y >> i));
      y = Q32_32_sub(y, (x >> i));
      zi = Q32_32_add(zi, atanTable[i]);
    }
    x = x_temp;
  }

  return zi;
}

Q32_32
Q32_32_sign(Q32_32 a) {
  if (a == 0) {
    return 0;
  }
  if (Q32_32_IsNeg(a)) {
    return -Q32_32_ONE;
  }
  return Q32_32_ONE;
}

Q32_32
Q32_32_copySign(Q32_32 a, Q32_32 sign_of) {
  Q32_32 s = Q32_32_sign(sign_of);

  if (s == -Q32_32_ONE) {
    return Q32_32_IsNeg(a) ? a : -a;
  }
  return Q32_32_IsNeg(a) ? -a : a;
}

bool Q32_32_atan2(Q32_32 y, Q32_32 x, Q32_32 *r) {

  if (y == 0) {
    if (x == 0) {
      *r = 0;
      return false;
    }
    if (x > 0) {
      *r = 0;
      return true;
    }
    *r = Q32_32_PI;
    return true;
  }
  Q32_32 res;

  if (x == 0) {
    res = Q32_32_copySign(Q32_32_PI_2, y);
  } else {
    Q32_32 angle = Q32_32_atan(Q32_32_abs(Q32_32_div(y, x)));

    if (x > 0) {
      res = Q32_32_copySign(angle, y);
    } else {
      res = Q32_32_copySign(Q32_32_sub(Q32_32_PI, angle), y);
    }
  }

  *r = res;
  return true;
}

Q32_32
Q32_32_sinh(Q32_32 a) {
  Q32_32 s;
  Q32_32 c;

  Q32_32_cordich(a, Q32_32_HALF_BIT, &s, &c);

  return s;
}

Q32_32
Q32_32_cosh(Q32_32 a) {
  Q32_32 s;
  Q32_32 c;

  Q32_32_cordich(a, Q32_32_HALF_BIT, &s, &c);

  return c;
}

Q32_32
Q32_32_tanh(Q32_32 a) {
  Q32_32 s;
  Q32_32 c;

  Q32_32_cordich(a, Q32_32_HALF_BIT, &s, &c);

  return Q32_32_div(s, c);
}

bool Q32_32_near(Q32_32 a, Q32_32 b, unsigned int tolerance) {
  Q32_32 a_abs = Q32_32_abs(a);
  Q32_32 b_abs = Q32_32_abs(b);

  Q32_32 diff = a_abs > b_abs ? Q32_32_sub(a, b) : Q32_32_sub(b, a);

  return tolerance >= diff;
}
