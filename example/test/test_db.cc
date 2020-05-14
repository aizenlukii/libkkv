/**
 *
 * @file    test_db.cc
 * @author  damianzim
 * @date    2020-04-09
 *
 */

#include <iostream>

#include <kkv/api.h>

void test_db() {
  kkv::logger::Configure();
  spdlog::set_level(spdlog::level::trace);

  std::string path = "container/";
  kkv::DB* db;

  kkv::Options options;
  options.SetSectorSize(kkv::constants::SectorSize::k256KiB);

  kkv::Status s = kkv::DB::Open(&db, path, options);

  std::cout << "Status: " << s.ToString() << std::endl;

  delete db;
}

