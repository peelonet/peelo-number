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
  number::exp(rounding_mode rounding) const
  {
    number result(*this);

    number_utils::unary_op(
      result.m_value,
      result.m_unit,
      rounding,
      mpfr_exp
    );

    return result;
  }

  number
  number::exp2(rounding_mode rounding) const
  {
    number result(*this);

    number_utils::unary_op(
      result.m_value,
      result.m_unit,
      rounding,
      mpfr_exp2
    );

    return result;
  }

  number
  number::expm1(rounding_mode rounding) const
  {
    number result(*this);

    number_utils::unary_op(
      result.m_value,
      result.m_unit,
      rounding,
      mpfr_expm1
    );

    return result;
  }

  number
  number::log(rounding_mode rounding) const
  {
    number result(*this);

    number_utils::unary_op(
      result.m_value,
      result.m_unit,
      rounding,
      mpfr_log
    );

    return result;
  }

  number
  number::log10(rounding_mode rounding) const
  {
    number result(*this);

    number_utils::unary_op(
      result.m_value,
      result.m_unit,
      rounding,
      mpfr_log10
    );

    return result;
  }

  number
  number::log2(rounding_mode rounding) const
  {
    number result(*this);

    number_utils::unary_op(
      result.m_value,
      result.m_unit,
      rounding,
      mpfr_log2
    );

    return result;
  }

  number
  number::log1p(rounding_mode rounding) const
  {
    number result(*this);

    number_utils::unary_op(
      result.m_value,
      result.m_unit,
      rounding,
      mpfr_log1p
    );

    return result;
  }

  number
  number::pow(const number& exp, rounding_mode rounding) const
  {
    number result;

    number_utils::binary_op(
      result.m_value,
      result.m_unit,
      m_value,
      m_unit,
      exp.m_value,
      exp.m_unit,
      rounding,
      mpfr_pow
    );

    return result;
  }

  number
  number::sqrt(rounding_mode rounding) const
  {
    number result;

    number_utils::unary_op(
      result.m_value,
      result.m_unit,
      rounding,
      mpfr_sqrt
    );

    return result;
  }

  number
  number::cbrt(rounding_mode rounding) const
  {
    number result(*this);

    number_utils::unary_op(
      result.m_value,
      result.m_unit,
      rounding,
      mpfr_cbrt
    );

    return result;
  }

  number
  number::hypot(const number& that, rounding_mode rounding) const
  {
    number result(*this);

    number_utils::binary_op(
      result.m_value,
      result.m_unit,
      m_value,
      m_unit,
      that.m_value,
      that.m_unit,
      rounding,
      mpfr_hypot
    );

    return result;
  }

  number
  number::cos(rounding_mode rounding) const
  {
    number result(*this);

    number_utils::unary_op(
      result.m_value,
      result.m_unit,
      rounding,
      mpfr_cos
    );

    return result;
  }

  number
  number::sin(rounding_mode rounding) const
  {
    number result(*this);

    number_utils::unary_op(
      result.m_value,
      result.m_unit,
      rounding,
      mpfr_sin
    );

    return result;
  }

  number
  number::tan(rounding_mode rounding) const
  {
    number result(*this);

    number_utils::unary_op(
      result.m_value,
      result.m_unit,
      rounding,
      mpfr_tan
    );

    return result;
  }

  number
  number::acos(rounding_mode rounding) const
  {
    number result(*this);

    number_utils::unary_op(
      result.m_value,
      result.m_unit,
      rounding,
      mpfr_acos
    );

    return result;
  }

  number
  number::asin(rounding_mode rounding) const
  {
    number result(*this);

    number_utils::unary_op(
      result.m_value,
      result.m_unit,
      rounding,
      mpfr_asin
    );

    return result;
  }

  number
  number::atan(rounding_mode rounding) const
  {
    number result(*this);

    number_utils::unary_op(
      result.m_value,
      result.m_unit,
      rounding,
      mpfr_atan
    );

    return result;
  }

  number
  number::atan2(const number& that, rounding_mode rounding) const
  {
    number result(*this);

    number_utils::binary_op(
      result.m_value,
      result.m_unit,
      m_value,
      m_unit,
      that.m_value,
      that.m_unit,
      rounding,
      mpfr_atan2
    );

    return result;
  }

  number
  number::sinh(rounding_mode rounding) const
  {
    number result(*this);

    number_utils::unary_op(
      result.m_value,
      result.m_unit,
      rounding,
      mpfr_sinh
    );

    return result;
  }

  number
  number::cosh(rounding_mode rounding) const
  {
    number result(*this);

    number_utils::unary_op(
      result.m_value,
      result.m_unit,
      rounding,
      mpfr_cosh
    );

    return result;
  }

  number
  number::tanh(rounding_mode rounding) const
  {
    number result(*this);

    number_utils::unary_op(
      result.m_value,
      result.m_unit,
      rounding,
      mpfr_tanh
    );

    return result;
  }

  number
  number::asinh(rounding_mode rounding) const
  {
    number result(*this);

    number_utils::unary_op(
      result.m_value,
      result.m_unit,
      rounding,
      mpfr_asinh
    );

    return result;
  }

  number
  number::acosh(rounding_mode rounding) const
  {
    number result(*this);

    number_utils::unary_op(
      result.m_value,
      result.m_unit,
      rounding,
      mpfr_acosh
    );

    return result;
  }

  number
  number::atanh(rounding_mode rounding) const
  {
    number result(*this);

    number_utils::unary_op(
      result.m_value,
      result.m_unit,
      rounding,
      mpfr_atanh
    );

    return result;
  }
}
