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
#include "./storage_api.hpp"

#include <cassert>
#include <climits>
#include <cstdint>

#include "./storage.hpp"

namespace peelo::internal
{
  mpfr_holder::mpfr_holder()
  {
    mpfr_init2(value, mpfr_get_default_prec());
  }

  mpfr_holder::mpfr_holder(const mpfr_holder& that)
  {
    mpfr_init2(value, mpfr_get_prec(that.value));
    mpfr_set(value, that.value, number::default_rounding_mode);
  }

  mpfr_holder&
  mpfr_holder::operator=(const mpfr_holder& that)
  {
    if (this != &that)
    {
      mpfr_set(value, that.value, number::default_rounding_mode);
    }

    return *this;
  }

  mpfr_holder::~mpfr_holder()
  {
    mpfr_clear(value);
  }

  void
  number_access::init_small(number& n, std::int64_t value)
  {
    n.m_storage = storage_kind::small;
    n.m_small = value;
    n.m_mpfr.reset();
    n.m_unit.reset();
  }

  void
  number_access::init_mpfr_si(
    number& n,
    long value,
    const number::unit_type& unit
  )
  {
    n.m_storage = storage_kind::mpfr;
    n.m_small = 0;
    n.m_mpfr = std::make_unique<mpfr_holder>();
    mpfr_set_si(n.m_mpfr->value, value, number::default_rounding_mode);
    n.m_unit = unit;
  }

  void
  number_access::init_mpfr_s64(
    number& n,
    std::int64_t value,
    const number::unit_type& unit
  )
  {
    n.m_storage = storage_kind::mpfr;
    n.m_small = 0;
    n.m_mpfr = std::make_unique<mpfr_holder>();
    mpfr_set_sj(n.m_mpfr->value, value, number::default_rounding_mode);
    n.m_unit = unit;
  }

  void
  number_access::init_mpfr_d(
    number& n,
    double value,
    const number::unit_type& unit,
    mpfr_rnd_t rnd
  )
  {
    n.m_storage = storage_kind::mpfr;
    n.m_small = 0;
    n.m_mpfr = std::make_unique<mpfr_holder>();
    mpfr_set_d(n.m_mpfr->value, value, rnd);
    n.m_unit = unit;
  }

  void
  number_access::init_mpfr_copy(
    number& n,
    mpfr_srcptr src,
    const number::unit_type& unit
  )
  {
    n.m_storage = storage_kind::mpfr;
    n.m_small = 0;
    n.m_mpfr = std::make_unique<mpfr_holder>();
    mpfr_set(n.m_mpfr->value, src, number::default_rounding_mode);
    n.m_unit = unit;
  }

  void
  number_access::init_mpfr_inf(number& n, const number::unit_type& unit)
  {
    n.m_storage = storage_kind::mpfr;
    n.m_small = 0;
    n.m_mpfr = std::make_unique<mpfr_holder>();
    mpfr_set_inf(n.m_mpfr->value, 0);
    n.m_unit = unit;
  }

  void
  number_access::init_mpfr_nan(number& n, const number::unit_type& unit)
  {
    n.m_storage = storage_kind::mpfr;
    n.m_small = 0;
    n.m_mpfr = std::make_unique<mpfr_holder>();
    mpfr_set_nan(n.m_mpfr->value);
    n.m_unit = unit;
  }

  bool
  number_access::is_small(const number& n)
  {
    return n.m_storage == storage_kind::small;
  }

  bool
  number_access::is_mpfr(const number& n)
  {
    return n.m_storage == storage_kind::mpfr;
  }

  std::int64_t
  number_access::small_value(const number& n)
  {
    assert(is_small(n));

    return n.m_small;
  }

  mpfr_srcptr
  number_access::mpfr_value(const number& n)
  {
    assert(is_mpfr(n) && n.m_mpfr);

    return n.m_mpfr->value;
  }

  mpfr_t&
  number_access::mpfr_mut(number& n)
  {
    promote_to_mpfr(n);

    return n.m_mpfr->value;
  }

  void
  number_access::destroy(number& n)
  {
    n.m_mpfr.reset();
    n.m_storage = storage_kind::small;
    n.m_small = 0;
    n.m_unit.reset();
  }

  void
  number_access::copy_from(number& dst, const number& src)
  {
    destroy(dst);
    dst.m_unit = src.m_unit;

    if (src.m_storage == storage_kind::small)
    {
      dst.m_storage = storage_kind::small;
      dst.m_small = src.m_small;
    }
    else
    {
      dst.m_storage = storage_kind::mpfr;
      dst.m_mpfr = std::make_unique<mpfr_holder>();
      mpfr_set(
        dst.m_mpfr->value,
        src.m_mpfr->value,
        number::default_rounding_mode
      );
    }
  }

