/**
 *
 * @file    options.cc
 * @author  damianzim
 * @date    2020-04-10
 *
 */

#include "kkv/options.h"

#include "kkv/logger.h"
#include "utility/utils.h"

namespace kkv {

using namespace constants;

// DBConfig

bool DBConfig::IsValidPartitionsCount(const uint16_t n) {
  return n >= kMinPartitionsCount && n <= kMaxPartitionsCount;
}

bool DBConfig::IsValidSlotsCount(const uint16_t n) {
  return n >= kMinSlotsCount && n <= kMaxSlotsCount;
}

bool DBConfig::IsValidSectorSize(const DBConfig& config, const uint32_t size) {
  const auto min = static_cast<uint32_t>(SectorSize::k4KiB);
  const auto max = GetMaxSectorSize(config.slots_count);
  if (!max) {
    spdlog::warn("Invalid slots count ({})", config.slots_count);
    return false;
  }

  return !(!math::IsPowerOf2(size) || size < min || size > max);
}

// Returns 0 if the slots_count parameter is not valid
uint32_t DBConfig::GetMaxSectorSize(const uint16_t slots_count) {
  if (!IsValidSlotsCount(slots_count))
    return 0;

  uint32_t max = kMaxPartitionSize / slots_count;
  const uint32_t max_rounded = math::RoundUpPowerOf2(max);
  max = max_rounded == max ? max : max_rounded >> 1U;

  return max;
}

// BaseConfiguration

uint16_t BaseConfiguration::GetPartitionsCount() const {
  return config_.partitions_count;
}

uint16_t BaseConfiguration::GetSlotsCount() const {
  return config_.slots_count;
}

uint32_t BaseConfiguration::GetSectorSize() const {
  return config_.sector_size;
}

uint64_t BaseConfiguration::GetAllSectorsSize() const {
  return 1ULL * config_.slots_count * config_.sector_size;
}

void BaseConfiguration::SetPartitionsCount(const uint16_t n) {
  is_config_touched_ = true;

  if (n < kMinPartitionsCount) {
    config_.partitions_count = kMinPartitionsCount;
    spdlog::warn("New partitions count ({}) is too small MIN = {}", n,
                 kMinPartitionsCount);
  } else if (n > kMaxPartitionsCount) {
    config_.partitions_count = kMaxPartitionsCount;
    spdlog::warn("New partitions count ({}) is too large MAX = {}", n,
                 kMaxPartitionsCount);
  } else {
    config_.partitions_count = n;
  }

  spdlog::debug("Partitions count has been set to ({})",
                config_.partitions_count);
}

void BaseConfiguration::SetSlotsCount(const uint16_t n) {
  is_config_touched_ = true;

  if (n < kMinSlotsCount) {
    config_.slots_count = kMinSlotsCount;
    spdlog::warn("New slots count ({}) is too small MIN = {}", n,
                 kMinSlotsCount);
  } else if (n > kMaxSlotsCount) {
    config_.slots_count = kMaxSlotsCount;
    spdlog::warn("New slots count ({}) is too large MAX = {}", n,
                 kMaxSlotsCount);
  } else {
    config_.slots_count = n;
  }

  spdlog::debug("Slots count has been set to ({})", config_.slots_count);
}

// If an invalid (too large) sector size is given, it will be restored to the
// default one.
void BaseConfiguration::SetSectorSize(const SectorSize& size) {
  // TODO: This method may allow a sector size that is not allowed. This can
  // happen when setting the sector size before setting the number of slots.

  is_config_touched_ = true;
  const auto param_size = static_cast<uint32_t>(size);

  if (!DBConfig::IsValidSectorSize(config_, param_size)) {
    config_.sector_size = static_cast<uint32_t>(SectorSize::kDefaultSectorSize);
    spdlog::warn("Invalid new sector size ({})", param_size);
  } else {
    config_.sector_size = param_size;
  }

  spdlog::debug("Sector size has been set to ({}) Bytes", config_.sector_size);
}

} // namespace kkv
