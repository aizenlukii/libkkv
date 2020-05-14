/**
 *
 * @file    filesystem.cc
 * @author  damianzim
 * @date    2020-04-07
 *
 */

#include "utility/filesystem.h"

#include <unistd.h>

#include <exception>
#include <fstream>
#include <string_view>

#include "kkv/logger.h"

namespace kkv {

namespace {

namespace db_files {

constexpr std::string_view kConfigFile = "MANIFEST";
constexpr std::string_view kLockFile = "LOCK";

constexpr std::string_view kPartitionPrefix = "DB_";

} // namespace db_files

} // namespace anonymous

// FileUtils

bool FileUtils::Rewind(FILE* file, const Slice& filename) {
  auto s = fseeko(file, SEEK_SET, 0);
  if (s) spdlog::debug("Cannot rewind file \"{}\"", filename.Data());
  return !s;
}

// FileSystemUtils

bool FileSystemUtils::IsFile(const fs::path& path) noexcept {
  bool is_ok = false;
  try {
    is_ok = fs::is_regular_file(path);
  } catch (const std::exception& err) {
    SPDLOG_DEBUG(err.what());
  }

  return is_ok;
}

bool FileSystemUtils::IsDir(const fs::path& path) noexcept {
  bool is_ok = false;
  try {
    is_ok = fs::is_directory(path);
  } catch (const std::exception& err) {
    SPDLOG_DEBUG(err.what());
  }

  return is_ok;
}

Result FileSystemUtils::CreateDir(const fs::path& path, const bool allow_exist)
    noexcept {
  if (IsDir(path)) {
    if (allow_exist) {
      SPDLOG_DEBUG("(Ok) Directory \"{}\" exists", path.c_str());
      return Result::kOk;
    } else {
      spdlog::debug("Directory \"{}\" already exists", path.c_str());
      return Result::kAlreadyExists;
    }
  }

  bool is_ok = false;
  try {
    fs::create_directories(path);
    is_ok = IsDir(path);
  } catch(const std::exception& err) {
    SPDLOG_DEBUG(err.what());
  }

  if (is_ok) {
    SPDLOG_DEBUG("Directory \"{}\" has been created", path.c_str());
    return Result::kOk;
  } else {
    spdlog::error("Cannot create directory \"{}\"", path.c_str());
    return Result::kError;
  }
}

Result FileSystemUtils::CreateFile(const fs::path& path, const bool allow_exist)
    noexcept {
  if (IsFile(path)) {
    if (allow_exist) {
      SPDLOG_DEBUG("(Ok) File \"{}\" exists", path.c_str());
      return Result::kOk;
    } else {
      spdlog::debug("File \"{}\" already exists", path.c_str());
      return Result::kAlreadyExists;
    }
  }

  const std::ofstream file(path);

  if (file.is_open()) {
    SPDLOG_DEBUG("File \"{}\" has been created", path.c_str());
    return Result::kOk;
  } else {
    spdlog::error("Cannot create file \"{}\"", path.c_str());
    return Result::kError;
  }
}

Result FileSystemUtils::LockFile(FILE* stream, const Slice& filename) {
  auto formatted_filename = filename == "%"
                            ? "(unknown)"
                            : ("\"" + filename.ToString() + "\"").c_str();

  auto fn_sep = filename == "unknown" ? "" : "\"";
  if (stream == nullptr) {
    spdlog::warn("There is no file {} to lock", formatted_filename);
    return Result::kNotFound;
  }

  auto s = lockf(fileno(stream), F_TLOCK, 0);
  if (s == -1) {
    if (errno == EACCES)
      spdlog::critical("Attempt to lock a file that is already locked by "
                       "another process {}", formatted_filename);
    else
      spdlog::error("Cannot lock file {}", formatted_filename);

    return Result::kFatal;
  }

  SPDLOG_DEBUG("File {} has been locked", formatted_filename);
  return Result::kOk;
}

Result FileSystemUtils::UnlockFile(FILE* stream, const Slice& filename) {
  auto formatted_filename = filename == "%"
                            ? "(unknown)"
                            : ("\"" + filename.ToString() + "\"").c_str();

  if (stream == nullptr) {
    spdlog::warn("There is no file {} to unlock", formatted_filename);
    return Result::kNotFound;
  }

  auto s = lockf(fileno(stream), F_ULOCK, 0);

  if (!s) {
    spdlog::debug("File {} has been unlocked", formatted_filename);
    return Result::kOk;
  } else {
    spdlog::error("Cannot unlock file {}", formatted_filename);
    return Result::kError;
  }
}

bool FileSystemUtils::CloseFile(FILE* stream) {
  if (stream == nullptr)
    return true;

  auto s = fclose(stream);
  stream = nullptr;

  return !s;
}

// Paths

const fs::path& Paths::Base() { return base_; }

const fs::path& Paths::PathConfigFile() const {
  static const auto file_path = base_ / db_files::kConfigFile;
  return file_path;
}

const fs::path& Paths::PathLockFile() const {
  static const auto file_path = base_ / db_files::kLockFile;
  return file_path;
}

fs::path Paths::PathPartitionFile(size_t &partition_no) const {
  auto partition_path = base_ / db_files::kPartitionPrefix;
  partition_path += std::to_string(partition_no);
  return partition_path;
}

} // namespace kkv
