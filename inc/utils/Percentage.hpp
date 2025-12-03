#pragma once

#include <cmath>
#include <cstdint>

namespace Utils {
  struct Percentage {
    uint16_t value;

    constexpr Percentage(double percentage) : value(std::round(percentage * 100.0)) {}
    constexpr operator uint16_t() const { return value; }
  };

  namespace literals {
    constexpr auto operator""_percent(long double val) -> Percentage { return Percentage{ static_cast<double>(val) }; }
    constexpr auto operator""_percent(unsigned long long val) -> Percentage { return Percentage{ static_cast<double>(val) }; }
  }  // namespace literals
}  // namespace Utils
