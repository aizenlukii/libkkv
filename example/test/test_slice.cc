/**
 *
 * @file    test_slice.cc
 * @author  damianzim
 * @date    2020-04-28
 *
 */

#include <iostream>

#include <kkv/slice.h>

void test_slice() {
  using kkv::Slice;

  Slice s1("test");
  Slice s2("te");

  std::cout << s1.Data() << std::endl;

  std::cout << s1.HasPrefix(s2) << std::endl;

  s2 = "st";
  std::cout << s1.HasSuffix(s2) << std::endl;

  Slice s3("abcde");
  std::cout << s3.Substr(2, 7).Data() << std::endl;

  Slice s4 = s3.Copy().Right(3).Substr(1);
  std::cout << (s4 == Slice("de")) << std::endl;
}
