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
#include <cmath>
#include <cstdint>

#include "peelo/number.hpp"

#include "./storage.hpp"
#include "./storage_api.hpp"

namespace peelo
{
  number
  number::inf(const unit_type& unit)
  {
    number result;

    internal::destroy(result);
    internal::init_mpfr_inf(result, unit);

    return result;
  }

  number
  number::nan(const unit_type& unit)
  {
    number result;

    internal::destroy(result);
    internal::init_mpfr_nan(result, unit);

    return result;
  }

  number::number(const unit_type& unit)
    : m_unit(unit)
  {
    if (unit)
    {
      internal::init_mpfr_si(*this, 0, unit);
    }
    else
    {
      internal::init_small(*this, 0);
    }
  }

  number::number(const number& that)
    : m_unit(that.m_unit)
  {
    internal::copy_from(*this, that);
  }

  number::number(number&& that) noexcept
  {
    internal::move_from(*this, std::move(that));
  }

  number::number(int value, const unit_type& unit)
    : m_unit(unit)
  {
    if (unit)
    {
      internal::init_mpfr_si(*this, value, unit);
    }
    else
    {
      internal::init_small(*this, value);
    }
  }

  number::number(std::int64_t value, const unit_type& unit)
    : m_unit(unit)
  {
    if (unit)
    {
      internal::init_mpfr_s64(*this, value, unit);
    }
    else
    {
      internal::init_small(*this, value);
    }
  }

  number::number(double value, const unit_type& unit, rounding_mode rounding)
    : m_unit(unit)
  {
    if (unit)
    {
      internal::init_mpfr_d(*this, value, unit, rounding);
    }
    else if (
      std::isfinite(value)
      && value == std::trunc(value)
      && value >= static_cast<double>(INT64_MIN)
      && value <= static_cast<double>(INT64_MAX)
    )
    {
      internal::init_small(*this, static_cast<std::int64_t>(value));
    }
    else
    {
      internal::init_mpfr_d(*this, value, unit, rounding);
    }
  }

  number::~number()
  {
    internal::destroy(*this);
  }

  number&
  number::operator=(number&& that) noexcept
  {
    if (this != &that)
    {
      internal::move_from(*this, std::move(that));
    }

    return *this;
  }

  bool
  number::is_inf() const
  {
    return internal::is_mpfr(*this) && mpfr_inf_p(internal::mpfr_value(*this));
  }

  bool
  number::is_nan() const
  {
    return internal::is_mpfr(*this) && mpfr_nan_p(internal::mpfr_value(*this));
  }

  number::operator bool() const noexcept
  {
    if (internal::is_small(*this))
    {
      return m_small != 0;
    }

    return mpfr_sgn(internal::mpfr_value(*this)) != 0;
  }

  bool
  number::operator!() const noexcept
  {
    if (internal::is_small(*this))
    {
      return m_small == 0;
    }

    return mpfr_sgn(internal::mpfr_value(*this)) == 0;
  }
}
