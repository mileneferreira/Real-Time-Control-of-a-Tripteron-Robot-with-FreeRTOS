#pragma once

#include <array>
#include <cmath>
#include <mutex>

#include "driver/ledc.h"
#include "esp_err.h"
#include "hal/ledc_types.h"

namespace Peripherals {
  namespace {
    static constexpr auto SPEED_MODE = LEDC_HIGH_SPEED_MODE;
    static std::array<std::once_flag, LEDC_TIMER_MAX> setup;

    // Helper to install the LEDC Timer configuration once (per unique Timer ID)
    static void install_timer_service(ledc_timer_t timer, ledc_timer_bit_t resolution, uint32_t freq_hz) {
      ledc_timer_config_t timer_cfg = {
        .speed_mode = SPEED_MODE,
        .duty_resolution = resolution,
        .timer_num = timer,
        .freq_hz = freq_hz,
        .clk_cfg = LEDC_AUTO_CLK,
        .deconfigure = false,
      };
      ESP_ERROR_CHECK(ledc_timer_config(&timer_cfg));
    }
  }  // namespace

  enum class PWMTimer {
    TIMER_0 = LEDC_TIMER_0,
    TIMER_1 = LEDC_TIMER_1,
    TIMER_2 = LEDC_TIMER_2,
    TIMER_3 = LEDC_TIMER_3,
  };

  enum class PWMChannel {
    CHANNEL_0 = LEDC_CHANNEL_0,
    CHANNEL_1 = LEDC_CHANNEL_1,
    CHANNEL_2 = LEDC_CHANNEL_2,
    CHANNEL_3 = LEDC_CHANNEL_3,
    CHANNEL_4 = LEDC_CHANNEL_4,
    CHANNEL_5 = LEDC_CHANNEL_5,
    CHANNEL_6 = LEDC_CHANNEL_6,
    CHANNEL_7 = LEDC_CHANNEL_7,
  };

  template <uint8_t pin, PWMTimer timer, PWMChannel channel>
  struct PWM {
    static constexpr auto RESOLUTION = LEDC_TIMER_13_BIT;
    static constexpr auto MAX_DUTY = (1 << RESOLUTION) - 1;
    static constexpr auto TIMER_FREQ = 4000;

    PWM() {
      std::call_once(setup[static_cast<ledc_timer_t>(timer)], install_timer_service, static_cast<ledc_timer_t>(timer), RESOLUTION, TIMER_FREQ);

      ledc_channel_config_t channel_cfg = {
        .gpio_num = pin,
        .speed_mode = SPEED_MODE,
        .channel = static_cast<ledc_channel_t>(channel),
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = static_cast<ledc_timer_t>(timer),
        .duty = 0,  // Start with 0% duty cycle (OFF)
        .hpoint = 0,
        .sleep_mode = LEDC_SLEEP_MODE_NO_ALIVE_NO_PD,
        .flags = { .output_invert = 0 },
      };
      ESP_ERROR_CHECK(ledc_channel_config(&channel_cfg));
    }

    auto stop() {
      ledc_set_duty(SPEED_MODE, static_cast<ledc_channel_t>(channel), 0);
      ledc_update_duty(SPEED_MODE, static_cast<ledc_channel_t>(channel));
    }

    auto set(uint32_t freq, double duty_cycle) -> void {
      if (freq == 0)
        return stop();

      // 1. Set the frequency for the timer (must be done before duty cycle)
      ledc_set_freq(SPEED_MODE, static_cast<ledc_timer_t>(timer), freq);

      // 2. Calculate the duty cycle value based on the resolution
      uint32_t duty_value = std::round(MAX_DUTY * duty_cycle);

      ledc_set_duty(SPEED_MODE, static_cast<ledc_channel_t>(channel), duty_value);
      ledc_update_duty(SPEED_MODE, static_cast<ledc_channel_t>(channel));
    }

    ~PWM() { stop(); }
  };

  template <typename T>
  concept IsPWM = requires(T copy) {
    { PWM{ copy } } -> std::same_as<T>;
  };
}  // namespace Peripherals
