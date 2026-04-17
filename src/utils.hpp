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
#pragma once

#include "peelo/number.hpp"

namespace peelo::number_utils
{
  using unary_op_callback = int(*)(
    mpfr_t,
    mpfr_srcptr,
    mpfr_rnd_t
  );
  using binary_op_callback = int(*)(
    mpfr_t,
    mpfr_srcptr,
    mpfr_srcptr,
    mpfr_rnd_t
  );
  using decimal_binary_op_callback = int(*)(
    mpfr_t,
    mpfr_srcptr,
    double,
    mpfr_rnd_t
  );

  void
  to_base_unit(
    number::value_type result,
    const number::value_type source,
    const number::unit_type& unit,
    number::rounding_mode rounding
  );

  void
  to_base_unit(
    double& result,
    const double source,
    const number::unit_type& unit
  );

  void
  unit_check(
    const number::unit_type& a,
    const number::unit_type& b
  );

  void
  normalize_unit(
    number::value_type value,
    number::unit_type& result,
    const number::unit& base_unit,
    number::rounding_mode rounding
  );

  void
  unary_op(
    number::value_type value,
    number::unit_type& unit,
    number::rounding_mode rounding,
    unary_op_callback callback
  );

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
  );

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
  );
}
