/**
 *
 * @file    filesystem.h
 * @author  damianzim
 * @date    2020-04-07
 *
 */

#ifndef KKV_UTILITY_FILESYSTEM_H_
#define KKV_UTILITY_FILESYSTEM_H_

#include <filesystem>
#include <string>

#include "kkv/slice.h"
#include "kkv/status.h"

namespace kkv {

namespace fs = std::filesystem;

struct FileUtils {
  static bool Rewind(FILE* file, const Slice& filename = "unknown");
};

struct FileSystemUtils {
  static bool IsFile(const fs::path& path) noexcept;
  static bool IsDir(const fs::path& path) noexcept;

  static Result CreateDir(const fs::path& path, bool allow_exist = true)
      noexcept;
  static Result CreateFile(const fs::path& path, bool allow_exist = false)
      noexcept;

  // The file name is not necessary, it is only used for logging and is unknown
  // by default.
  static Result LockFile(FILE* stream, const Slice& filename = "%");
  // The file name is not necessary, it is only used for logging and is unknown
  // by default.
  static Result UnlockFile(FILE* stream, const Slice& filename = "%");

  static bool CloseFile(FILE* file);
};

class Paths : public FileSystemUtils {
 public:
  Paths(const Slice& path) : base_(fs::absolute(path.Data())) {};
  explicit Paths(const std::string& path) : base_(fs::absolute(path)) {};

  const fs::path& Base();

  [[nodiscard]] const fs::path& PathConfigFile() const;
  [[nodiscard]] const fs::path& PathLockFile() const;

  fs::path PathPartitionFile(size_t &partition_no) const;

 private:
  fs::path base_;
};

} // namespace kkv

#endif  // KKV_UTILITY_FILESYSTEM_H_