  void
  number_access::move_from(number& dst, number&& src)
  {
    destroy(dst);
    dst.m_unit = std::move(src.m_unit);
    dst.m_storage = src.m_storage;
    dst.m_small = src.m_small;
    dst.m_mpfr = std::move(src.m_mpfr);
    src.m_storage = storage_kind::small;
    src.m_small = 0;
  }

  void
  number_access::promote_to_mpfr(number& n, mpfr_rnd_t rnd)
  {
    if (n.m_storage == storage_kind::mpfr)
    {
      return;
    }

    const std::int64_t v = n.m_small;

    n.m_mpfr = std::make_unique<mpfr_holder>();
    mpfr_set_sj(n.m_mpfr->value, v, rnd);
    n.m_storage = storage_kind::mpfr;
  }

  bool
  number_access::add_small(std::int64_t a, std::int64_t b, std::int64_t& out)
  {
#if defined(__has_builtin) && __has_builtin(__builtin_add_overflow)
    return !__builtin_add_overflow(a, b, &out);
#else
    const __int128 result = static_cast<__int128>(a) + static_cast<__int128>(b);

    if (result < INT64_MIN || result > INT64_MAX)
    {
      return false;
    }

    out = static_cast<std::int64_t>(result);

    return true;
#endif
  }

  bool
  number_access::sub_small(std::int64_t a, std::int64_t b, std::int64_t& out)
  {
#if defined(__has_builtin) && __has_builtin(__builtin_sub_overflow)
    return !__builtin_sub_overflow(a, b, &out);
#else
    const __int128 result = static_cast<__int128>(a) - static_cast<__int128>(b);

    if (result < INT64_MIN || result > INT64_MAX)
    {
      return false;
    }

    out = static_cast<std::int64_t>(result);

    return true;
#endif
  }

  bool
  number_access::mul_small(std::int64_t a, std::int64_t b, std::int64_t& out)
  {
#if defined(__has_builtin) && __has_builtin(__builtin_mul_overflow)
    return !__builtin_mul_overflow(a, b, &out);
#else
    const __int128 result = static_cast<__int128>(a) * static_cast<__int128>(b);

    if (result < INT64_MIN || result > INT64_MAX)
    {
      return false;
    }

    out = static_cast<std::int64_t>(result);

    return true;
#endif
  }

  void
  init_small(number& n, std::int64_t value)
  {
    number_access::init_small(n, value);
  }

  void
  init_mpfr_si(number& n, long value, const number::unit_type& unit)
  {
    number_access::init_mpfr_si(n, value, unit);
  }

  void
  init_mpfr_s64(number& n, std::int64_t value, const number::unit_type& unit)
  {
    number_access::init_mpfr_s64(n, value, unit);
  }

  void
  init_mpfr_d(
    number& n,
    double value,
    const number::unit_type& unit,
    mpfr_rnd_t rnd
  )
  {
    number_access::init_mpfr_d(n, value, unit, rnd);
  }

  void
  init_mpfr_copy(number& n, mpfr_srcptr src, const number::unit_type& unit)
  {
    number_access::init_mpfr_copy(n, src, unit);
  }

  void
  init_mpfr_inf(number& n, const number::unit_type& unit)
  {
    number_access::init_mpfr_inf(n, unit);
  }

  void
  init_mpfr_nan(number& n, const number::unit_type& unit)
  {
    number_access::init_mpfr_nan(n, unit);
  }

  bool
  is_small(const number& n)
  {
    return number_access::is_small(n);
  }

  bool
  is_mpfr(const number& n)
  {
    return number_access::is_mpfr(n);
  }

  std::int64_t
  small_value(const number& n)
  {
    return number_access::small_value(n);
  }

  mpfr_srcptr
  mpfr_value(const number& n)
  {
    return number_access::mpfr_value(n);
  }

  mpfr_t&
  mpfr_mut(number& n)
  {
    return number_access::mpfr_mut(n);
  }

  void
  destroy(number& n)
  {
    number_access::destroy(n);
  }

  void
  copy_from(number& dst, const number& src)
  {
    number_access::copy_from(dst, src);
  }

  void
  move_from(number& dst, number&& src)
  {
    number_access::move_from(dst, std::move(src));
  }

  void
  promote_to_mpfr(number& n, mpfr_rnd_t rnd)
  {
    number_access::promote_to_mpfr(n, rnd);
  }

  bool
  add_small(std::int64_t a, std::int64_t b, std::int64_t& out)
  {
    return number_access::add_small(a, b, out);
  }

  bool
  sub_small(std::int64_t a, std::int64_t b, std::int64_t& out)
  {
    return number_access::sub_small(a, b, out);
  }

  bool
  mul_small(std::int64_t a, std::int64_t b, std::int64_t& out)
  {
    return number_access::mul_small(a, b, out);
  }
}
