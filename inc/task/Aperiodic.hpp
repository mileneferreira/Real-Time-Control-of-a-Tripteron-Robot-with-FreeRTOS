#pragma once

#include <thread>
#include <utility>

#include "freertos/idf_additions.h"
#include "task/Config.h"
#include "task/Query.h"
#include "utils/print.hpp"

namespace Task {
  using isr_handler_t = void (*)(void*);

  namespace {
    template <typename T>
    concept HasRegisterInterrupt = requires(isr_handler_t handler, void* arg) {
      { T::register_interrupt(handler, arg) } -> std::same_as<void>;
    };

    template <typename T>
    concept HasUnregisterInterrupt = requires {
      { T::unregister_interrupt() } -> std::same_as<void>;
    };
  }  // namespace

  template <typename T>
  concept Event = HasRegisterInterrupt<T> && HasUnregisterInterrupt<T>;

  template <Event Trigger>
  struct Aperiodic final {
   private:
    SemaphoreHandle_t trigger_sem;
    std::thread worker;

    template <typename FnType, typename... ArgsType>
    static auto wrap(SemaphoreHandle_t sem, FnType&& fn, ArgsType&&... args) {
      if (Query::pthread::priority() == Config::AUTOMATIC_PRIORITY)
        Config(true).with_priority(Config::MAX_PRIORITY);

      auto wrapper = [=]() {
        while (true)
          if (xSemaphoreTake(sem, portMAX_DELAY) == pdTRUE)
            fn(args...);
      };
      return wrapper;
    }

    static void IRAM_ATTR isr_handler(void* arg) {
      Aperiodic* self = static_cast<Aperiodic*>(arg);

      if (BaseType_t xHigherPriorityTaskWoken = pdFALSE; self->trigger_sem)
        xSemaphoreGiveFromISR(self->trigger_sem, &xHigherPriorityTaskWoken);
    }

   public:
    template <typename FnType, typename... ArgsType>
    Aperiodic(FnType&& fn, ArgsType&&... args) : trigger_sem(xSemaphoreCreateBinary()), worker(wrap(trigger_sem, std::forward<FnType>(fn), std::forward<ArgsType>(args)...)) {
      Trigger::register_interrupt(isr_handler, this);
      worker.detach();
    }

    ~Aperiodic() {
      Trigger::unregister_interrupt();

      if (trigger_sem)
        vSemaphoreDelete(trigger_sem);

      if (worker.joinable())
        worker.join();
    }
  };
};  // namespace Task
