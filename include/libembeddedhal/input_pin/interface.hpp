#pragma once

#include "../error.hpp"
#include "pin_resistors.hpp"

namespace embed {
/**
 * @brief Digital input pin hardware abstraction.
 *
 * Use this to read a pin and determine if the voltage on it is HIGH or LOW.
 *
 */
class input_pin
{
public:
  /// Generic settings for input pins
  struct settings
  {
    /// pull resistor for an input pin
    pin_resistor resistor = pin_resistor::pull_up;
  };

  /**
   * @brief Configure the input pin to match the settings supplied
   *
   * @param p_settings - settings to apply to input pin
   * @return boost::leaf::result<void> - any error that occurred during this
   * operation. Will return embed::error::invalid_settings if the settings could
   * not be achieved.
   */
  [[nodiscard]] boost::leaf::result<void> configure(
    const settings& p_settings) noexcept
  {
    return driver_configure(p_settings);
  }

  /**
   * @brief Read the state of the input pin
   *
   * @return boost::leaf::result<bool> - true indicates HIGH voltage and false
   * indicates LOW voltage
   */
  [[nodiscard]] boost::leaf::result<bool> level() noexcept
  {
    return driver_level();
  }

private:
  virtual boost::leaf::result<void> driver_configure(
    const settings& p_settings) noexcept = 0;
  virtual boost::leaf::result<bool> driver_level() noexcept = 0;
};
}  // namespace embed
