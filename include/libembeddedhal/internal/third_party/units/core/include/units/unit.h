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

#include <units/bits/derived_symbol_text.h>
#include <units/bits/external/downcasting.h>

// IWYU pragma: begin_exports
#include <units/bits/derived_unit.h>
#include <units/bits/external/fixed_string.h>
#include <units/prefix.h>
#include <units/ratio.h>
#include <units/symbol_text.h>
// IWYU pragma: end_exports

namespace units {

/**
 * @brief A common point for a hierarchy of units
 *
 * A unit is an entity defined and adopted by convention, with which any other quantity of
 * the same kind can be compared to express the ratio of the second quantity to the first
 * one as a number.
 *
 * All units of the same dimension can be convereted between each other. To allow this all of
 * them are expressed as different ratios of the same one proprietary chosen reference unit
 * (i.e. all length units are expressed in terms of meter, all mass units are expressed in
 * terms of gram, ...)
 *
 * @tparam R a ratio of a reference unit
 * @tparam U a unit to use as a reference for this dimension
 */
template<ratio R, typename U>
  requires UnitRatio<R>
struct scaled_unit : downcast_base<scaled_unit<R, U>> {
  static constexpr ::units::ratio ratio = R;
  using reference = U;
};

template<Dimension D, auto R>
// template<Dimension D, ratio R>  // TODO: GCC crash!!!
  requires UnitRatio<R>
using downcast_unit = downcast<scaled_unit<R, typename dimension_unit<D>::reference>>;

template<Unit U1, Unit U2>
struct same_unit_reference : is_same<typename U1::reference, typename U2::reference> {};

/**
 * @brief An unnamed unit
 *
 * Defines a new unnamed (in most cases coherent) derived unit of a specific derived dimension
 * and it should be passed in this dimension's definition.
 *
 * @tparam Child inherited class type used by the downcasting facility (CRTP Idiom)
 */
template<typename Child>
struct unit : downcast_dispatch<Child, scaled_unit<ratio(1), Child>> {
  static constexpr bool is_named = false;
  using prefix_family = no_prefix;
};

/**
 * @brief A named unit
 *
 * Defines a named (in most cases coherent) unit that is then passed to a dimension definition.
 * A named unit may be used by other units defined with the prefix of the same type, unless
 * no_prefix is provided for PF template parameter (in such a case it is impossible to define
 * a prefix unit based on this one).
 *
 * @tparam Child inherited class type used by the downcasting facility (CRTP Idiom)
 * @tparam Symbol a short text representation of the unit
 * @tparam PF no_prefix or a type of prefix family
 */
template<typename Child, basic_symbol_text Symbol, PrefixFamily PF>
struct named_unit : downcast_dispatch<Child, scaled_unit<ratio(1), Child>> {
  static constexpr bool is_named = true;
  static constexpr auto symbol = Symbol;
  using prefix_family = PF;
};

/**
 * @brief A scaled unit
 *
 * Defines a new named unit that is a scaled version of another unit. Such unit can be used by
 * other units defined with the prefix of the same type, unless no_prefix is provided for PF
 * template parameter (in such a case it is impossible to define a prefix unit based on this
 * one).
 *
 * @tparam Child inherited class type used by the downcasting facility (CRTP Idiom)
 * @tparam Symbol a short text representation of the unit
 * @tparam PF no_prefix or a type of prefix family
 * @tparam R a scale to apply to U
 * @tparam U a reference unit to scale
 */
template<typename Child, basic_symbol_text Symbol, PrefixFamily PF, ratio R, Unit U>
  requires UnitRatio<R>
struct named_scaled_unit : downcast_dispatch<Child, scaled_unit<R * U::ratio, typename U::reference>> {
  static constexpr bool is_named = true;
  static constexpr auto symbol = Symbol;
  using prefix_family = PF;
};

/**
 * @brief A prefixed unit
 *
 * Defines a new unit that is a scaled version of another unit by the provided prefix. It is
 * only possible to create such a unit if the given prefix type matches the one defined in a
 * reference unit.
 *
 * @tparam Child inherited class type used by the downcasting facility (CRTP Idiom)
 * @tparam P prefix to be appied to the reference unit
 * @tparam U reference unit
 */
template<typename Child, Prefix P, Unit U>
  requires U::is_named && std::same_as<typename P::prefix_family, typename U::prefix_family>
struct prefixed_unit : downcast_dispatch<Child, scaled_unit<P::ratio * U::ratio, typename U::reference>> {
  static constexpr bool is_named = true;
  static constexpr auto symbol = P::symbol + U::symbol;
  using prefix_family = no_prefix;
};

/**
 * @brief A unit with a deduced ratio and symbol
 *
 * Defines a new unit with a deduced ratio and symbol based on the recipe from the provided
 * derived dimension. The number and order of provided units should match the recipe of the
 * derived dimension. All of the units provided should also be a named ones so it is possible
 * to create a deduced symbol text.
 *
 * @tparam Child inherited class type used by the downcasting facility (CRTP Idiom)
 * @tparam Dim a derived dimension recipe to use for deduction
 * @tparam U the unit of the first composite dimension from provided derived dimension's recipe
 * @tparam URest the units for the rest of dimensions from the recipe
 */
template<typename Child, DerivedDimension Dim, Unit U, Unit... URest>
  requires detail::same_scaled_units<typename Dim::recipe, U, URest...> &&
           (U::is_named && (URest::is_named && ... && true))
struct derived_unit : downcast_dispatch<Child, detail::derived_unit<Dim, U, URest...>> {
  static constexpr bool is_named = false;
  static constexpr auto symbol = detail::derived_symbol_text<Dim, U, URest...>();
  using prefix_family = no_prefix;
};

/**
 * @brief A named unit with a deduced ratio
 *
 * Defines a new unit with a deduced ratio and the given symbol based on the recipe from the provided
 * derived dimension. The number and order of provided units should match the recipe of the
 * derived dimension. All of the units provided should also be a named ones so it is possible
 * to create a deduced symbol text.
 *
 * @tparam Child inherited class type used by the downcasting facility (CRTP Idiom)
 * @tparam Dim a derived dimension recipe to use for deduction
 * @tparam Symbol a short text representation of the unit
 * @tparam PF no_prefix or a type of prefix family
 * @tparam U the unit of the first composite dimension from provided derived dimension's recipe
 * @tparam URest the units for the rest of dimensions from the recipe
 */
template<typename Child, DerivedDimension Dim, basic_symbol_text Symbol, PrefixFamily PF, Unit U, Unit... URest>
  requires detail::same_scaled_units<typename Dim::recipe, U, URest...>
struct named_derived_unit : downcast_dispatch<Child, detail::derived_unit<Dim, U, URest...>> {
  static constexpr bool is_named = true;
  static constexpr auto symbol = Symbol;
  using prefix_family = PF;
};

/**
 * @brief An aliased named unit
 *
 * Defines a named alias for another unit. It is useful to assign alternative names and symbols
 * to the already predefined units (i.e. "tonne" for "megagram").
 * An alias unit may be used by other units defined with the prefix of the same type, unless
 * no_prefix is provided for PF template parameter (in such a case it is impossible to define
 * a prefix unit based on this one).
 *
 * @tparam U Unit for which an alias is defined
 * @tparam Symbol a short text representation of the unit
 * @tparam PF no_prefix or a type of prefix family
 */
template<Unit U, basic_symbol_text Symbol, PrefixFamily PF>
struct alias_unit : U {
  static constexpr bool is_named = true;
  static constexpr auto symbol = Symbol;
  using prefix_family = PF;
};

/**
 * @brief A prefixed alias unit
 *
 * Defines a new unit that is an alias for a scaled version of another unit by the provided
 * prefix. It is only possible to create such a unit if the given prefix type matches the one
 * defined in a reference unit.
 *
 * @tparam U Unit for which an alias is defined
 * @tparam P prefix to be appied to the reference unit
 * @tparam AU reference alias unit
 */
// TODO gcc bug: 95015
// https://gcc.gnu.org/bugzilla/show_bug.cgi?id=95015
// template<Unit U, Prefix P, AliasUnit AU>
//   requires (!AliasUnit<U>) && std::same_as<typename P::prefix_family, typename AU::prefix_family>
template<Unit U, Prefix P, Unit AU>
  requires AU::is_named && std::same_as<typename P::prefix_family, typename AU::prefix_family>
struct prefixed_alias_unit : U {
  static constexpr bool is_named = true;
  static constexpr auto symbol = P::symbol + AU::symbol;
  using prefix_family = no_prefix;
};

/**
 * @brief Unknown unit
 *
 * Used as a coherent unit of an unknown dimension.
 */
struct unknown_coherent_unit : unit<unknown_coherent_unit> {};

}  // namespace units
