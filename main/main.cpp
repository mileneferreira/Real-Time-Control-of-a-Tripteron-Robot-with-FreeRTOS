// #include "peripherals/GPIO.hpp"
#include <cmath>
#include <numbers>

#include "robot/Tripteron.hpp"
#include "task/Periodic.hpp"
#include "utils/Percentage.hpp"
#include "utils/print.hpp"

template <size_t N>
constexpr auto generate_circle_path(uint16_t centerX, uint16_t centerY, uint16_t radius, uint16_t zHeight) {
  std::array<Robot::Tripteron::Position, N> path{ { 0, 0, 0 } };

  for (size_t i = 0; i < N; ++i) {
    const float angle = (2.0f * std::numbers::pi * i) / N;
    path[i] = {
      static_cast<uint16_t>(centerX + radius * std::cos(angle)),
      static_cast<uint16_t>(centerY + radius * std::sin(angle)),
      zHeight
    };
  }
  return path;
}

/**
 * @brief Generates three circles, one in each primary plane (XY, YZ, ZX).
 * * Circle 1: XY Plane (Flat) at fixed Z
 * Circle 2: YZ Plane (Side) at fixed X
 * Circle 3: ZX Plane (Front) at fixed Y
 * * * @tparam RES_PER_CIRCLE Resolution of points per circle
 * @param center Center point for the "cube" space these circles inhabit
 * @param radius Radius of the circles
 * * @return A std::array containing the full path for all 3 circles.
 */
template <size_t RES_PER_CIRCLE>
constexpr auto generate_circles_for_each_plane(Robot::Tripteron::Position center, uint16_t radius) -> std::array<Robot::Tripteron::Position, (RES_PER_CIRCLE + 1) * 3> {
  // Total points: 3 circles * (resolution + 1 to close loop)
  constexpr size_t POINTS_PER_CIRCLE = RES_PER_CIRCLE + 1;
  constexpr size_t TOTAL_POINTS = POINTS_PER_CIRCLE * 3;

  std::array<Robot::Tripteron::Position, TOTAL_POINTS> fullPath;
  size_t idx = 0;

  // --- Circle 1: XY Plane (Z is fixed) ---
  // Moving in X and Y, holding Z steady.
  for (size_t i = 0; i < POINTS_PER_CIRCLE; ++i) {
    float theta = (2.0f * std::numbers::pi_v<float> * i) / RES_PER_CIRCLE;

    auto x = static_cast<uint16_t>(center.x + radius * std::cos(theta));
    auto y = static_cast<uint16_t>(center.y + radius * std::sin(theta));
    auto z = center.z;  // Fixed Z height

    fullPath[idx++] = { x, y, z };
  }

  // --- Circle 2: YZ Plane (X is fixed) ---
  // Moving in Y and Z, holding X steady.
  // Note: We might want to offset X slightly or keep it at center.x
  for (size_t i = 0; i < POINTS_PER_CIRCLE; ++i) {
    float theta = (2.0f * std::numbers::pi_v<float> * i) / RES_PER_CIRCLE;

    auto x = center.x;  // Fixed X position
    auto y = static_cast<uint16_t>(center.y + radius * std::cos(theta));
    auto z = static_cast<uint16_t>(center.z + radius * std::sin(theta));

    fullPath[idx++] = { x, y, z };
  }

  // --- Circle 3: ZX Plane (Y is fixed) ---
  // Moving in Z and X, holding Y steady.
  for (size_t i = 0; i < POINTS_PER_CIRCLE; ++i) {
    float theta = (2.0f * std::numbers::pi_v<float> * i) / RES_PER_CIRCLE;

    auto x = static_cast<uint16_t>(center.x + radius * std::sin(theta));
    auto y = center.y;  // Fixed Y position
    auto z = static_cast<uint16_t>(center.z + radius * std::cos(theta));

    fullPath[idx++] = { x, y, z };
  }

  return fullPath;
}

extern "C" void app_main(void) {
  using namespace Utils::literals;

  static Robot::Tripteron robot;
  // static constexpr auto circle = generate_circles_for_each_plane<40>({ 50_percent, 50_percent, 50_percent }, 30_percent);
  static constexpr auto circle = generate_circle_path<40>(50_percent, 50_percent, 30_percent, 20_percent);

  robot.calibrate();
  while (true) {
    const auto path = std::span{ circle.data(), circle.size() };
    robot.move(path);
    std::this_thread::sleep_for(1s);
  }
}
