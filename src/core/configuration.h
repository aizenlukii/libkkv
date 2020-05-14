/**
 *
 * @file    configuration.h
 * @author  damianzim
 * @date    2020-04-20
 *
 */

#ifndef KKV_CORE_CONFIGURATION_H_
#define KKV_CORE_CONFIGURATION_H_

#include <stdio.h>

#include <memory>
#include <utility>

#include "kkv/options.h"
#include "kkv/status.h"
#include "utility/filesystem.h"

namespace kkv {

class Configuration : public FileSystemUtils {
 public:
  Configuration(const fs::path& path, std::shared_ptr<BaseConfiguration> config)
      : path_(path),
        config_(std::move(config)),
        config_file_(nullptr) {};

  ~Configuration();

  Result Open();

 private:
  bool Dump();
  bool Load();

  const fs::path& path_;

  std::shared_ptr<BaseConfiguration> config_;
  FILE* config_file_;
  bool error_dump_{false};
  bool is_dumped_{false};
};

} // namespace kkv

#endif  // KKV_CORE_CONFIGURATION_H_
