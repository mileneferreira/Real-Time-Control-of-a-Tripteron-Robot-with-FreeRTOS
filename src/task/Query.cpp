#include "task/Query.h"

#include "esp_pthread.h"
#include "portmacro.h"

namespace Task {
  auto Query::this_thread::name() -> const char* { return pcTaskGetName(nullptr); }

  auto Query::this_thread::core() -> BaseType_t { return xPortGetCoreID(); }

  auto Query::this_thread::priority() -> UBaseType_t { return uxTaskPriorityGet(nullptr); }

  auto Query::this_thread::free_stack() -> UBaseType_t { return uxTaskGetStackHighWaterMark(nullptr); }

  auto Query::pthread::name() -> const char* {
    esp_pthread_cfg_t config;
    esp_pthread_get_cfg(&config);
    return config.thread_name;
  }

  auto Query::pthread::core() -> int {
    esp_pthread_cfg_t config;
    esp_pthread_get_cfg(&config);
    return config.pin_to_core;
  }

  auto Query::pthread::priority() -> size_t {
    esp_pthread_cfg_t config;
    esp_pthread_get_cfg(&config);
    return config.prio;
  }

  auto Query::pthread::stack_size() -> size_t {
    esp_pthread_cfg_t config;
    esp_pthread_get_cfg(&config);
    return config.stack_size;
  }
}  // namespace Task
