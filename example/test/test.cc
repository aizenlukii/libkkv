/**
 *
 * @file    test.cc
 * @author  damianzim
 * @date    2020-04-09
 *
 */

#include "test.h"

#include <string.h>

#include <iostream>


int print_help() {
  std::cout << "Usage:" << std::endl;

  for (const auto& mode : modes)
    std::cout << "\t--" << mode.first << std::endl;

  return 0;
}

[[nodiscard]] int is_valid_mode(const char * carg) {
  int mode_num = -1;

  carg += 2;
  std::string arg = carg;

  for (const auto& mode : modes) {
    if (mode.first == arg) {
      std::cout << "EXAMPLE TEST " << mode.first << "\n\n";
      mode_num = mode.second;
      break;
    }
  }

  return mode_num;
}

int main(int argc, char *argv[]) {
  if (argc < 2)
    return print_help();

  if (strlen(argv[1]) > 256 || strncmp(argv[1], "--", 2) != 0)
    return print_help();

  auto mode = is_valid_mode(argv[1]);

  switch (mode) {
    case Mode::DB:
      test_db();
      break;
    case Mode::Slice:
      test_slice();
      break;
    default:
      break;
  }

  return mode ? 0 : print_help();
}
