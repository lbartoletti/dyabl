extern "C" {
#include "Q32_32.h"
#include <stdlib.h>
}

#include <cmath>
#include <cstdint>
#include <gtest/gtest.h>
#include <limits>
using namespace std;

TEST(f32p32, sign) {
  int ia = 12345;
  Q32_32 a;

  a = Q32_32_from_int(ia);
  ASSERT_EQ(Q32_32_sign(a), Q32_32_ONE);
  a = Q32_32_from_int(-ia);
  ASSERT_EQ(Q32_32_sign(a), -Q32_32_ONE);
  a = Q32_32_from_int(0);
  ASSERT_EQ(Q32_32_sign(a), 0);
  ASSERT_EQ(Q32_32_sign(-a), 0);

  int ib = 789;
  Q32_32 b;

  b = Q32_32_from_int(ib);
  ASSERT_EQ(Q32_32_copySign(b, ia), b);
  b = Q32_32_from_int(ib);
  ASSERT_EQ(Q32_32_copySign(b, -ia), -b);
  b = Q32_32_from_int(-ib);
  ASSERT_EQ(Q32_32_copySign(b, ia), -b);
  b = Q32_32_from_int(-ib);
  ASSERT_EQ(Q32_32_copySign(b, -ia), b);
  b = Q32_32_from_int(ib);
  ASSERT_EQ(Q32_32_copySign(b, 0), b);
  ASSERT_EQ(Q32_32_copySign(-b, 0), b);
}

TEST(f32p32, floor_ceil) {
  Q32_32 a = Q32_32_from_double(1.4);
  ASSERT_EQ(Q32_32_to_int(Q32_32_floor(a)), 1);
  ASSERT_EQ(Q32_32_to_int(Q32_32_ceil(a)), 2);

  a = Q32_32_from_double(0.8);
  ASSERT_EQ(Q32_32_to_int(Q32_32_floor(a)), 0);
  ASSERT_EQ(Q32_32_to_int(Q32_32_ceil(a)), 1);

  a = Q32_32_from_double(50.5);
  ASSERT_EQ(Q32_32_to_int(Q32_32_floor(a)), 50);
  ASSERT_EQ(Q32_32_to_int(Q32_32_ceil(a)), 51);

  a = Q32_32_from_double(-5.8);
  ASSERT_EQ(Q32_32_to_int(Q32_32_floor(a)), -6);
  ASSERT_EQ(Q32_32_to_int(Q32_32_ceil(a)), -5);
}

TEST(f32p32, modf) {
  Q32_32 a = Q32_32_from_double(1.4);
  Q32_32 i, f;
  Q32_32_modf(a, &i, &f);
  ASSERT_EQ(1, Q32_32_to_int(i));
  ASSERT_NEAR(0.4 - Q32_32_to_double(f), 0, 0.01);
  ASSERT_EQ(Q32_32_from_double(1.4), i + f);

  a = Q32_32_from_double(-1.4);
  Q32_32_modf(a, &i, &f);
  ASSERT_GT(0, Q32_32_to_double(f));
  ASSERT_EQ(-1, Q32_32_to_int(i));
  ASSERT_NEAR(-0.4 - Q32_32_to_double(f), 0, 0.01);
  ASSERT_EQ(Q32_32_from_double(-1.4), i + f);

  a = Q32_32_from_double(-5.8);
  Q32_32_modf(a, &i, &f);
  ASSERT_EQ(-5, Q32_32_to_int(i));
  ASSERT_GT(0, Q32_32_to_double(f));
  ASSERT_NEAR(-0.8 - Q32_32_to_double(f), 0, 0.01);
  ASSERT_EQ(Q32_32_from_double(-5.8), i + f);

  a = Q32_32_from_double(10);
  Q32_32_modf(a, &i, &f);
  ASSERT_EQ(Q32_32_to_int(i), 10);
  ASSERT_DOUBLE_EQ(Q32_32_to_double(f), 0);

  a = Q32_32_from_double(123456789);
  Q32_32_modf(a, &i, &f);
  ASSERT_EQ(Q32_32_to_int(i), 123456789);
  ASSERT_DOUBLE_EQ(Q32_32_to_double(f), 0);

  a = Q32_32_from_double(-123456789);
  Q32_32_modf(a, &i, &f);
  ASSERT_EQ(Q32_32_to_int(i), -123456789);
  ASSERT_DOUBLE_EQ(Q32_32_to_double(f), 0);
}

