#pragma once

#include <algorithm>
#include <chrono>
#include <cmath>
#include <thread>

#include "Config.h"
#include "Query.h"
#include "utils/print.hpp"

using namespace std::chrono_literals;

namespace Task {
  struct Periodic final : std::thread {
    template <typename FnType, typename... ArgsType>
    static constexpr auto wrap(const std::chrono::milliseconds period, FnType&& fn, ArgsType&&... args) {
      if (Query::pthread::priority() == Config::AUTOMATIC_PRIORITY) {
        // I can't just call period.count() on the log10 because C++ is a mess.
        //
        // If I don't specify it as an unsigned integer like this, the compiler
        // uses it as a signed, interpreting it as a much larger value.
        const uint32_t ms = period.count();

        // Target Points: (1ms => 24) and (1000ms => 1)
        // Slope (m) = (1 - 24) / (log10(1000) - log10(1)) = -23 / 3 ≈ -7.6667
        // Intercept (b) = 24
        constexpr double SLOPE = -23.0 / 3.0;

        // Calculate the raw priority based on log10 of the period.
        const double calculated_prio = static_cast<double>(Config::MAX_PRIORITY) + SLOPE * std::log10(static_cast<double>(ms));
        Config(true).with_priority(std::round(calculated_prio));
      }

      const auto wrapper = [=]() {
        auto next_exec = std::chrono::steady_clock::now();
        while (true) {
          std::this_thread::sleep_until(next_exec);
          next_exec += period;

          const auto start = std::chrono::steady_clock::now();
          fn(args...);

          if (const auto end = std::chrono::steady_clock::now(); end > next_exec)
            Utils::println<Utils::Colors::RED>("{} missed its deadline", Query::this_thread::name());
          else
            Utils::println<Utils::Colors::GREEN>("{} took {} µs", Query::this_thread::name(), std::chrono::duration_cast<std::chrono::microseconds>(end - start).count());
        }
      };

      return wrapper;
    }

    template <typename FnType, typename... ArgsType>
    Periodic(const std::chrono::milliseconds period, FnType&& fn, ArgsType&&... args) : std::thread(wrap(period, std::forward<FnType>(fn), std::forward<ArgsType>(args)...)) {
    }

    ~Periodic() {
      if (joinable())
        join();
    }
  };
}  // namespace Task
