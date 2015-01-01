#ifndef DYABL_HASH_FUNCTIONS_H__
#define DYABL_HASH_FUNCTIONS_H__

#include <stdint.h>
/*
 * TODO
 *
 * http://eternallyconfuzzled.com/tuts/algorithms/jsw_tut_hashing.aspx
 * https://en.wikipedia.org/wiki/Universal_hashing
 * http://www.cse.yorku.ca/~oz/hash.html
 * https://www.strchr.com/hash_functions
 * https://131002.net/siphash/
 * http://aras-p.info/blog/2016/08/02/Hash-Functions-all-the-way-down/
 * https://en.wikibooks.org/wiki/Algorithm_Implementation/Hashing
 *
 *
 *
 *
 * INTEGER
 * https://www.cs.hmc.edu/~geoff/classes/hmc.cs070.200101/homework10/hashfuncs.html
 * https://stackoverflow.com/questions/664014/what-integer-hash-function-are-good-that-accepts-an-integer-hash-key
 * http://burtleburtle.net/bob/hash/integer.html
 * thomas WANG
 * https://gist.github.com/badboy/6267743
 * https://gist.github.com/lh3/974ced188be2f90422cc
 *
 */

/* Strings */
/**
 * @brief Hash a string by adding value of it's char.
 *
 * @param[in] s The string to hash. It's a const void, just to be generic.
 *
 * @return Hash value of string (addition).
 */
uint32_t SH_add(const void *s);

// https://www.cs.hmc.edu/~geoff/classes/hmc.cs070.200101/homework10/hashfuncs.html
/**
 * @brief SH_pjw
 * orignal from
 * https://www.cs.hmc.edu/~geoff/classes/hmc.cs070.200101/homework10/hashfuncs.html
 * @code
 *    // The top 4 bits of h are all zero
 *    h = (h << 4) + ki               // shift h 4 bits left, add in ki
 *    g = h & 0xf0000000              // get the top 4 bits of h
 *    if (g != 0)                     // if the top 4 bits aren't zero,
 *       h = h ^ (g >> 24)            //   move them to the low end of h
 *       h = h ^ g
 *    // The top 4 bits of h are again all zero
 * @endcode
 * @param[in] s The string to hash. It's a const void, just to be generic.
 *
 * @return Hash value of string
 */
uint32_t SH_pjw(const void *s);

/**
 * @brief SH_crc
 * orignal from
 * https://www.cs.hmc.edu/~geoff/classes/hmc.cs070.200101/homework10/hashfuncs.html
 * @code
 *    highorder = h & 0xf8000000    // extract high-order 5 bits from h
 *                                  // 0xf8000000 is the hexadecimal
 * representation
 *                                  //   for the 32-bit number with the first
 * five
 *                                  //   bits = 1 and the other bits = 0
 *    h = h << 5                    // shift h left by 5 bits
 *    h = h ^ (highorder >> 27)     // move the highorder 5 bits to the
 * low-order
 *                                  //   end and XOR into h
 *    h = h ^ ki                    // XOR h and ki
 * @endcode
 * @param[in] s The string to hash. It's a const void, just to be generic.
 *
 * @return Hash value of string
 */
uint32_t SH_crc(const void *s);

/**
 * @brief SH_elf is an unix variant of PJW.
 * @param[in] s The string to hash. It's a const void, just to be generic.
 *
 * @return Hash value of string
 */
uint32_t SH_elf(const void *s);

/**
 * @brief SH_jenkins Jenkins one-at-a-time hash functions.
 * @param[in] s The string to hash. It's a const void, just to be generic.
 *
 * @return Hash value of string
 */
uint32_t SH_jenkins(const void *s);

// add murmur

#endif // DYABL_HASH_FUNCTIONS_H__