TEST(f32p32, add) {
  Q32_32 a = Q32_32_from_int(1);
  Q32_32 b = Q32_32_from_int(1);
  ASSERT_EQ(Q32_32_add(a, b), Q32_32_from_int(2));

#pragma clang diagnostic ignored "-Wconstant-conversion"
  a = Q32_32_from_int(Q32_32_HALF_MAX);
  /* overflow */
  ASSERT_NE(Q32_32_add(a, b), Q32_32_HALF_MAX + 1);
}

TEST(f32p32, add_noof) {
  Q32_32 a = Q32_32_from_int(1);
  Q32_32 b = Q32_32_from_int(1);
  bool of;
  ASSERT_EQ(Q32_32_add_noof(a, b, &of), Q32_32_from_int(2));
  ASSERT_EQ(of, false);

  a = Q32_32_MAX;
  /* overflow */
  Q32_32_add_noof(a, b, &of);
  ASSERT_EQ(of, true);

  a = Q32_32_MIN;
  b = Q32_32_from_int(-1);
  /* underflow */
  Q32_32_add_noof(a, b, &of);
  ASSERT_EQ(of, true);
}

TEST(f32p32, sat_add) {
  Q32_32 a = Q32_32_from_int(1);
  Q32_32 b = Q32_32_from_int(1);
  ASSERT_EQ(Q32_32_sat_add(a, b), Q32_32_from_int(2));

  a = Q32_32_MIN;
  b = Q32_32_from_int(-10);
  /* underflow */
  ASSERT_EQ(Q32_32_sat_add(a, b), Q32_32_MIN);

  a = Q32_32_MAX;
  b = Q32_32_from_int(10);
  /* overflow */
  ASSERT_EQ(Q32_32_sat_add(a, b), Q32_32_MAX);
}

TEST(f32p32, sub) {
  Q32_32 a = Q32_32_from_int(1);
  Q32_32 b = Q32_32_from_int(1);
  ASSERT_EQ(Q32_32_sub(a, b), Q32_32_from_int(0));

  a = Q32_32_from_int(1);
  b = Q32_32_from_int(2);
  ASSERT_EQ(Q32_32_sub(a, b), Q32_32_from_int(-1));

  a = Q32_32_from_int(-1);
  b = Q32_32_from_int(-1);
  ASSERT_EQ(Q32_32_sub(a, b), Q32_32_from_int(0));

  // a = Q32_32_MIN;
  // b = Q32_32_from_int (1);
  // ASSERT_EQ (Q32_32_sub (a, b), Q32_32_MIN - 1);
}

TEST(f32p32, sub_noof) {
  bool of;
  Q32_32 a = Q32_32_from_int(-1);
  Q32_32 b = Q32_32_from_int(-1);
  ASSERT_EQ(Q32_32_sub_noof(a, b, &of), 0);
  ASSERT_EQ(of, false);

  a = Q32_32_MIN;
  b = Q32_32_from_int(1);
  Q32_32_sub_noof(a, b, &of);
  ASSERT_EQ(of, true);

  a = Q32_32_MAX;
  b = Q32_32_from_int(-1);
  Q32_32_sub_noof(a, b, &of);
  ASSERT_EQ(of, true);
}

TEST(f32p32, sat_sub) {
  Q32_32 a = Q32_32_from_int(20);
  Q32_32 b = Q32_32_from_int(10);
  ASSERT_EQ(Q32_32_sat_sub(a, b), Q32_32_from_int(10));

  a = Q32_32_MAX;
  b = Q32_32_from_int(-10);
  ASSERT_EQ(Q32_32_sat_sub(a, b), Q32_32_MAX);

  a = Q32_32_MIN;
  b = Q32_32_from_int(10);
  ASSERT_EQ(Q32_32_sat_sub(a, b), Q32_32_MIN);
}

TEST(f32p32, mul) {
  Q32_32 a = Q32_32_from_int(20);
  Q32_32 b = Q32_32_from_int(10);
  ASSERT_EQ(Q32_32_mul(a, b), Q32_32_from_int(200));
  ASSERT_EQ(Q32_32_mul(-a, -b), Q32_32_from_int(200));
  ASSERT_EQ(Q32_32_mul(-a, b), Q32_32_from_int(-200));
  ASSERT_EQ(Q32_32_mul(a, -b), Q32_32_from_int(-200));

  int ia = 1000, ib = 1000;
  a = Q32_32_from_int(ia);
  b = Q32_32_from_int(ib);
  double dc = 2.1, dd = 2.1;
  Q32_32 c = Q32_32_from_double(dc);
  Q32_32 d = Q32_32_from_double(dd);

  Q32_32 ri = Q32_32_mul(a, b);
  Q32_32 rd = Q32_32_mul(c, d);

  ASSERT_EQ(ri, Q32_32_from_int(ia * ib));
  ASSERT_TRUE(Q32_32_near(rd, Q32_32_from_double(dc * dd), 2));
}

