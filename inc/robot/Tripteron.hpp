#pragma once

#include <semaphore>
#include <span>
#include <thread>

#include "peripherals/GPIO.hpp"
#include "robot/Axis.hpp"
#include "robot/Motor.hpp"
#include "utils/print.hpp"

namespace Robot {
  class Tripteron final {
   private:
    struct X {
      using Motor = Motor<23, 25>;
      using Sensor = Peripherals::GPIO::Input<14, Peripherals::GPIO::Edge::FALLING, Peripherals::GPIO::Pull::UP>;
      using Axis = Axis<Motor, Sensor>;
    };

    struct Y {
      using Motor = Motor<22, 26>;
      using Sensor = Peripherals::GPIO::Input<12, Peripherals::GPIO::Edge::FALLING, Peripherals::GPIO::Pull::UP>;
      using Axis = Axis<Motor, Sensor>;
    };

    struct Z {
      using Motor = Motor<32, 27>;
      using Sensor = Peripherals::GPIO::Input<13, Peripherals::GPIO::Edge::FALLING, Peripherals::GPIO::Pull::UP>;
      using Axis = Axis<Motor, Sensor>;
    };

    X::Axis x;
    Y::Axis y;
    // Z::Axis z;

    static inline std::counting_semaphore<3> motors_done{ 0 };

   public:
    struct Position {
      uint16_t x;
      uint16_t y;
      uint16_t z;
    };

    Tripteron() {}

    auto calibrate() {
      // auto [worker_x, worker_y, worker_z] = std::tuple{
      auto [worker_x, worker_y] = std::tuple{
        std::thread{ [&]() {
          x.calibrate();
          motors_done.release();
          Utils::println<Utils::Colors::CYAN>("X calibrated");
        } },
        std::thread{ [&]() {
          y.calibrate();
          motors_done.release();
          Utils::println<Utils::Colors::CYAN>("Y calibrated");
        } },
        // std::thread{ [&]() {
        //   z.calibrate();
        //   motors_done.release();
        //   Utils::println<Utils::Colors::CYAN>("Z calibrated");
        // } }
      };

      worker_x.detach();
      worker_y.detach();
      // worker_z.detach();

      motors_done.acquire();
      motors_done.acquire();
      // motors_done.acquire();
    }

    auto move(const std::span<const Position> trajectory) -> void {
      for (const auto pos : trajectory) {
        // Utils::println<Utils::Colors::YELLOW>("pos = [ {}, {} ]", pos.x, pos.y);

        // auto [worker_x, worker_y, worker_z] = std::tuple{
        auto [worker_x, worker_y] = std::tuple{
          std::thread{ [&]() {
            x.move(pos.x, true);
            motors_done.release();
          } },

          std::thread{
            [&]() {
              y.move(pos.y, true);
              motors_done.release();
            } },

          // std::thread{ [&]() {
          //   z.move(pos.z, true);
          //   motors_done.release();
          // } },
        };

        worker_x.detach();
        worker_y.detach();
        // worker_z.detach();

        motors_done.acquire();
        motors_done.acquire();
        // motors_done.acquire();
      }

      // auto [worker_x, worker_y, worker_z] = std::tuple{
      auto [worker_x, worker_y] = std::tuple{
        std::thread{ [&]() {
          x.move(50_percent, true);
          motors_done.release();
        } },

        std::thread{
          [&]() {
            y.move(50_percent, true);
            motors_done.release();
          } },

        // std::thread{ [&]() {
        //   z.move(50_percent, true);
        //   motors_done.release();
        // } },
      };

      worker_x.detach();
      worker_y.detach();
      // worker_z.detach();

      motors_done.acquire();
      motors_done.acquire();
      // motors_done.acquire();
    }

    auto where() -> Position {
      return {
        .x = x.where(),
        .y = y.where(),
        // .z = z.where()
      };
    }

    auto stop() -> void {
      x.stop();
      y.stop();
      // z.stop();
    }
  };
}  // namespace Robot
