/**
 *
 * @file    utils.h
 * @author  damianzim
 * @date    2020-04-16
 *
 */

#ifndef KKV_UTILITY_UTILS_H_
#define KKV_UTILITY_UTILS_H_

#include <stdint.h>

namespace kkv {

namespace math {

inline bool IsPowerOf2(uint32_t a) { return a && !(a & (a - 1)); }

inline uint32_t RoundUpPowerOf2(uint32_t a) {
  a--;
  a |= a >> 1U;
  a |= a >> 2U;
  a |= a >> 4U;
  a |= a >> 8U;
  a |= a >> 16U;
  a++;

  return a;
}

} // namespace math

} // namespace kkv

#endif  // KKV_UTILITY_UTILS_H_
