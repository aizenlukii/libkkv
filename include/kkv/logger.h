/**
 *
 * @file    logger.h
 * @author  damianzim
 * @date    2020-04-20
 *
 */

#ifndef KKV_LOGGER_H_
#define KKV_LOGGER_H_

#define LOGGER_LEVEL_TRACE 0
#define LOGGER_LEVEL_DEBUG 1
#define LOGGER_LEVEL_INFO 2
#define LOGGER_LEVEL_WARN 3
#define LOGGER_LEVEL_ERROR 4
#define LOGGER_LEVEL_CRITICAL 5
#define LOGGER_LEVEL_OFF 6

#ifndef LOGGER_ACTIVE_LEVEL
#define LOGGER_ACTIVE_LEVEL LOGGER_LEVEL_TRACE
#endif

#define SPDLOG_ACTIVE_LEVEL LOGGER_ACTIVE_LEVEL

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

namespace kkv::logger {

inline void Configure() {
  spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%f] [%n] [%^%l%$] %v");
  auto logger = spdlog::stdout_color_mt("KKV");
  spdlog::set_default_logger(logger);
}

} // namespace kkv::logger

#endif  // KKV_LOGGER_H_