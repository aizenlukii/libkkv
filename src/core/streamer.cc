/**
 *
 * @file    streamer.cc
 * @author  damianzim
 * @date    2020-04-14
 *
 */

#include "core/streamer.h"

#include <spdlog/sinks/stdout_color_sinks.h>

namespace kkv {

namespace {

using constants::SectorSize;

constexpr auto kMinEmptyBufferSize = static_cast<uint32_t>(SectorSize::k4KiB);
constexpr auto kMaxEmptyBufferSize = static_cast<uint32_t>(SectorSize::k1MiB);

} // namespace anonymous

// Stream

const std::string& Stream::Name() const {
  static const auto name = path_.filename().string();
  return name;
}

const fs::path& Stream::Path() const { return path_; }

bool Stream::Open() {
  file_ = fopen(path_.c_str(), "r+b");
  return file_ != nullptr;
}

// Streamer

Status Streamer::Init(const Paths& paths) {
  size_t i = 0;
  try {
    streams_ = new std::unique_ptr<Stream>[streams_no_];

    for (i = 0; i < streams_no_; ++i)
      streams_[i] = std::make_unique<Stream>(paths.PathPartitionFile(i));
  } catch (const std::bad_alloc& err) {
    SPDLOG_CRITICAL(err.what());
    return Status(State::kFatal, "Cannot allocate memory for Streams objects.");
  }

  Result s;
  for (i = 0; i < streams_no_; ++i) {
    s = FileSystemUtils::CreateFile(streams_[i]->Path());
    if (s == Result::kError)
      return Status(State::kFatal, "Could not create partition file");

    if (!streams_[i]->Open()) {
      spdlog::critical("Cannot open file ({}) {}", i,
                       paths.PathPartitionFile(i));
      return Status(State::kFatal, "Could not open partition file");
    } else {
      if (s == Result::kOk && !AllocateEmptySpace(streams_[i].get())) {
        spdlog::critical("Allocating empty space in partition ({}) failed", i);
        return Status(State::kFatal,
                      "Could not allocate space in partition file");
      }
    }
  }

  delete[] buffer_;
  buffer_ = nullptr;

  try {
    InitLogger();
    log_->debug("Streamer initialized successfully");
  } catch (const spdlog::spdlog_ex& err) {
    SPDLOG_WARN(err.what());
    spdlog::warn("Cannot initializer streamer logger");
  }

  return Status();
}

const size_t &Streamer::CalcEmptyBufferSize() const {
  static const size_t size = [](uint64_t all_sectors_size) {
    for (auto i = kMaxEmptyBufferSize; i >= kMinEmptyBufferSize; i >>= 1U) {
      if (i <= all_sectors_size)
        return i;
    }

    return kMinEmptyBufferSize;
  }(config_->GetAllSectorsSize());

  return size;
}

bool Streamer::AllocateEmptySpace(const Stream* stream) {
  static const auto buffer_size = CalcEmptyBufferSize();

  if (buffer_ == nullptr) {
    try {
      // TODO: If this fails, try allocating smaller buffer.
      // Then the kMaxEmptyBufferSize can be boldly increased.
      buffer_ = new TByte[buffer_size];
    } catch (const std::bad_alloc& err) {
      SPDLOG_CRITICAL(err.what());
      return false;
    }
  }

  if (!FileUtils::Rewind(stream->file_, stream->Name()))
    return false;

  uint64_t saved = 0;
  while (saved < config_->GetAllSectorsSize()) {
    if (!fwrite(buffer_, buffer_size, 1, stream->file_)) {
      spdlog::error("Could not write to the file {}", stream->Name());
      break;
    } else {
      saved += buffer_size;
      SPDLOG_TRACE("{} saved: {}, total: {}", stream->Name(),
                   buffer_size, saved);
    }
  }

  return saved == config_->GetAllSectorsSize();
}

void Streamer::InitLogger() { log_ = spdlog::stdout_color_mt("streamer"); }

} // namespace kkv
