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
  bool
  number::equals(const number& that, rounding_mode rounding) const noexcept
  {
    if (m_unit)
    {
      if (!that.m_unit || m_unit->type != that.m_unit->type)
      {
        return false;
      }

      value_type a;
      value_type b;
      number lhs(*this);
      number rhs(that);

      internal::promote_to_mpfr(lhs, rounding);
      internal::promote_to_mpfr(rhs, rounding);
      number_utils::to_base_unit(
        a,
        internal::mpfr_value(lhs),
        m_unit,
        rounding
      );
      number_utils::to_base_unit(
        b,
        internal::mpfr_value(rhs),
        that.m_unit,
        rounding
      );
      const int result = mpfr_cmp(a, b);

      mpfr_clear(a);
      mpfr_clear(b);

      return result == 0;
    }

    if (that.m_unit)
    {
      return false;
    }

    if (internal::is_small(*this) && internal::is_small(that))
    {
      return m_small == that.m_small;
    }

    number lhs(*this);
    number rhs(that);

    internal::promote_to_mpfr(lhs, rounding);
    internal::promote_to_mpfr(rhs, rounding);

    return mpfr_cmp(internal::mpfr_value(lhs), internal::mpfr_value(rhs)) == 0;
  }

  bool
  number::equals(
    double value,
    const unit_type& unit,
    rounding_mode rounding
  ) const noexcept
  {
    if (m_unit)
    {
      if (!unit || m_unit->type != unit->type)
      {
        return false;
      }

      value_type a;
      double b;
      number lhs(*this);

      internal::promote_to_mpfr(lhs, rounding);
      number_utils::to_base_unit(
        a,
        internal::mpfr_value(lhs),
        m_unit,
        rounding
      );
      number_utils::to_base_unit(b, value, unit);
      const int result = mpfr_cmp_d(a, b);

      mpfr_clear(a);

      return result == 0;
    }

    if (unit)
    {
      return false;
    }

    if (internal::is_small(*this))
    {
      return static_cast<double>(m_small) == value;
    }

    return mpfr_cmp_d(internal::mpfr_value(*this), value) == 0;
  }

  int
  number::compare(const number& that, rounding_mode rounding) const
  {
    number_utils::unit_check(m_unit, that.m_unit);

    if (!m_unit && internal::is_small(*this) && internal::is_small(that))
    {
      if (m_small < that.m_small)
      {
        return -1;
      }
      if (m_small > that.m_small)
      {
        return 1;
      }

      return 0;
    }

    number lhs(*this);
    number rhs(that);

    internal::promote_to_mpfr(lhs, rounding);
    internal::promote_to_mpfr(rhs, rounding);

    if (m_unit)
    {
      value_type a;
      value_type b;

      number_utils::to_base_unit(
        a,
        internal::mpfr_value(lhs),
        m_unit,
        rounding
      );
      number_utils::to_base_unit(
        b,
        internal::mpfr_value(rhs),
        that.m_unit,
        rounding
      );
      const int result = mpfr_cmp(a, b);

      mpfr_clear(a);
      mpfr_clear(b);

      return result;
    }

    return mpfr_cmp(internal::mpfr_value(lhs), internal::mpfr_value(rhs));
  }

  int
  number::compare(
    double value,
    const unit_type& unit,
    rounding_mode rounding
  ) const
  {
    number_utils::unit_check(m_unit, unit);

    if (!m_unit && internal::is_small(*this) && !unit)
    {
      const double lhs = static_cast<double>(m_small);

      if (lhs < value)
      {
        return -1;
      }
      if (lhs > value)
      {
        return 1;
      }

      return 0;
    }

    if (m_unit)
    {
      value_type a;
      double b;
      number lhs(*this);

      internal::promote_to_mpfr(lhs, rounding);
      number_utils::to_base_unit(
        a,
        internal::mpfr_value(lhs),
        m_unit,
        rounding
      );
      number_utils::to_base_unit(b, value, unit);
      const int result = mpfr_cmp_d(a, b);

      mpfr_clear(a);

      return result;
    }

    number copy(*this);

    internal::promote_to_mpfr(copy, rounding);

    return mpfr_cmp_d(internal::mpfr_value(copy), value);
  }
}