TEST(f32p32, mul_noof) {
  Q32_32 a = Q32_32_from_int(20);
  Q32_32 b = Q32_32_from_int(10);
  bool of;
  Q32_32_mul_noof(a, b, &of);
  ASSERT_EQ(of, false);
  Q32_32_mul_noof(-a, -b, &of);
  ASSERT_EQ(of, false);
  Q32_32_mul_noof(-a, b, &of);
  ASSERT_EQ(of, false);
  Q32_32_mul_noof(a, -b, &of);
  ASSERT_EQ(of, false);

  a = INT64_MAX;
  Q32_32_mul_noof(a, b, &of);
  ASSERT_EQ(of, true);
  Q32_32_mul_noof(a, -b, &of);
  ASSERT_EQ(of, true);
}

TEST(f32p32, sat_mul) {
  Q32_32 a = Q32_32_from_int(20);
  Q32_32 b = Q32_32_from_int(10);
  ASSERT_EQ(Q32_32_sat_mul(a, b), Q32_32_from_int(200));
  ASSERT_EQ(Q32_32_sat_mul(-a, -b), Q32_32_from_int(200));
  ASSERT_EQ(Q32_32_sat_mul(-a, b), Q32_32_from_int(-200));
  ASSERT_EQ(Q32_32_sat_mul(a, -b), Q32_32_from_int(-200));

  a = Q32_32_MAX;
  ASSERT_EQ(Q32_32_sat_mul(a, b), Q32_32_MAX);
  ASSERT_EQ(Q32_32_sat_mul(a, -b), Q32_32_MIN);
}

TEST(f32p32, div) {
  Q32_32 a = Q32_32_from_int(20);
  Q32_32 b = Q32_32_from_int(10);
  ASSERT_EQ(Q32_32_div(a, b), Q32_32_from_int(2));
  ASSERT_EQ(Q32_32_div(-a, -b), Q32_32_from_int(2));
  ASSERT_EQ(Q32_32_div(-a, b), Q32_32_from_int(-2));
  ASSERT_EQ(Q32_32_div(a, -b), Q32_32_from_int(-2));

  ASSERT_EQ(Q32_32_div(b, a), Q32_32_from_double(10 / 20.0));
  ASSERT_EQ(Q32_32_div(-b, -a), Q32_32_from_double(-10 / -20.0));
  ASSERT_EQ(Q32_32_div(-b, a), Q32_32_from_double(-10 / 20.0));
  ASSERT_EQ(Q32_32_div(b, -a), Q32_32_from_double(10 / -20.0));

  ASSERT_EQ(Q32_32_div(a, 0), Q32_32_MAX);
  ASSERT_EQ(Q32_32_div(-a, 0), Q32_32_MASK);

  ASSERT_EQ(Q32_32_div(0, a), 0);
  ASSERT_EQ(Q32_32_div(Q32_32_MIN, Q32_32_from_int(-1)), 0);

  ASSERT_EQ(Q32_32_div(Q32_32_from_double(8.4), Q32_32_from_double(3.5)),
            Q32_32_from_double(2.4));
}

TEST(f32p32, div_noof) {
  Q32_32 a = Q32_32_MAX;
  Q32_32 b = Q32_32_from_int(-1);
  bool of;
  Q32_32_div_noof(a, b, &of);
  ASSERT_EQ(of, true);

  ASSERT_EQ(Q32_32_div_noof(Q32_32_from_int(20), Q32_32_from_int(10), &of),
            Q32_32_from_int(2));
  ASSERT_EQ(of, false);
}

TEST(f32p32, mod) {
  Q32_32 a = Q32_32_from_int(462);
  Q32_32 b = Q32_32_from_int(147);
  Q32_32 c = Q32_32_from_int(1071);
  Q32_32 d = Q32_32_from_int(21);

  ASSERT_EQ(Q32_32_mod(a, b), d);
  ASSERT_EQ(Q32_32_mod(c, a), b);
  ASSERT_EQ(Q32_32_mod(a, c), a);

  /* pair? */
  ASSERT_EQ(Q32_32_mod(a, Q32_32_from_int(2)), 0);
}

TEST(f32p32, divmod) {
  Q32_32 a = Q32_32_from_int(462);
  Q32_32 b = Q32_32_from_int(147);
  Q32_32 c = Q32_32_from_int(1071);

  Q32_32 div, mod;
  Q32_32_divmod(a, b, &div, &mod);
  ASSERT_EQ(div, Q32_32_from_int(3));
  ASSERT_EQ(mod, Q32_32_mod(a, b));
  Q32_32_divmod(c, a, &div, &mod);
  ASSERT_EQ(div, Q32_32_from_int(2));
  ASSERT_EQ(mod, Q32_32_mod(c, a));
  Q32_32_divmod(a, c, &div, &mod);
  ASSERT_EQ(div, Q32_32_from_int(0));
  ASSERT_EQ(mod, Q32_32_mod(a, c));
}

