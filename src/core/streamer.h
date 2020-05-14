/**
 *
 * @file    streamer.h
 * @author  damianzim
 * @date    2020-04-14
 *
 */

#ifndef KKV_CORE_STREAMER_H_
#define KKV_CORE_STREAMER_H_

#include <stdio.h>

#include <filesystem>
#include <memory>

#include "common/types.h"
#include "kkv/logger.h"
#include "kkv/options.h"
#include "kkv/status.h"
#include "utility/filesystem.h"

namespace kkv {

namespace fs = std::filesystem;

class Streamer;

class Stream {
 public:
  explicit Stream(fs::path path) : path_(std::move(path)), file_(nullptr) {
    SPDLOG_DEBUG("Initializing a stream (path=\"{}\")", path_.c_str());
  }

  ~Stream() {
    SPDLOG_DEBUG("Destructing a stream (path=\"{}\")", path_.c_str());
    FileSystemUtils::CloseFile(file_);
  }

  [[nodiscard]] const std::string& Name() const;
  [[nodiscard]] const fs::path& Path() const;

  bool Open();

 private:
  friend class Streamer;

  const fs::path path_;

  FILE* file_;
};

class Streamer {
 public:
  explicit Streamer(const std::shared_ptr<BaseConfiguration> config)
      : config_(config),
        streams_no_(config->GetPartitionsCount()),
        buffer_(nullptr),
        streams_(nullptr) {};

  ~Streamer() { delete[] buffer_; }

  Status Init(const Paths& paths);

  bool AllocateEmptySpace(const Stream* stream);

 private:
  [[nodiscard]] const size_t &CalcEmptyBufferSize() const;

  void InitLogger();

  const std::shared_ptr<BaseConfiguration> config_;
  const size_t streams_no_;

  TByte *buffer_; // Used for empty buffer
  std::shared_ptr<spdlog::logger> log_;
  std::unique_ptr<Stream>* streams_;

};

} // namespace kkv

#endif  // KKV_CORE_STREAMER_H_
