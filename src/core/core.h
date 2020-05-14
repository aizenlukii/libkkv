/**
 *
 * @file    dbcore.h
 * @author  damianzim
 * @date    2020-04-07
 *
 */

#ifndef KKV_CORE_CORE_H_
#define KKV_CORE_CORE_H_

#include <stdio.h>

#include <memory>

#include "core/configuration.h"
#include "core/streamer.h"
#include "kkv/db.h"
#include "kkv/slice.h"

namespace kkv {

class DBCore : public DB {
 public:
  DBCore(const Slice& path, Options& options);

  DBCore(const DBCore&) = delete;
  DBCore& operator=(const DBCore&) = delete;

  ~DBCore() override;

  Status status_;

 private:
  friend class DB;

  const std::shared_ptr<Options> options_;

  std::unique_ptr<Configuration> configuration_;
  FILE* lock_file_;
  Paths paths_;
  Streamer* streamer_;
};

} // namespace kkv

#endif  // KKV_CORE_CORE_H_