TEST(f32p32, sqrt) {
  int32_t i = 1;
  Q32_32 a;
  while (i < (1 << 16) / 2) {
    a = Q32_32_from_int(i * i);
    ASSERT_EQ(Q32_32_sqrt(-a), Q32_32_MIN);
    ASSERT_EQ(Q32_32_sqrt(a), Q32_32_from_int(i));
    i += 1;
  }

  ASSERT_TRUE((Q32_32_to_double(Q32_32_sqrt(Q32_32_from_int(2))) - sqrt(2)) <
              0.00001);
}

TEST(f32p32, sq) {
  Q32_32 a = Q32_32_from_int(5);
  ASSERT_EQ(Q32_32_sq(a), Q32_32_from_int(25));
  a = Q32_32_from_int(7);
  ASSERT_EQ(Q32_32_sq(a), Q32_32_from_int(49));
  a = Q32_32_MAX;
  /* overflow */
  ASSERT_NE(Q32_32_sq(a), Q32_32_MAX * Q32_32_MAX);
}

TEST(f32p32, cube) {
  Q32_32 a = Q32_32_from_int(5);
  ASSERT_EQ(Q32_32_cube(a), Q32_32_from_int(125));
  a = Q32_32_from_int(7);
  ASSERT_EQ(Q32_32_cube(a), Q32_32_from_int(343));
  a = Q32_32_MAX;
  /* overflow */
  ASSERT_NE(Q32_32_cube(a), Q32_32_MAX * Q32_32_MAX * Q32_32_MAX);
}

TEST(f32p32, pow) {
  Q32_32 a = Q32_32_from_int(5);
  ASSERT_EQ(Q32_32_pow(a, 0), Q32_32_ONE);
  ASSERT_EQ(Q32_32_pow(a, 2), Q32_32_sq(Q32_32_from_int(5)));
  ASSERT_EQ(Q32_32_pow(a, 3), Q32_32_cube(Q32_32_from_int(5)));
  ASSERT_EQ(Q32_32_pow(a, 5), Q32_32_from_int(3125));
  ASSERT_EQ(Q32_32_pow(a, 10), Q32_32_from_int(9765625));
}

TEST(f32p32, pow_noof) {
  Q32_32 a = Q32_32_from_int(5);
  bool ov;
  ASSERT_EQ(Q32_32_pow_noof(a, 0, &ov), Q32_32_ONE);
  ASSERT_FALSE(ov);
  ASSERT_EQ(Q32_32_pow_noof(a, 2, &ov), Q32_32_sq(Q32_32_from_int(5)));
  ASSERT_FALSE(ov);
  ASSERT_EQ(Q32_32_pow_noof(a, 3, &ov), Q32_32_cube(Q32_32_from_int(5)));
  ASSERT_FALSE(ov);
  ASSERT_EQ(Q32_32_pow_noof(a, 5, &ov), Q32_32_from_int(3125));
  ASSERT_FALSE(ov);
  ASSERT_EQ(Q32_32_pow_noof(a, 10, &ov), Q32_32_from_int(9765625));
  ASSERT_FALSE(ov);

  Q32_32_pow_noof(Q32_32_from_int(500), 500, &ov);
  ASSERT_TRUE(ov);
}

TEST(f32p32, ipow) {
  Q32_32 a = Q32_32_from_int(5);
  ASSERT_EQ(Q32_32_ipow(a, 0), Q32_32_ONE);
  ASSERT_EQ(Q32_32_ipow(a, 2), Q32_32_sq(Q32_32_from_int(5)));
  ASSERT_EQ(Q32_32_ipow(a, 3), Q32_32_cube(Q32_32_from_int(5)));
  ASSERT_EQ(Q32_32_ipow(a, 5), Q32_32_from_int(3125));
  ASSERT_EQ(Q32_32_ipow(a, 10), Q32_32_from_int(9765625));
}

