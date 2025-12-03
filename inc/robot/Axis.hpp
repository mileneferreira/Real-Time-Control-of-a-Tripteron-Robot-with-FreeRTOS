#pragma once

#include <cstdlib>
#include <thread>

#include "peripherals/GPIO.hpp"
#include "robot/Motor.hpp"
#include "utils/Percentage.hpp"

namespace Robot {
  template <IsMotor Motor, Peripherals::GPIO::IsInput EndSensor>
  class Axis final {
   private:
    Motor motor;
    uint16_t pos = 0;

    uint16_t steps_at_100percent = 0;

   public:
    Axis() {}

    auto move(uint16_t target_percentage, bool sync = false) -> void {
      Utils::println<Utils::Colors::GREEN>("Axis.move({}, {})", target_percentage, sync);

      using namespace Utils::literals;
      if (target_percentage < 0 or target_percentage > 100_percent)
        return Utils::println<Utils::Colors::YELLOW>("Can't go to this position");

      const int32_t delta = static_cast<int32_t>(target_percentage) - static_cast<int32_t>(pos);
      if (delta == 0)
        return;

      const auto dir = (delta > 0 ? Motor::Direction::COUNTER_CLOCKWISE : Motor::Direction::CLOCKWISE);
      const auto steps = std::abs(delta) * steps_at_100percent / 100_percent;

      motor.move(dir, steps);
      if (sync)
        motor.wait();

      pos = target_percentage;
    }

    auto where() const -> uint16_t { return pos; }

    auto wait() -> void { motor.wait(); }

    auto stop() -> void { motor.stop(); }

    auto calibrate() -> void {
      EndSensor::initialize();
      using namespace std::chrono_literals;
      std::this_thread::sleep_for(250ms);

      static constexpr auto END_SENSOR_ACTIVE = (EndSensor::pull == Peripherals::GPIO::Pull::UP ? Peripherals::GPIO::Level::HIGH : Peripherals::GPIO::Level::LOW);
      static constexpr auto CALIBRATION_STEP = 50;
      do {
        motor.move(Motor::Direction::COUNTER_CLOCKWISE, CALIBRATION_STEP, true);
      } while (EndSensor::read() == END_SENSOR_ACTIVE);

      steps_at_100percent = 0;
      do {
        motor.move(Motor::Direction::CLOCKWISE, CALIBRATION_STEP, true);
        steps_at_100percent += CALIBRATION_STEP;
      } while (EndSensor::read() == END_SENSOR_ACTIVE);

      Utils::println<Utils::Colors::RED>("steps_at_100percent: {}", steps_at_100percent);
      move(50_percent, true);
    }
  };
}  // namespace Robot
