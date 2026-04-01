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
  bool
  number::equals(const number& that, rounding_mode rounding) const noexcept
  {
    int result;

    if (m_unit)
    {
      value_type a;
      value_type b;

      if (!that.m_unit || m_unit->type != that.m_unit->type)
      {
        return false;
      }
      number_utils::to_base_unit(a, m_value, m_unit, rounding);
      number_utils::to_base_unit(b, that.m_value, that.m_unit, rounding);
      result = mpfr_cmp(a, b);
      mpfr_clear(a);
      mpfr_clear(b);
    }
    else if (that.m_unit)
    {
      return false;
    } else {
      result = mpfr_cmp(m_value, that.m_value);
    }

    return result == 0;
  }

  bool
  number::equals(
    double value,
    const unit_type& unit,
    rounding_mode rounding
  ) const noexcept
  {
    int result;

    if (m_unit)
    {
      value_type a;
      double b;

      if (!unit || m_unit->type != unit->type)
      {
        return false;
      }
      number_utils::to_base_unit(a, m_value, m_unit, rounding);
      number_utils::to_base_unit(b, value, unit);
      result = mpfr_cmp_d(a, b);
      mpfr_clear(a);
    }
    else if (unit)
    {
      return false;
    } else {
      result = mpfr_cmp_d(m_value, value);
    }

    return result == 0;
  }

  int
  number::compare(const number& that, rounding_mode rounding) const
  {
    int result;

    number_utils::unit_check(m_unit, that.m_unit);
    if (m_unit)
    {
      value_type a;
      value_type b;

      number_utils::to_base_unit(a, m_value, m_unit, rounding);
      number_utils::to_base_unit(b, that.m_value, that.m_unit, rounding);
      result = mpfr_cmp(a, b);
      mpfr_clear(a);
      mpfr_clear(b);
    } else {
      result = mpfr_cmp(m_value, that.m_value);
    }

    return result;
  }

  int
  number::compare(
    double value,
    const unit_type& unit,
    rounding_mode rounding
  ) const
  {
    int result;

    number_utils::unit_check(m_unit, unit);
    if (m_unit)
    {
      value_type a;
      double b;

      number_utils::to_base_unit(a, m_value, m_unit, rounding);
      number_utils::to_base_unit(b, value, unit);
      result = mpfr_cmp_d(a, b);
      mpfr_clear(a);
    } else {
      result = mpfr_cmp_d(m_value, value);
    }

    return result;
  }
}
