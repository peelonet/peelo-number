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

namespace peelo
{
  number
  number::inf(const unit_type& unit)
  {
    number result;

    mpfr_set_inf(result.m_value, 0);
    result.m_unit = unit;

    return result;
  }

  number
  number::nan(const unit_type& unit)
  {
    number result;

    mpfr_set_nan(result.m_value);
    result.m_unit = unit;

    return result;
  }

  number::number(const unit_type& unit)
    : m_unit(unit)
  {
    mpfr_init_set_si(m_value, 0, default_rounding_mode);
  }

  number::number(const number& that)
    : m_unit(that.m_unit)
  {
    mpfr_init_set(m_value, that.m_value, default_rounding_mode);
  }

  number::number(double value, const unit_type& unit, rounding_mode rounding)
    : m_unit(unit)
  {
    mpfr_init_set_d(m_value, value, rounding);
  }

  number::~number()
  {
    mpfr_clear(m_value);
  }
}