TEST(f32p32, ipow_noof) {
  Q32_32 a = Q32_32_from_int(5);
  bool ov;
  ASSERT_EQ(Q32_32_ipow_noof(a, 0, &ov), Q32_32_ONE);
  ASSERT_FALSE(ov);
  ASSERT_EQ(Q32_32_ipow_noof(a, 2, &ov), Q32_32_sq(Q32_32_from_int(5)));
  ASSERT_FALSE(ov);
  ASSERT_EQ(Q32_32_ipow_noof(a, 3, &ov), Q32_32_cube(Q32_32_from_int(5)));
  ASSERT_FALSE(ov);
  ASSERT_EQ(Q32_32_ipow_noof(a, 5, &ov), Q32_32_from_int(3125));
  ASSERT_FALSE(ov);
  ASSERT_EQ(Q32_32_ipow_noof(a, 10, &ov), Q32_32_from_int(9765625));
  ASSERT_FALSE(ov);

  Q32_32_ipow_noof(Q32_32_from_int(500), 500, &ov);
  ASSERT_TRUE(ov);
}

TEST(f32p32, fact) {
  ASSERT_EQ(Q32_32_fact(Q32_32_from_int(-1)), Q32_32_MIN);
  ASSERT_EQ(Q32_32_fact(Q32_32_from_int(0)), Q32_32_from_int(1));
  ASSERT_EQ(Q32_32_fact(Q32_32_from_int(1)), Q32_32_from_int(1));
  ASSERT_EQ(Q32_32_fact(Q32_32_from_int(2)), Q32_32_from_int(2));
  ASSERT_EQ(Q32_32_fact(Q32_32_from_int(3)), Q32_32_from_int(6));
  ASSERT_EQ(Q32_32_fact(Q32_32_from_int(4)), Q32_32_from_int(24));
  ASSERT_EQ(Q32_32_fact(Q32_32_from_int(5)), Q32_32_from_int(120));
  ASSERT_EQ(Q32_32_fact(Q32_32_from_int(6)), Q32_32_from_int(720));
  ASSERT_EQ(Q32_32_fact(Q32_32_from_int(7)), Q32_32_from_int(5040));
  ASSERT_EQ(Q32_32_fact(Q32_32_from_int(8)), Q32_32_from_int(40320));
  ASSERT_EQ(Q32_32_fact(Q32_32_from_int(9)), Q32_32_from_int(362880));
  ASSERT_EQ(Q32_32_fact(Q32_32_from_int(10)), Q32_32_from_int(3628800));
}

TEST(f32p32, fact_noof) {
  bool ov;
  ASSERT_EQ(Q32_32_fact_noof(Q32_32_from_int(-1), &ov), Q32_32_MIN);
  ASSERT_FALSE(ov);
  ASSERT_EQ(Q32_32_fact_noof(Q32_32_from_int(0), &ov), Q32_32_from_int(1));
  ASSERT_FALSE(ov);
  ASSERT_EQ(Q32_32_fact_noof(Q32_32_from_int(1), &ov), Q32_32_from_int(1));
  ASSERT_FALSE(ov);
  ASSERT_EQ(Q32_32_fact_noof(Q32_32_from_int(2), &ov), Q32_32_from_int(2));
  ASSERT_FALSE(ov);
  ASSERT_EQ(Q32_32_fact_noof(Q32_32_from_int(3), &ov), Q32_32_from_int(6));
  ASSERT_FALSE(ov);
  ASSERT_EQ(Q32_32_fact_noof(Q32_32_from_int(4), &ov), Q32_32_from_int(24));
  ASSERT_FALSE(ov);
  ASSERT_EQ(Q32_32_fact_noof(Q32_32_from_int(5), &ov), Q32_32_from_int(120));
  ASSERT_FALSE(ov);
  ASSERT_EQ(Q32_32_fact_noof(Q32_32_from_int(6), &ov), Q32_32_from_int(720));
  ASSERT_FALSE(ov);
  ASSERT_EQ(Q32_32_fact_noof(Q32_32_from_int(7), &ov), Q32_32_from_int(5040));
  ASSERT_FALSE(ov);
  ASSERT_EQ(Q32_32_fact_noof(Q32_32_from_int(8), &ov), Q32_32_from_int(40320));
  ASSERT_FALSE(ov);
  ASSERT_EQ(Q32_32_fact_noof(Q32_32_from_int(9), &ov), Q32_32_from_int(362880));
  ASSERT_FALSE(ov);
  ASSERT_EQ(Q32_32_fact_noof(Q32_32_from_int(10), &ov),
            Q32_32_from_int(3628800));
  ASSERT_FALSE(ov);
  Q32_32_fact_noof(Q32_32_from_int(13), &ov);
  ASSERT_TRUE(ov);
}

