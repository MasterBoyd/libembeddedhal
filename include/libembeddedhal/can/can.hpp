#pragma once

#include <array>
#include <chrono>
#include <cinttypes>
#include <cstddef>
#include <functional>

#include "../driver.hpp"
#include "../frequency.hpp"

namespace embed {
/**
 * @brief Generic settings for a can peripheral
 *
 */
struct can_settings
{
  /// Bus clock rate
  frequency clock_rate = default_clock_rate;
};

/**
 * @brief Controller Area Network (CAN bus) hardware abstraction interface.
 *
 */
class can : public driver<can_settings>
{
public:
  /// Can message ID type trait
  using id_t = uint32_t;

  /// Structure of a CAN message
  struct message_t
  {
    /// ID of the message
    id_t id;
    /// The number of elements in the payload
    uint8_t length = 0;
    /// The message data
    std::array<std::byte, 8> payload{ std::byte{ 0 } };
    /// Whether or not the message is a remote request frame. If true, then
    /// length and payload are ignored.
    bool is_remote_request = false;
  };

  /// default constructor
  can() = default;
  /// Explicitly delete copy constructor to prevent slicing
  can(const can& p_other) = delete;
  /// Explicitly delete assignment operator to prevent slicing
  can& operator=(const can& p_other) = delete;
  /// Destroy the object
  virtual ~can() = default;

  /**
   * @brief Send a can message over the can bus
   *
   * @param p_message - the message to be sent
   *
   * @return boost::leaf::result<void> - any error that occurred during this
   * operation.
   */
  virtual boost::leaf::result<void> send(const message_t& p_message) = 0;

  /**
   * @brief Setup driver to execute callback when a can message is received.
   *
   * @param p_receive_handler - this handler will be called when the can device
   * receives a message. Set to nullptr to disable receive interrupts.
   * @return boost::leaf::result<void> - any error that occurred during this
   * operation.
   */
  virtual boost::leaf::result<void> attach_interrupt(
    std::function<void(const message_t& p_message)> p_receive_handler) = 0;
};
}  // namespace embed
