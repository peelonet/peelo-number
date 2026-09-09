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
#include "peelo/number.hpp"

#include "./storage_api.hpp"
#include "./utils.hpp"

namespace peelo
{
  using directed_rounding_op_callback = int(*)(mpfr_t, mpfr_srcptr);

  static void
  normalize_unit_after_rounding(
    number::value_type value,
    number::unit_type& unit,
    number::rounding_mode rounding
  )
  {
    if (unit)
    {
      number_utils::normalize_unit(
        value,
        unit,
        number::unit::base_unit_of(unit->type),
        rounding
      );
    }
  }

  static void
  directed_rounding_to_decimal_places(
    number::value_type value,
    number::unit_type& unit,
    int decimal_places,
    number::rounding_mode rounding,
    directed_rounding_op_callback callback
  )
  {
    if (decimal_places == 0)
    {
      callback(value, value);
      normalize_unit_after_rounding(value, unit, rounding);
      return;
    }

    mpfr_t scale;
    mpfr_init(scale);
    mpfr_set_ui(scale, 10, rounding);
    mpfr_pow_si(scale, scale, decimal_places, rounding);

    mpfr_mul(value, value, scale, rounding);
    callback(value, value);
    mpfr_div(value, value, scale, rounding);

    mpfr_clear(scale);
    normalize_unit_after_rounding(value, unit, rounding);
  }

  static void
  round_to_decimal_places(
    number::value_type value,
    number::unit_type& unit,
    int decimal_places,
    number::rounding_mode rounding
  )
  {
    if (decimal_places == 0)
    {
      mpfr_rint(value, value, rounding);
      normalize_unit_after_rounding(value, unit, rounding);
      return;
    }

    mpfr_t scale;
    mpfr_init(scale);
    mpfr_set_ui(scale, 10, rounding);
    mpfr_pow_si(scale, scale, decimal_places, rounding);

    mpfr_mul(value, value, scale, rounding);
    mpfr_rint(value, value, rounding);
    mpfr_div(value, value, scale, rounding);

    mpfr_clear(scale);
    normalize_unit_after_rounding(value, unit, rounding);
  }

  number
  number::ceil(
    int decimal_places,
    rounding_mode rounding
  ) const
  {
    number result(*this);

    internal::promote_to_mpfr(result, rounding);
    directed_rounding_to_decimal_places(
      internal::mpfr_mut(result),
      result.m_unit,
      decimal_places,
      rounding,
      mpfr_ceil
    );

    return result;
  }

  number
  number::floor(
    int decimal_places,
    rounding_mode rounding
  ) const
  {
    number result(*this);

    internal::promote_to_mpfr(result, rounding);
    directed_rounding_to_decimal_places(
      internal::mpfr_mut(result),
      result.m_unit,
      decimal_places,
      rounding,
      mpfr_floor
    );

    return result;
  }

  number
  number::round(
    int decimal_places,
    rounding_mode rounding
  ) const
  {
    number result(*this);

    internal::promote_to_mpfr(result, rounding);
    round_to_decimal_places(
      internal::mpfr_mut(result),
      result.m_unit,
      decimal_places,
      rounding
    );

    return result;
  }
}