TEST(f32p32, exp) {
  ASSERT_EQ(Q32_32_exp(0), Q32_32_from_int(1));
  ASSERT_EQ(Q32_32_exp(Q32_32_from_int(1)), Q32_32_E);
  ASSERT_EQ(Q32_32_exp(Q32_32_from_int(-1)),
            Q32_32_div(Q32_32_from_int(1), Q32_32_E));
  ASSERT_EQ(Q32_32_to_int(Q32_32_exp(Q32_32_from_int(10))),
            ((int)22026.4657948));
  ASSERT_EQ(Q32_32_to_int(Q32_32_exp(Q32_32_from_int(-10))), ((int)4.54e-5));
}

TEST(f32p32, ilg) {
  Q32_32 a = Q32_32_from_int(-1);
  ASSERT_EQ(Q32_32_ilg(a), a);
  a = Q32_32_from_double(-0.5);
  ASSERT_EQ(Q32_32_ilg(a), a);
  a = 0;
  ASSERT_EQ(Q32_32_ilg(a), a);

  ASSERT_EQ(Q32_32_ilg(Q32_32_from_int(5)), Q32_32_from_int(0));
  ASSERT_EQ(Q32_32_ilg(Q32_32_from_double(8.12)), Q32_32_from_int(0));
  ASSERT_EQ(Q32_32_ilg(Q32_32_from_int(25)), Q32_32_from_int(1));
  ASSERT_EQ(Q32_32_ilg(Q32_32_from_double(98.12)), Q32_32_from_int(1));
  ASSERT_EQ(Q32_32_ilg(Q32_32_from_int(123)), Q32_32_from_int(2));
  ASSERT_EQ(Q32_32_ilg(Q32_32_from_double(654.21)), Q32_32_from_int(2));
  ASSERT_EQ(Q32_32_ilg(Q32_32_from_int(1234)), Q32_32_from_int(3));
  ASSERT_EQ(Q32_32_ilg(Q32_32_from_double(4321.456)), Q32_32_from_int(3));
}

TEST(f32p32, ilog) {
  Q32_32 base = Q32_32_from_int(2);
  Q32_32 a = Q32_32_from_int(-1);
  ASSERT_EQ(Q32_32_ilog(a, base), a);
  a = Q32_32_from_double(-0.5);
  ASSERT_EQ(Q32_32_ilog(a, base), a);
  a = 0;
  ASSERT_EQ(Q32_32_ilog(a, base), a);

  /* base 2 */
  base = Q32_32_from_int(2);
  ASSERT_EQ(Q32_32_ilog(Q32_32_from_int(1), base), Q32_32_from_int(0));
  ASSERT_EQ(Q32_32_ilog(Q32_32_from_double(0.12), base), Q32_32_from_int(0));
  ASSERT_EQ(Q32_32_ilog(Q32_32_from_int(2), base), Q32_32_from_int(1));
  ASSERT_EQ(Q32_32_ilog(Q32_32_from_double(3.12), base), Q32_32_from_int(1));
  ASSERT_EQ(Q32_32_ilog(Q32_32_from_int(4), base), Q32_32_from_int(2));
  ASSERT_EQ(Q32_32_ilog(Q32_32_from_double(4.21), base), Q32_32_from_int(2));
  ASSERT_EQ(Q32_32_ilog(Q32_32_from_int(8), base), Q32_32_from_int(3));
  ASSERT_EQ(Q32_32_ilog(Q32_32_from_double(9.456), base), Q32_32_from_int(3));

  /* base 7 */
  base = Q32_32_from_int(7);
  ASSERT_EQ(Q32_32_ilog(Q32_32_from_int(1), base), Q32_32_from_int(0));
  ASSERT_EQ(Q32_32_ilog(Q32_32_from_double(0.12), base), Q32_32_from_int(0));
  ASSERT_EQ(Q32_32_ilog(Q32_32_from_int(7), base), Q32_32_from_int(1));
  ASSERT_EQ(Q32_32_ilog(Q32_32_from_double(12.34), base), Q32_32_from_int(1));
  ASSERT_EQ(Q32_32_ilog(Q32_32_from_int(49), base), Q32_32_from_int(2));
  ASSERT_EQ(Q32_32_ilog(Q32_32_from_double(51.45), base), Q32_32_from_int(2));
  ASSERT_EQ(Q32_32_ilog(Q32_32_from_int(343), base), Q32_32_from_int(3));
  ASSERT_EQ(Q32_32_ilog(Q32_32_from_double(456.12), base), Q32_32_from_int(3));

  /* base 10 */
  base = Q32_32_from_int(10);
  ASSERT_EQ(Q32_32_ilog(Q32_32_from_int(5), base),
            Q32_32_ilg(Q32_32_from_int(1)));
  ASSERT_EQ(Q32_32_ilog(Q32_32_from_double(8.12), base),
            Q32_32_ilg(Q32_32_from_double(8.12)));
  ASSERT_EQ(Q32_32_ilog(Q32_32_from_int(25), base),
            Q32_32_ilg(Q32_32_from_int(25)));
  ASSERT_EQ(Q32_32_ilog(Q32_32_from_double(98.12), base),
            Q32_32_ilg(Q32_32_from_double(98.12)));
  ASSERT_EQ(Q32_32_ilog(Q32_32_from_int(123), base),
            Q32_32_ilg(Q32_32_from_int(123)));
  ASSERT_EQ(Q32_32_ilog(Q32_32_from_double(654.21), base),
            Q32_32_ilg(Q32_32_from_double(654.21)));
  ASSERT_EQ(Q32_32_ilog(Q32_32_from_int(1234), base),
            Q32_32_ilg(Q32_32_from_int(1234)));
  ASSERT_EQ(Q32_32_ilog(Q32_32_from_double(4321.456), base),
            Q32_32_ilg(Q32_32_from_double(4321.456)));

  /* base 15 */
  base = Q32_32_from_int(15);
  ASSERT_EQ(Q32_32_ilog(Q32_32_from_int(1), base), Q32_32_from_int(0));
  ASSERT_EQ(Q32_32_ilog(Q32_32_from_double(0.12), base), Q32_32_from_int(0));
  ASSERT_EQ(Q32_32_ilog(Q32_32_from_int(15), base), Q32_32_from_int(1));
  ASSERT_EQ(Q32_32_ilog(Q32_32_from_double(153.12), base), Q32_32_from_int(1));
  ASSERT_EQ(Q32_32_ilog(Q32_32_from_int(225), base), Q32_32_from_int(2));
  ASSERT_EQ(Q32_32_ilog(Q32_32_from_double(354.21), base), Q32_32_from_int(2));
  ASSERT_EQ(Q32_32_ilog(Q32_32_from_int(3375), base), Q32_32_from_int(3));
  ASSERT_EQ(Q32_32_ilog(Q32_32_from_double(9456.456), base),
            Q32_32_from_int(3));
}

