/**
 * @file util.hpp
 * @brief Provide utilities for the counter interface
 */
#pragma once

#include <chrono>

#include "../error.hpp"
#include "../timeout.hpp"
#include "interface.hpp"
#include "timeout.hpp"

namespace embed {
/**
 * @brief Create a timeout object based on embed::counter.
 *
 * NOTE: that multiple timeout objects can be made from a single counter without
 * any influence on other timeout objects.
 *
 * @param p_counter - embed::counter implementation
 * @param p_duration - amount of time until timeout
 * @return boost::leaf::result<embed::counter_timeout>
 */
inline boost::leaf::result<embed::counter_timeout> create_timeout(
  embed::counter& p_counter,
  std::chrono::nanoseconds p_duration)
{
  if (p_duration < std::chrono::nanoseconds(0)) {
    return boost::leaf::new_error(std::errc::result_out_of_range);
  }
  const auto [frequency, count] = BOOST_LEAF_CHECK(p_counter.uptime());
  auto cycles = BOOST_LEAF_CHECK(frequency.cycles_per(p_duration));
  return embed::counter_timeout(p_counter, cycles);
}

/**
 * @brief Delay execution for a duration of time using a hardware counter.
 *
 * @param p_counter - counter driver
 * @param p_duration - the amount of time to delay for, must be positive
 * @return boost::leaf::result<void> - returns any errors that result from
 * embed::counter::uptime(), otherwise returns success.
 * @throws std::errc::result_out_of_range - if the calculated cycle count
 * exceeds std::int64_t.
 */
[[nodiscard]] inline boost::leaf::result<void> delay(
  embed::counter& p_counter,
  std::chrono::nanoseconds p_duration) noexcept
{
  if (p_duration < std::chrono::nanoseconds(0)) {
    return boost::leaf::new_error(std::errc::result_out_of_range);
  }
  auto timeout_object = BOOST_LEAF_CHECK(create_timeout(p_counter, p_duration));
  return delay(timeout_object);
}
}  // namespace embed
