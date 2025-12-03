#pragma once
#include <mutex>
#include <span>

#include "driver/rmt_tx.h"
#include "esp_err.h"
#include "hal/rmt_types.h"
#include "soc/clk_tree_defs.h"
#include "utils/Frequency.hpp"

namespace Peripherals {
  namespace {
    using namespace Utils::literals;
  }  // namespace

  /**
   * @brief Wrapper for the ESP32 RMT (Remote Control) peripheral.
   *
   * @tparam pin The GPIO pin to output signals on.
   * @tparam channel The RMT channel (0-7) to use.
   * @tparam mem_block_num Number of 64x32-bit memory blocks to use (default 1).
   */
  template <uint8_t pin, Utils::Frequency freq = 1_MHz, size_t mem_blocks = 64>
  class RMT {
    static_assert(mem_blocks % 2 == 0, "mem_blocks must be even!");
    static_assert(mem_blocks >= 64, "mem_blocks must be at least 64!");

   private:
    rmt_channel_handle_t channel = NULL;
    rmt_encoder_handle_t encoder = NULL;

    rmt_transmit_config_t tx_config = {};

   public:
    RMT() {
      rmt_tx_channel_config_t config = {
        .gpio_num = static_cast<gpio_num_t>(pin),
        .clk_src = RMT_CLK_SRC_DEFAULT,
        .resolution_hz = freq,
        .mem_block_symbols = mem_blocks,
        .trans_queue_depth = 10,
      };

      // Configure and Install Driver
      ESP_ERROR_CHECK(rmt_new_tx_channel(&config, &channel));

      rmt_copy_encoder_config_t encoder_config = {};
      rmt_new_copy_encoder(&encoder_config, &encoder);

      rmt_enable(channel);

      tx_config.loop_count = 0;
    }

    /**
     * @brief Send a raw sequence of RMT items.
     *
     * @param items Reference to a vector containing the pulse data.
     * @param wait_until_done If true, blocks until transmission is complete.
     *
     * @note The 'items' vector must remain valid (in scope) until transmission finishes
     * if you pass wait_until_done = false.
     */
    auto transmit(const std::span<rmt_symbol_word_t>& items, bool sync = false) -> void {
      if (items.empty())
        return;

      rmt_transmit(channel, encoder, items.data(), items.size() * sizeof(rmt_symbol_word_t), &tx_config);
      if (sync)
        join();
    }

    /**
     * @brief Wait for the current transmission to finish.
     */
    auto join() -> void { rmt_tx_wait_all_done(channel, -1); }

    /**
     * @brief Stop transmission and clear internal buffers.
     */
    auto stop() -> void {
      rmt_disable(channel);
      rmt_enable(channel);
    }

    ~RMT() {
      if (channel)
        rmt_disable(channel);

      if (encoder)
        rmt_del_encoder(encoder);

      if (channel)
        rmt_del_channel(channel);
    }
  };
}  // namespace Peripherals
