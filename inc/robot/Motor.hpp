#pragma once

#include <cmath>
#include <vector>

#include "peripherals/GPIO.hpp"
#include "peripherals/RMT.hpp"
#include "soc/clk_tree_defs.h"
#include "utils/Frequency.hpp"
#include "utils/print.hpp"

namespace Robot {
  namespace {
    using namespace Utils::literals;
  }

  /**
   * @brief Stepper Motor Driver using RMT (ESP-IDF v5.x).
   *
   * @tparam StepPin GPIO pin for Step signal.
   * @tparam DirPin GPIO pin for Direction signal.
   */
  template <uint8_t step_pin, uint8_t dir_pin>
  class Motor {
   public:
    enum class Direction {
      CLOCKWISE = static_cast<bool>(Peripherals::GPIO::Level::HIGH),
      COUNTER_CLOCKWISE = static_cast<bool>(Peripherals::GPIO::Level::LOW),
    };

   private:
    using DirectionPin = Peripherals::GPIO::Output<dir_pin>;
    static constexpr auto RMT_FREQ = 10_MHz;
    static constexpr auto SPEED = 500_Hz;

    Peripherals::RMT<step_pin, RMT_FREQ> rmt;
    std::array<rmt_symbol_word_t, 1000> pulse_buffer;

   public:
    Motor() {
      DirectionPin::initialize();

      static constexpr auto HALF_PERIOD = RMT_FREQ / (SPEED * 2);
      static constexpr auto FRAME = rmt_symbol_word_t{
        .duration0 = HALF_PERIOD,
        .level0 = 1,
        .duration1 = HALF_PERIOD,
        .level1 = 0,
      };
      std::fill(pulse_buffer.begin(), pulse_buffer.end(), FRAME);
    }

    /**
     * @brief Move the motor.
     *
     * @param dir Direction to spin the motor in.
     * @param steps Number of steps.
     */
    auto move(Direction dir, size_t steps, bool sync = false) -> void {
      if (steps == 0)
        return;

      DirectionPin::set(static_cast<Peripherals::GPIO::Level>(dir));
      rmt.join();

      while (steps > 0) {
        const auto count = std::min(pulse_buffer.size(), steps);
        rmt.transmit({ pulse_buffer.data(), count }, sync);
        if (sync)
          wait();
        steps -= count;
      }
    }

    /**
     * @brief Blocks until the motor finishes the current move.
     */
    auto wait() -> void { rmt.join(); }

    /**
     * @brief Emergency stop.
     */
    auto stop() -> void { rmt.stop(); }
  };

  template <typename T>
  concept IsMotor = requires(T copy) {
    { Motor{ copy } } -> std::same_as<T>;
  };
}  // namespace Robot
