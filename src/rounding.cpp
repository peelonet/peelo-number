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

#include "./utils.hpp"

namespace peelo
{
  using rounding_op_callback = int(*)(mpfr_t, mpfr_srcptr);

  static void
  rounding_op(
    number::value_type value,
    number::unit_type& unit,
    rounding_op_callback callback
  )
  {
    callback(value, value);
    if (unit)
    {
      number_utils::normalize_unit(
        value,
        unit,
        number::unit::base_unit_of(unit->type),
        number::default_rounding_mode
      );
    }
  }

  number
  number::ceil() const
  {
    number result(*this);

    rounding_op(result.m_value, result.m_unit, mpfr_ceil);

    return result;
  }

  number
  number::floor() const
  {
    number result(*this);

    rounding_op(result.m_value, result.m_unit, mpfr_floor);

    return result;
  }

  number
  number::round() const
  {
    number result(*this);

    rounding_op(result.m_value, result.m_unit, mpfr_round);

    return result;
  }
}
