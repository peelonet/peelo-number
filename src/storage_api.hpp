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

namespace peelo::internal
{
  struct number_access
  {
    static void init_small(number& n, std::int64_t value);
    static void init_mpfr_si(
      number& n,
      long value,
      const number::unit_type& unit = std::nullopt
    );
    static void init_mpfr_s64(
      number& n,
      std::int64_t value,
      const number::unit_type& unit
    );
    static void init_mpfr_d(
      number& n,
      double value,
      const number::unit_type& unit,
      mpfr_rnd_t rnd
    );
    static void init_mpfr_copy(
      number& n,
      mpfr_srcptr src,
      const number::unit_type& unit
    );
    static void init_mpfr_inf(number& n, const number::unit_type& unit);
    static void init_mpfr_nan(number& n, const number::unit_type& unit);

    static bool is_small(const number& n);
    static bool is_mpfr(const number& n);
    static std::int64_t small_value(const number& n);
    static mpfr_srcptr mpfr_value(const number& n);
    static mpfr_t& mpfr_mut(number& n);

    static void destroy(number& n);
    static void copy_from(number& dst, const number& src);
    static void move_from(number& dst, number&& src);

    static void promote_to_mpfr(
      number& n,
      mpfr_rnd_t rnd = number::default_rounding_mode
    );

    static bool add_small(std::int64_t a, std::int64_t b, std::int64_t& out);
    static bool sub_small(std::int64_t a, std::int64_t b, std::int64_t& out);
    static bool mul_small(std::int64_t a, std::int64_t b, std::int64_t& out);
  };

  void init_small(number& n, std::int64_t value);
  void init_mpfr_si(
    number& n,
    long value,
    const number::unit_type& unit = std::nullopt
  );
  void init_mpfr_s64(
    number& n,
    std::int64_t value,
    const number::unit_type& unit
  );
  void init_mpfr_d(
    number& n,
    double value,
    const number::unit_type& unit,
    mpfr_rnd_t rnd
  );
  void init_mpfr_copy(
    number& n,
    mpfr_srcptr src,
    const number::unit_type& unit
  );
  void init_mpfr_inf(number& n, const number::unit_type& unit);
  void init_mpfr_nan(number& n, const number::unit_type& unit);

  bool is_small(const number& n);
  bool is_mpfr(const number& n);
  std::int64_t small_value(const number& n);
  mpfr_srcptr mpfr_value(const number& n);
  mpfr_t& mpfr_mut(number& n);

  void destroy(number& n);
  void copy_from(number& dst, const number& src);
  void move_from(number& dst, number&& src);

  void promote_to_mpfr(
    number& n,
    mpfr_rnd_t rnd = number::default_rounding_mode
  );

  bool add_small(std::int64_t a, std::int64_t b, std::int64_t& out);
  bool sub_small(std::int64_t a, std::int64_t b, std::int64_t& out);
  bool mul_small(std::int64_t a, std::int64_t b, std::int64_t& out);
}
