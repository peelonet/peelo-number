/*
 * Copyright (c) 2026, peelo.net
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#include "./utils.hpp"

namespace peelo::number_utils
{
  void
  to_base_unit(
    number::value_type result,
    const number::value_type source,
    const number::unit_type& unit,
    number::rounding_mode rounding
  )
  {
    if (unit && !unit->is_base_unit())
    {
      if (unit->multiplier > 0)
      {
        mpfr_init(result);
        mpfr_mul_si(result, source, unit->multiplier, rounding);
        return;
      }
      else if (unit->multiplier < 0)
      {
        mpfr_init(result);
        mpfr_div_si(result, source, -unit->multiplier, rounding);
        return;
      }
    }
    mpfr_init_set(result, source, rounding);
  }

  void
  to_base_unit(
    double& result,
    const double source,
    const number::unit_type& unit
  )
  {
    if (unit && !unit->is_base_unit())
    {
      if (unit->multiplier > 0)
      {
        result = source * unit->multiplier;
        return;
      }
      else if (unit->multiplier < 0)
      {
        result = source * -unit->multiplier;
        return;
      }
    }
    result = source;
  }

  void
  unit_check(const number::unit_type& a, const number::unit_type& b)
  {
    if (a && b)
    {
      if (a->type != b->type)
      {
        throw number::unit_error(
          "Cannot compare " +
          to_string(a->type) +
          " against " +
          to_string(b->type) +
          "."
        );
      }
    }
    else if (b)
    {
      throw number::unit_error(
        "Cannot compare number without an unit against number which has " +
        to_string(b->type) +
        " as measurement unit."
      );
    }
  }

  void
  normalize_unit(
    number::value_type value,
    number::unit_type& result,
    const number::unit& base_unit,
    number::rounding_mode rounding
  )
  {
    for (const auto& unit : number::unit::all_units_of(base_unit.type))
    {
      if (unit.multiplier > 0 && mpfr_cmp_si(value, unit.multiplier) >= 0)
      {
        mpfr_div_si(value, value, unit.multiplier, rounding);
        result = unit;
        return;
      }
    }
    if (!result)
    {
      result = base_unit;
    }
  }

  void
  unary_op(
    number::value_type value,
    number::unit_type& unit,
    number::rounding_mode rounding,
    unary_op_callback callback
  )
  {
    callback(value, value, rounding);
    if (unit)
    {
      normalize_unit(
        value,
        unit,
        number::unit::base_unit_of(unit->type),
        rounding
      );
    }
  }

  void
  binary_op(
    number::value_type value,
    number::unit_type& unit,
    const number::value_type a_value,
    const number::unit_type& a_unit,
    const number::value_type b_value,
    const number::unit_type& b_unit,
    number::rounding_mode rounding,
    binary_op_callback callback
  )
  {
    number_utils::unit_check(a_unit, b_unit);
    if (a_unit)
    {
      number::value_type a_base_value;
      number::value_type b_base_value;

      to_base_unit(a_base_value, a_value, a_unit, rounding);
      to_base_unit(b_base_value, b_value, b_unit, rounding);
      callback(value, a_base_value, b_base_value, rounding);
      mpfr_clear(a_base_value);
      mpfr_clear(b_base_value);
      normalize_unit(
        value,
        unit,
        number::unit::base_unit_of(a_unit->type),
        rounding
      );
    } else {
      callback(value, a_value, b_value, rounding);
    }
  }

  void
  binary_op(
    number::value_type value,
    number::unit_type& unit,
    const number::value_type a_value,
    const number::unit_type& a_unit,
    const double b_value,
    const number::unit_type& b_unit,
    number::rounding_mode rounding,
    decimal_binary_op_callback callback
  )
  {
    number_utils::unit_check(a_unit, b_unit);
    if (a_unit)
    {
      number::value_type a_base_value;
      double b_base_value;

      to_base_unit(a_base_value, a_value, a_unit, rounding);
      to_base_unit(b_base_value, b_value, b_unit);
      callback(value, a_base_value, b_base_value, rounding);
      mpfr_clear(a_base_value);
      normalize_unit(
        value,
        unit,
        number::unit::base_unit_of(a_unit->type),
        rounding
      );
    } else {
      callback(value, a_value, b_value, rounding);
    }
  }
}
