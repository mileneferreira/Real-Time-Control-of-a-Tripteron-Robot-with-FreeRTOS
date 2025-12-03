#include "task/Config.h"

#include <algorithm>

#include "esp_log.h"

namespace Task {
  [[nodiscard]] Config::Config(bool keep_current) {
    if (keep_current) {
      esp_pthread_get_cfg(&config);
    } else {
      config = esp_pthread_get_default_config();
      config.prio = AUTOMATIC_PRIORITY;
    }
  }

  auto Config::done() -> void {}

  auto Config::with_stack_size(size_t size) -> Config {
    config.stack_size = size;
    return *this;
  }

  auto Config::with_priority(size_t priority) -> Config {
    config.prio = std::clamp(priority, MIN_PRIORITY, MAX_PRIORITY);
    return *this;
  }

  auto Config::with_name(const char* name) -> Config {
    config.thread_name = name;
    return *this;
  }

  auto Config::pinned_to_core(int core) -> Config {
    config.pin_to_core = core;
    return *this;
  }

  auto Config::inherit_further() -> Config {
    config.inherit_cfg = true;
    return *this;
  }

  Config::~Config() { esp_pthread_set_cfg(&config); }
}  // namespace Task
