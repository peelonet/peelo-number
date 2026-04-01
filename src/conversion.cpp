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
#include <stdexcept>

#include "peelo/number.hpp"

#include "./utils.hpp"

namespace peelo
{
  static void
  size_check()
  {
    if (mpfr_underflow_p())
    {
      mpfr_clear_underflow();

      throw std::underflow_error("Numeric value too small.");
    }
    else if (mpfr_overflow_p())
    {
      mpfr_clear_overflow();

      throw std::overflow_error("Numeric value too large.");
    }
  }

  number
  number::without_measurement_unit() const
  {
    number result(*this);

    result.m_unit.reset();

    return result;
  }

  number::operator double() const
  {
    double result;

    if (m_unit)
    {
      value_type a;

      number_utils::to_base_unit(a, m_value, m_unit, default_rounding_mode);
      result = mpfr_get_d(a, default_rounding_mode);
      mpfr_clear(a);
    } else {
      result = mpfr_get_d(m_value, default_rounding_mode);
    }
    size_check();

    return result;
  }

  number::operator long() const
  {
    long result;

    if (m_unit)
    {
      value_type a;

      number_utils::to_base_unit(a, m_value, m_unit, default_rounding_mode);
      result = mpfr_get_si(a, default_rounding_mode);
      mpfr_clear(a);
    } else {
      result = mpfr_get_si(m_value, default_rounding_mode);
    }
    size_check();

    return result;
  }
}
