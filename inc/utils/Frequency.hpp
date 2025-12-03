#pragma once

#include <cstdint>

namespace Utils {
  struct Frequency {
    uint32_t freq;

    constexpr Frequency(uint32_t f) : freq(f) {}
    constexpr operator uint32_t() const { return freq; }
  };

  namespace literals {
    constexpr auto operator""_Hz(unsigned long long val) -> Frequency { return Frequency{ static_cast<uint32_t>(val) }; }
    constexpr auto operator""_kHz(unsigned long long val) -> Frequency { return Frequency{ static_cast<uint32_t>(val * 1e3) }; }
    constexpr auto operator""_MHz(unsigned long long val) -> Frequency { return Frequency{ static_cast<uint32_t>(val * 1e6) }; }
  }  // namespace literals
}  // namespace Utils
