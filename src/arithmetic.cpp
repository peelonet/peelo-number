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
  number
  number::add(const number& that, rounding_mode rounding) const
  {
    number result;

    number_utils::binary_op(
      result.m_value,
      result.m_unit,
      m_value,
      m_unit,
      that.m_value,
      that.m_unit,
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

    number_utils::binary_op(
      result.m_value,
      result.m_unit,
      m_value,
      m_unit,
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
    number result;

    number_utils::binary_op(
      result.m_value,
      result.m_unit,
      m_value,
      m_unit,
      that.m_value,
      that.m_unit,
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

    number_utils::binary_op(
      result.m_value,
      result.m_unit,
      m_value,
      m_unit,
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
    number result;

    number_utils::binary_op(
      result.m_value,
      result.m_unit,
      m_value,
      m_unit,
      that.m_value,
      that.m_unit,
      rounding,
      mpfr_mul
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

    number_utils::binary_op(
      result.m_value,
      result.m_unit,
      m_value,
      m_unit,
      value,
      unit,
      rounding,
      mpfr_mul_d
    );

    return result;
  }

  number
  number::divide(const number& that, rounding_mode rounding) const
  {
    number result;

    number_utils::binary_op(
      result.m_value,
      result.m_unit,
      m_value,
      m_unit,
      that.m_value,
      that.m_unit,
      rounding,
      mpfr_div
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

    number_utils::binary_op(
      result.m_value,
      result.m_unit,
      m_value,
      m_unit,
      value,
      unit,
      rounding,
      mpfr_div_d
    );

    return result;
  }

  number
  number::modulo(const number& that, rounding_mode rounding) const
  {
    number result;

    number_utils::binary_op(
      result.m_value,
      result.m_unit,
      m_value,
      m_unit,
      that.m_value,
      that.m_unit,
      rounding,
      mpfr_fmod
    );

    return result;
  }

  number
  number::negate(rounding_mode rounding) const
  {
    number result(*this);

    mpfr_neg(result.m_value, result.m_value, rounding);

    return result;
  }

  number&
  number::operator++()
  {
    mpfr_add_si(m_value, m_value, 1, default_rounding_mode);

    return *this;
  }

  number&
  number::operator--()
  {
    mpfr_sub_si(m_value, m_value, 1, default_rounding_mode);

    return *this;
  }

  number
  number::operator++(int)
  {
    number copy(*this);

    mpfr_add_si(m_value, m_value, 1, default_rounding_mode);

    return copy;
  }

  number
  number::operator--(int)
  {
    number copy(*this);

    mpfr_sub_si(m_value, m_value, 1, default_rounding_mode);

    return copy;
  }
}