TEST(f32p32, ln) {
  Q32_32 a = Q32_32_from_int(-1);
  ASSERT_EQ(Q32_32_ln(a), a);
  a = Q32_32_from_double(-0.5);
  ASSERT_EQ(Q32_32_ln(a), a);
  a = 0;
  ASSERT_EQ(Q32_32_ln(a), a);
  a = Q32_32_from_int(1);
  ASSERT_EQ(Q32_32_ln(a), 0);

  ASSERT_NEAR(Q32_32_to_double(Q32_32_ln(Q32_32_from_double(0.1))), -2.3025851,
              1e-4);
  ASSERT_NEAR(Q32_32_to_double(Q32_32_ln(Q32_32_from_int(2))), 0.693147180,
              1e-4);
  ASSERT_NEAR(Q32_32_to_double(Q32_32_ln(Q32_32_from_int(123456789))),
              18.63140176, 1e-4);
}
TEST(f32p32, log) {
  Q32_32 a = Q32_32_from_int(10);
  Q32_32 base = Q32_32_from_int(7);
  ASSERT_NEAR(Q32_32_to_double(Q32_32_lg(a)), 1.0, 1e-4);
  ASSERT_NEAR(Q32_32_to_double(Q32_32_lb(a)), 3.321980, 1e-4);
  ASSERT_NEAR(Q32_32_to_double(Q32_32_log(a, base)), 1.183294, 1e-4);

  a = Q32_32_from_double(753.159);
  base = Q32_32_from_int(456);
  ASSERT_NEAR(Q32_32_to_double(Q32_32_lg(a)), 2.8768866, 1e-4);
  ASSERT_NEAR(Q32_32_to_double(Q32_32_lb(a)), 9.5568106, 1e-4);
  ASSERT_NEAR(Q32_32_to_double(Q32_32_log(a, base)), 1.0819573, 1e-4);
}

TEST(f32p32, trigo_cordic) {
  Q32_32 s, c;
  double i = -2 * M_PI;
  while (i < 6 * M_PI) {
    Q32_32_cordic(Q32_32_from_double(i), Q32_32_HALF_BIT, &s, &c);
    ASSERT_NEAR(Q32_32_to_double(s), sin(i), 1e-4);
    ASSERT_NEAR(Q32_32_to_double(Q32_32_sin(Q32_32_from_double(i))), sin(i),
                1e-4);

    ASSERT_NEAR(Q32_32_to_double(c), cos(i), 1e-4);
    ASSERT_NEAR(Q32_32_to_double(Q32_32_cos(Q32_32_from_double(i))), cos(i),
                1e-4);

    ASSERT_NEAR(Q32_32_to_double(Q32_32_tan(Q32_32_from_double(i))), tan(i),
                1e-4);
    i += 0.1;
  }
}

