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
#include <cstdint>

#include "peelo/number.hpp"

#include "./storage_api.hpp"
#include "./utils.hpp"

namespace peelo
{
  number
  number::add(const number& that, rounding_mode rounding) const
  {
    if (
      internal::is_small(*this)
      && internal::is_small(that)
      && !m_unit
      && !that.m_unit
    )
    {
      std::int64_t result_value;

      if (internal::add_small(m_small, that.m_small, result_value))
      {
        return number(result_value);
      }
    }

    number result;
    number lhs(*this);
    number rhs(that);

    internal::promote_to_mpfr(result, rounding);
    internal::promote_to_mpfr(lhs, rounding);
    internal::promote_to_mpfr(rhs, rounding);

    number_utils::binary_op(
      internal::mpfr_mut(result),
      result.m_unit,
      internal::mpfr_value(lhs),
      lhs.m_unit,
      internal::mpfr_value(rhs),
      rhs.m_unit,
      rounding,
      mpfr_add
    );

    return result;
  }

  number
  number::add(
    double value,
    const unit_type& unit,
    rounding_mode rounding
  ) const
  {
    number result;
    number lhs(*this);

    internal::promote_to_mpfr(result, rounding);
    internal::promote_to_mpfr(lhs, rounding);

    number_utils::binary_op(
      internal::mpfr_mut(result),
      result.m_unit,
      internal::mpfr_value(lhs),
      lhs.m_unit,
      value,
      unit,
      rounding,
      mpfr_add_d
    );

    return result;
  }

  number
  number::substract(const number& that, rounding_mode rounding) const
  {
    if (
      internal::is_small(*this)
      && internal::is_small(that)
      && !m_unit
      && !that.m_unit
    )
    {
      std::int64_t result_value;

      if (internal::sub_small(m_small, that.m_small, result_value))
      {
        return number(result_value);
      }
    }

    number result;
    number lhs(*this);
    number rhs(that);

    internal::promote_to_mpfr(result, rounding);
    internal::promote_to_mpfr(lhs, rounding);
    internal::promote_to_mpfr(rhs, rounding);

    number_utils::binary_op(
      internal::mpfr_mut(result),
      result.m_unit,
      internal::mpfr_value(lhs),
      lhs.m_unit,
      internal::mpfr_value(rhs),
      rhs.m_unit,
      rounding,
      mpfr_sub
    );

    return result;
  }

  number
  number::substract(
    double value,
    const unit_type& unit,
    rounding_mode rounding
  ) const
  {
    number result;
    number lhs(*this);

    internal::promote_to_mpfr(result, rounding);
    internal::promote_to_mpfr(lhs, rounding);

    number_utils::binary_op(
      internal::mpfr_mut(result),
      result.m_unit,
      internal::mpfr_value(lhs),
      lhs.m_unit,
      value,
      unit,
      rounding,
      mpfr_sub_d
    );

    return result;
  }

  number
  number::multiply(const number& that, rounding_mode rounding) const
  {
    if (
      internal::is_small(*this)
      && internal::is_small(that)
      && !m_unit
      && !that.m_unit
    )
    {
      std::int64_t result_value;

      if (internal::mul_small(m_small, that.m_small, result_value))
      {
        return number(result_value);
      }
    }

    number result;
    number lhs(*this);
    number rhs(that);

    internal::promote_to_mpfr(result, rounding);
    internal::promote_to_mpfr(lhs, rounding);
    internal::promote_to_mpfr(rhs, rounding);

    number_utils::multiply_op(
      internal::mpfr_mut(result),
      result.m_unit,
      internal::mpfr_value(lhs),
      lhs.m_unit,
      internal::mpfr_value(rhs),
      rhs.m_unit,
      rounding
    );

    return result;
  }

  number
  number::multiply(
    double value,
    const unit_type& unit,
    rounding_mode rounding
  ) const
  {
    number result;
    number lhs(*this);

    internal::promote_to_mpfr(result, rounding);
    internal::promote_to_mpfr(lhs, rounding);

    number_utils::multiply_op(
      internal::mpfr_mut(result),
      result.m_unit,
      internal::mpfr_value(lhs),
      lhs.m_unit,
      value,
      unit,
      rounding
    );

    return result;
  }

  number
  number::divide(const number& that, rounding_mode rounding) const
  {
    number result;
    number lhs(*this);
    number rhs(that);

    internal::promote_to_mpfr(result, rounding);
    internal::promote_to_mpfr(lhs, rounding);
    internal::promote_to_mpfr(rhs, rounding);

    number_utils::divide_op(
      internal::mpfr_mut(result),
      result.m_unit,
      internal::mpfr_value(lhs),
      lhs.m_unit,
      internal::mpfr_value(rhs),
      rhs.m_unit,
      rounding
    );

    return result;
  }

  number
  number::divide(
    double value,
    const unit_type& unit,
    rounding_mode rounding
  ) const
  {
    number result;
    number lhs(*this);

    internal::promote_to_mpfr(result, rounding);
    internal::promote_to_mpfr(lhs, rounding);

    number_utils::divide_op(
      internal::mpfr_mut(result),
      result.m_unit,
      internal::mpfr_value(lhs),
      lhs.m_unit,
      value,
      unit,
      rounding
    );

    return result;
  }

  number
  number::modulo(const number& that, rounding_mode rounding) const
  {
    number result;
    number lhs(*this);
    number rhs(that);

    internal::promote_to_mpfr(result, rounding);
    internal::promote_to_mpfr(lhs, rounding);
    internal::promote_to_mpfr(rhs, rounding);

    number_utils::binary_op(
      internal::mpfr_mut(result),
      result.m_unit,
      internal::mpfr_value(lhs),
      lhs.m_unit,
      internal::mpfr_value(rhs),
      rhs.m_unit,
      rounding,
      mpfr_fmod
    );

    return result;
  }

  number
  number::negate(rounding_mode rounding) const
  {
    if (internal::is_small(*this) && !m_unit)
    {
      if (m_small != INT64_MIN)
      {
        return number(-m_small);
      }
    }

    number result(*this);

    internal::promote_to_mpfr(result, rounding);
    mpfr_neg(internal::mpfr_mut(result), internal::mpfr_value(result), rounding);

    return result;
  }

  number&
  number::operator++()
  {
    if (internal::is_small(*this) && !m_unit)
    {
      std::int64_t result_value;

      if (internal::add_small(m_small, 1, result_value))
      {
        m_small = result_value;

        return *this;
      }

      internal::promote_to_mpfr(*this);
    }

    auto& value = internal::mpfr_mut(*this);

    mpfr_add_si(value, value, 1, default_rounding_mode);

    return *this;
  }

  number&
  number::operator--()
  {
    if (internal::is_small(*this) && !m_unit)
    {
      std::int64_t result_value;

      if (internal::sub_small(m_small, 1, result_value))
      {
        m_small = result_value;

        return *this;
      }

      internal::promote_to_mpfr(*this);
    }

    auto& value = internal::mpfr_mut(*this);

    mpfr_sub_si(value, value, 1, default_rounding_mode);

    return *this;
  }

  number
  number::operator++(int)
  {
    number copy(*this);

    operator++();

    return copy;
  }

  number
  number::operator--(int)
  {
    number copy(*this);

    operator--();

    return copy;
  }
}
