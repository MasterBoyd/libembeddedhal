// The MIT License (MIT)
//
// Copyright (c) 2018 Mateusz Pusz
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

// IWYU pragma: begin_exports
#include <units/isq/dimensions/acceleration.h>
#include <units/quantity.h>
#include <units/reference.h>
#include <units/symbol_text.h>
// IWYU pragma: end_exports

#include <units/isq/si/cgs/speed.h>
#include <units/unit.h>

namespace units::isq::si::cgs {

struct gal : named_unit<gal, "Gal", si::prefix> {};
struct dim_acceleration : isq::dim_acceleration<dim_acceleration, gal, dim_length, dim_time> {};

template<UnitOf<dim_acceleration> U, Representation Rep = double>
using acceleration = quantity<dim_acceleration, U, Rep>;

#ifndef UNITS_NO_LITERALS

inline namespace literals {

// Gal
constexpr auto operator"" _q_Gal(unsigned long long l)
{
  gsl_ExpectsAudit(std::in_range<std::int64_t>(l));
  return acceleration<gal, std::int64_t>(static_cast<std::int64_t>(l));
}
constexpr auto operator"" _q_Gal(long double l) { return acceleration<gal, long double>(l); }

}  // namespace literals

#endif  // UNITS_NO_LITERALS

#ifndef UNITS_NO_REFERENCES

namespace acceleration_references {

inline constexpr auto Gal = reference<dim_acceleration, gal>{};

}  // namespace acceleration_references

namespace references {

using namespace acceleration_references;

}  // namespace references

#endif  // UNITS_NO_REFERENCES

}  // namespace units::isq::si::cgs

#ifndef UNITS_NO_ALIASES

namespace units::aliases::isq::si::cgs::inline acceleration {

template<Representation Rep = double>
using Gal = units::isq::si::cgs::acceleration<units::isq::si::cgs::gal, Rep>;

}  // namespace units::aliases::isq::si::cgs::inline acceleration

#endif  // UNITS_NO_ALIASES
