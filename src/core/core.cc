/**
 *
 * @file    dbcore.cc
 * @author  damianzim
 * @date    2020-04-07
 *
 */

#include "core/core.h"

#include <string>

#include "kkv/logger.h"
#include "kkv/options.h"
#include "kkv/status.h"

namespace kkv {

DBCore::DBCore(const Slice& path, Options& options)
    : options_(std::make_shared<Options>(options)),
      configuration_(nullptr),
      lock_file_(nullptr),
      paths_(path),
      streamer_(nullptr) {
  status_.SetState(Paths::CreateDir(paths_.Base()));
  if (!status_.ok()) {
    status_.SetMessage("Cannot create DB directory");
    return;
  }

  lock_file_ = fopen(paths_.PathLockFile().c_str(), "w");
  status_.SetState(Paths::LockFile(lock_file_, paths_.PathLockFile().c_str()));
  if (!status_.ok()) {
    status_.SetMessage("Cannot lock file: DB::LOCK");
    return;
  }

  if (kkv::Paths::IsFile(paths_.PathConfigFile()) &&
      options_->error_when_exists_) {
    status_.Set(State::kError, "DB already exists");
    return;
  }

  try {
    configuration_ = std::make_unique<Configuration>(paths_.PathConfigFile(),
                                                     options_);
  } catch (const std::bad_alloc& err) {
    SPDLOG_CRITICAL(err.what());
    status_.Set(State::kFatal, "Allocation error (Configuration)");
    return;
  }

  status_.SetState(configuration_->Open());
  if (!status_.ok()) {
    status_.SetMessage("Cannot resolve configuration (see logs)");
    return;
  }

  try {
    streamer_ = new Streamer(options_);
  } catch (const std::bad_alloc& err) {
    SPDLOG_CRITICAL(err.what());
    status_.Set(State::kFatal, "Allocation error (Streamer)");
    return;
  }

  if (!(status_ = streamer_->Init(paths_)).ok())
    return;

  status_.Set(State::kOk, "Database initialized successfully");
};

DBCore::~DBCore() {
  delete streamer_;

  Paths::CloseFile(lock_file_);
}

DB::~DB() = default;

Status DB::Open(DB** db, const Slice& path, Options options) {
  *db = nullptr;

  DBCore* dbcore = nullptr;
  Status status;

  try {
    dbcore = new DBCore(path, options);
    status = dbcore->status_;
  } catch (const std::bad_alloc& err) {
    SPDLOG_CRITICAL(err.what());
    status.Set(State::kFatal, "Allocation error (DB)");
  }

  if (status.ok())
    *db = dbcore;
  else
    delete dbcore;

  return status;
}

} // namespace kkv
