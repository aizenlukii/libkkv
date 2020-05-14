/**
 *
 * @file    test.h
 * @author  damianzim
 * @date    2020-04-11
 *
 */

#ifndef KKV_EXAMPLE_TEST_TEST_H_
#define KKV_EXAMPLE_TEST_TEST_H_

#include <map>
#include <string>

enum Mode {
  DB = 1,
  Slice,
};

static std::map<std::string, int> modes = {
  { "db", Mode::DB },
  { "slice", Mode::Slice },
};


void test_db();
void test_slice();

#endif  // KKV_EXAMPLE_TEST_TEST_H_
