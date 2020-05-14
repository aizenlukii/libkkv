/**
 *
 * @file    db.h
 * @author  damianzim
 * @date    2020-04-07
 *
 */

#ifndef KKV_DB_H_
#define KKV_DB_H_

#include "kkv/options.h"
#include "kkv/slice.h"
#include "kkv/status.h"

namespace kkv {

class DB {
 public:
  DB() = default;

  DB(const DB&) = delete;
  DB& operator=(const DB&) = delete;

  virtual ~DB();

  static Status Open(DB** db, const Slice& path, Options options = Options());
};

} // namespace kkv

#endif  // KKV_DB_H_
