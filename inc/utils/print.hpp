#pragma once

#include <format>
#include <print>

namespace Utils {
  enum class Colors {
    RED,
    GREEN,
    YELLOW,
    BLUE,
    MAGENTA,
    CYAN,
    WHITE,
    DEFAULT,
  };  // namespace Colors

  template <Colors color, typename... ArgsType>
  inline auto print(std::format_string<ArgsType...> fmt, ArgsType&&... args) -> void {
    if constexpr (color == Colors::RED) {
      std::print("\x1B[31m");
      std::print(std::forward<std::format_string<ArgsType...>>(fmt), std::forward<ArgsType>(args)...);
      std::print("\x1B[39m");
    } else if constexpr (color == Colors::GREEN) {
      std::print("\x1B[32m");
      std::print(std::forward<std::format_string<ArgsType...>>(fmt), std::forward<ArgsType>(args)...);
      std::print("\x1B[39m");
    } else if constexpr (color == Colors::YELLOW) {
      std::print("\x1B[33m");
      std::print(std::forward<std::format_string<ArgsType...>>(fmt), std::forward<ArgsType>(args)...);
      std::print("\x1B[39m");
    } else if constexpr (color == Colors::BLUE) {
      std::print("\x1B[34m");
      std::print(std::forward<std::format_string<ArgsType...>>(fmt), std::forward<ArgsType>(args)...);
      std::print("\x1B[39m");
    } else if constexpr (color == Colors::MAGENTA) {
      std::print("\x1B[35m");
      std::print(std::forward<std::format_string<ArgsType...>>(fmt), std::forward<ArgsType>(args)...);
      std::print("\x1B[39m");
    } else if constexpr (color == Colors::CYAN) {
      std::print("\x1B[36m");
      std::print(std::forward<std::format_string<ArgsType...>>(fmt), std::forward<ArgsType>(args)...);
      std::print("\x1B[39m");
    } else if constexpr (color == Colors::WHITE) {
      std::print("\x1B[37m");
      std::print(std::forward<std::format_string<ArgsType...>>(fmt), std::forward<ArgsType>(args)...);
      std::print("\x1B[39m");
    } else if constexpr (color == Colors::DEFAULT) {
      std::print(std::forward<std::format_string<ArgsType...>>(fmt), std::forward<ArgsType>(args)...);
    }
  }

  template <Colors color, typename... ArgsType>
  inline auto println(std::format_string<ArgsType...> fmt, ArgsType&&... args) -> void {
    if constexpr (color == Colors::RED) {
      std::print("\x1B[31m");
      std::print(std::forward<std::format_string<ArgsType...>>(fmt), std::forward<ArgsType>(args)...);
      std::println("\x1B[39m");
    } else if constexpr (color == Colors::GREEN) {
      std::print("\x1B[32m");
      std::print(std::forward<std::format_string<ArgsType...>>(fmt), std::forward<ArgsType>(args)...);
      std::println("\x1B[39m");
    } else if constexpr (color == Colors::YELLOW) {
      std::print("\x1B[33m");
      std::print(std::forward<std::format_string<ArgsType...>>(fmt), std::forward<ArgsType>(args)...);
      std::println("\x1B[39m");
    } else if constexpr (color == Colors::BLUE) {
      std::print("\x1B[34m");
      std::print(std::forward<std::format_string<ArgsType...>>(fmt), std::forward<ArgsType>(args)...);
      std::println("\x1B[39m");
    } else if constexpr (color == Colors::MAGENTA) {
      std::print("\x1B[35m");
      std::print(std::forward<std::format_string<ArgsType...>>(fmt), std::forward<ArgsType>(args)...);
      std::println("\x1B[39m");
    } else if constexpr (color == Colors::CYAN) {
      std::print("\x1B[36m");
      std::print(std::forward<std::format_string<ArgsType...>>(fmt), std::forward<ArgsType>(args)...);
      std::println("\x1B[39m");
    } else if constexpr (color == Colors::WHITE) {
      std::print("\x1B[37m");
      std::print(std::forward<std::format_string<ArgsType...>>(fmt), std::forward<ArgsType>(args)...);
      std::println("\x1B[39m");
    } else if constexpr (color == Colors::DEFAULT) {
      std::println(std::forward<std::format_string<ArgsType...>>(fmt), std::forward<ArgsType>(args)...);
    }
  }

}  // namespace Utils
