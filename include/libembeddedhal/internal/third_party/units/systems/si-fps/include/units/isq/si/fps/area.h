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
#include <units/isq/dimensions/area.h>
#include <units/quantity.h>
#include <units/reference.h>
#include <units/symbol_text.h>
// IWYU pragma: end_exports

#include <units/isq/si/fps/length.h>
#include <units/unit.h>

namespace units::isq::si::fps {

struct square_foot : unit<square_foot> {};
struct dim_area : isq::dim_area<dim_area, square_foot, dim_length> {};


template<UnitOf<dim_area> U, Representation Rep = double>
using area = quantity<dim_area, U, Rep>;

#ifndef UNITS_NO_LITERALS

inline namespace literals {

// ft2
constexpr auto operator"" _q_ft2(unsigned long long l)
{
  gsl_ExpectsAudit(std::in_range<std::int64_t>(l));
  return area<square_foot, std::int64_t>(static_cast<std::int64_t>(l));
}
constexpr auto operator"" _q_ft2(long double l) { return area<square_foot, long double>(l); }

}  // namespace literals

#endif  // UNITS_NO_LITERALS

#ifndef UNITS_NO_REFERENCES

namespace area_references {

inline constexpr auto ft2 = reference<dim_area, square_foot>{};

}  // namespace area_references

namespace references {

using namespace area_references;

}  // namespace references

#endif  // UNITS_NO_REFERENCES

}  // namespace units::isq::si::fps

#ifndef UNITS_NO_ALIASES

namespace units::aliases::isq::si::fps::inline area {

template<Representation Rep = double>
using ft2 = units::isq::si::fps::area<units::isq::si::fps::square_foot, Rep>;

}  // namespace units::aliases::isq::si::fps::inline area

#endif  // UNITS_NO_ALIASES
