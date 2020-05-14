/**
 *
 * @file    options.h
 * @author  damianzim
 * @date    2020-04-10
 *
 */

#ifndef KKV_OPTIONS_H_
#define KKV_OPTIONS_H_

#include <stdint.h>
#include <sys/types.h>

namespace kkv {

namespace constants {

constexpr uint16_t kMinPartitionsCount = 2;
constexpr uint16_t kMaxPartitionsCount = 64;
constexpr uint16_t kDefaultPartitionsCount = 4;

constexpr uint16_t kMinSlotsCount = 4;
constexpr uint16_t kMaxSlotsCount = 32;
constexpr uint16_t kDefaultSlotsCount = 8;

constexpr auto kMaxPartitionSize = 1ULL << 32U;

namespace { constexpr auto k1KiB = 1024U; }

enum class SectorSize : uint32_t {
  k4KiB   = k1KiB << 2U,
  k8KiB   = k1KiB << 3U,
  k16KiB  = k1KiB << 4U,
  k32KiB  = k1KiB << 5U,
  k64KiB  = k1KiB << 6U,
  k128KiB = k1KiB << 7U,
  k256KiB = k1KiB << 8U,
  k512KiB = k1KiB << 9U,
  k1MiB   = k1KiB << 10U,
  k2MiB   = k1KiB << 11U,
  k4MiB   = k1KiB << 12U,
  k8MiB   = k1KiB << 13U,
  k16MiB  = k1KiB << 14U,
  k32MiB  = k1KiB << 15U,
  k64MiB  = k1KiB << 16U,
  k128MiB = k1KiB << 17U,
  k256MiB = k1KiB << 18U,
  k512MiB = k1KiB << 19U,
  k1GiB   = k1KiB << 20U,

  kDefaultSectorSize = k2MiB,
};

} // namespace constants


struct DBConfig {
  uint16_t partitions_count;
  uint16_t slots_count;
  uint32_t sector_size;

  inline static constexpr size_t TotalSize() {
    return sizeof(partitions_count) + sizeof(slots_count) + sizeof(sector_size);
  }

  static bool IsValidPartitionsCount(uint16_t n);
  static bool IsValidSlotsCount(uint16_t n);
  // slots_count member should be set first to check if the sector size is
  // correct
  static bool IsValidSectorSize(const DBConfig& config, uint32_t size);

  static uint32_t GetMaxSectorSize(uint16_t slots_count);
};

class Configuration;

class BaseConfiguration {
 public:
  BaseConfiguration() : config_({
      constants::kDefaultPartitionsCount,
      constants::kDefaultSlotsCount,
      static_cast<uint32_t>(constants::SectorSize::kDefaultSectorSize)}) {};

  uint16_t GetPartitionsCount() const;
  uint16_t GetSlotsCount() const;
  uint32_t GetSectorSize() const;

  uint64_t GetAllSectorsSize() const;

  void SetPartitionsCount(uint16_t n);
  void SetSlotsCount(uint16_t n);
  void SetSectorSize(const constants::SectorSize& size);

 private:
  friend class Configuration;

  DBConfig config_;
  bool is_config_touched_{false};
};

class Options : public BaseConfiguration {
 public:
  bool error_when_exists_ = false;
};

} // namespace kkv

#endif  // KKV_OPTIONS_H_
