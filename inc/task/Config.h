#pragma once

#include "FreeRTOSConfig.h"
#include "esp_pthread.h"

namespace Task {
  /// Syntatic sugar for creating and applying the pthread
  /// configuration. Upon construction initializes the default
  /// pthread config (using esp_pthread_get_default_config) and
  /// upon destruction call esp_pthread_set_cfg.
  ///
  /// NOTE: The constructor for this class is marked as nodiscard,
  /// so that the user must either change one of the parameters
  /// or acknowledge the default configuration by calling
  /// Task::Config().done()
  class Config {
   private:
    esp_pthread_cfg_t config;

   public:
    /// Minimum priority that can be assigned to a task
    static constexpr size_t MIN_PRIORITY = 0;
    /// Maximum priority that can be assigned to a task
    static constexpr size_t MAX_PRIORITY = configMAX_PRIORITIES - 1;
    /// Special value to signal that the periodic thread should
    /// determine its priority based on the period assigned to it
    static constexpr size_t AUTOMATIC_PRIORITY = configMAX_PRIORITIES + 1;

    /// Create the default configuration for the thread.
    ///
    /// @param keep_current This is used to determine whether the
    /// constructor should start from a default configuration or
    /// take the current one and modify it.
    ///
    /// NOTE: This is marked as nodiscard, so that the user
    /// must either change one of the parameters or acknowledge
    /// the default configuration by calling TaskConfig().done()
    [[nodiscard]] Config(bool keep_current = false);

    /// Explicitly acknowledge that no more configuration will
    /// be done.
    ///
    /// This is meant to make the user explicitly acknowledge
    /// the default configuration from the constructor, it is
    /// not necessary if any configuration is performed.
    auto done() -> void;

    /// Specify the stack size for this thread
    auto with_stack_size(size_t size) -> Config;

    /// Specify the priority of this thread
    auto with_priority(size_t priority) -> Config;

    /// Specify the name of the thread
    auto with_name(const char* name) -> Config;

    /// Specify the core in which the thread will run
    auto pinned_to_core(int core) -> Config;

    /// Children threads will inherit this same configuration
    auto inherit_further() -> Config;

    /// Apply the config using esp_pthread_set_cfg
    ~Config();
  };
}  // namespace Task