TEST(f32p32, atan) {
  double i = -2 * M_PI;
  while (i < 6 * M_PI) {

    ASSERT_NEAR(Q32_32_to_double(Q32_32_atan(Q32_32_from_double(i))), atan(i),
                1e-4);
    i += 0.1;
  }
}

TEST(f32p32, atan2) {
  double i = -2 * M_PI;
  Q32_32 r;
  while (i < 6 * M_PI) {
    ASSERT_TRUE(Q32_32_atan2(Q32_32_from_double(i), Q32_32_ONE, &r));
    ASSERT_NEAR(Q32_32_to_double(r), atan2(i, 1), 1e-4);
    i += 0.1;
  }

  i = -6 * M_PI;
  double j = -2 * M_PI;
  while (i < 2 * M_PI) {
    ASSERT_TRUE(Q32_32_atan2(Q32_32_from_double(i), Q32_32_from_double(j), &r));
    ASSERT_NEAR(Q32_32_to_double(r), atan2(i, j), 1e-4);
    ASSERT_TRUE(Q32_32_atan2(Q32_32_from_double(j), Q32_32_from_double(i), &r));
    ASSERT_NEAR(Q32_32_to_double(r), atan2(j, i), 1e-4);
    i += 0.1;
    j += 0.1;
  }

  /* x == 0 && y == 0 */
  ASSERT_FALSE(Q32_32_atan2(0, 0, &r));
  ASSERT_NEAR(Q32_32_to_double(r), atan2(0, 0), 1e-4);

  /* x > 0 && y == 0 */
  ASSERT_TRUE(Q32_32_atan2(0, 1, &r));
  ASSERT_EQ(r, 0);
  ASSERT_NEAR(Q32_32_to_double(r), atan2(0, 1), 1e-4);
  /* x < 0 && y == 0 */
  ASSERT_TRUE(Q32_32_atan2(0, -1, &r));
  ASSERT_EQ(r, Q32_32_PI);
  ASSERT_NEAR(Q32_32_to_double(r), atan2(0, -1), 1e-4);

  /* x == 0 && y != 0 */
  /* x == 0 && y < 0 */
  ASSERT_TRUE(Q32_32_atan2(1, 0, &r));
  ASSERT_EQ(r, Q32_32_PI_2);
  ASSERT_NEAR(Q32_32_to_double(r), atan2(1, 0), 1e-4);
  /* x == 0 && y > 0 */
  ASSERT_TRUE(Q32_32_atan2(-1, 0, &r));
  ASSERT_EQ(r, -Q32_32_PI_2);
  ASSERT_NEAR(Q32_32_to_double(r), atan2(-1, 0), 1e-4);
}

TEST(f32p32, asin) {
  double i = -2 * M_PI;
  Q32_32 r;
  while (i < 6 * M_PI) {

    bool ret = Q32_32_asin(Q32_32_from_double(i), &r);
    if (abs(i) > 1)
      ASSERT_FALSE(ret);
    else {
      ASSERT_TRUE(ret);
      ASSERT_NEAR(Q32_32_to_double(r), asin(i), 1e-4);
    }
    i += 0.1;
  }
}
TEST(f32p32, acos) {
  double i = -2 * M_PI;
  Q32_32 r;
  while (i < 6 * M_PI) {

    bool ret = Q32_32_acos(Q32_32_from_double(i), &r);
    if (abs(i) > 1)
      ASSERT_FALSE(ret);
    else {
      ASSERT_TRUE(ret);
      ASSERT_NEAR(Q32_32_to_double(r), acos(i), 1e-4);
    }
    i += 0.1;
  }
}

TEST(f32p32, trigo_cordich) {
  Q32_32 s, c, t;
  double i = 0.1;
  while (i < 1) {
    s = Q32_32_sinh(Q32_32_from_double(i));
    c = Q32_32_cosh(Q32_32_from_double(i));
    t = Q32_32_tanh(Q32_32_from_double(i));
    std::cout << i << std::endl;
    ASSERT_NEAR(Q32_32_to_double(s), sinh(i), 1e-4);

    ASSERT_NEAR(Q32_32_to_double(c), cosh(i), 1e-4);

    ASSERT_NEAR(Q32_32_to_double(t), tanh(i), 1e-4);
    i += 0.1;
  }
}

/*
TEST (f32p32, atanh)
{
    double i = 0;
    while (i < 1) {

        ASSERT_NEAR (Q32_32_to_double (Q32_32_atanh (Q32_32_from_double (i))),
atanh (i), 1e-4); i += 0.1;
    }
}
*/

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
