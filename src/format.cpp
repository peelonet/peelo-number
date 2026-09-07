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
  namespace
  {
    static std::string
    plain_decimal_string(
      mpfr_srcptr value,
      mpfr_rnd_t rounding,
      std::size_t significant_digits
    )
    {
      if (mpfr_nan_p(value))
      {
        return "nan";
      }
      if (mpfr_inf_p(value))
      {
        return mpfr_sgn(value) < 0 ? "-inf" : "inf";
      }
      if (mpfr_zero_p(value))
      {
        return "0";
      }

      mpfr_exp_t exponent;
      char* raw = mpfr_get_str(
        nullptr,
        &exponent,
        10,
        significant_digits,
        value,
        rounding
      );
      std::string significand(raw);

      mpfr_free_str(raw);

      bool negative = !significand.empty() && significand[0] == '-';

      if (negative)
      {
        significand.erase(0, 1);
      }

      const auto length = significand.length();
      const mpfr_exp_t offset = exponent - static_cast<mpfr_exp_t>(length);
      std::string result = negative ? "-" : "";

      if (offset >= 0)
      {
        result += significand;
        result.append(static_cast<std::size_t>(offset), '0');
      }
      else
      {
        const mpfr_exp_t point = static_cast<mpfr_exp_t>(length) + offset;

        if (point <= 0)
        {
          result += "0.";
          result.append(static_cast<std::size_t>(-point), '0');
          result += significand;
        }
        else
        {
          result += significand.substr(0, static_cast<std::size_t>(point));
          result += '.';
          result += significand.substr(static_cast<std::size_t>(point));
        }
      }

      const auto dot = result.find('.');

      if (dot != std::string::npos)
      {
        const auto end = result.find_last_not_of('0');

        if (end != std::string::npos)
        {
          if (end == dot)
          {
            result.erase(dot);
          }
          else if (end > dot)
          {
            result.erase(end + 1);
          }
        }
      }

      return result;
    }
  }

  std::string
  number::to_string(const std::string& format, rounding_mode rounding) const
  {
    std::string result;

    if (format.empty())
    {
      result = plain_decimal_string(m_value, rounding, 10);
    }
    else
    {
      char* buffer = nullptr;
      const auto length = mpfr_asprintf(
        &buffer,
        format.c_str(),
        rounding,
        m_value
      );

      if (length >= 0)
      {
        result.append(buffer, length);
        mpfr_free_str(buffer);
      }
    }

    if (m_unit)
    {
      result.append(m_unit->symbol);
    }

    return result;
  }

  std::u32string
  number::to_u32string(const std::string& format, rounding_mode rounding) const
  {
    const auto input = to_string(format, rounding);
    const auto length = input.length();
    std::u32string result;

    result.reserve(length);
    for (std::string::size_type i = 0; i < length; ++i)
    {
      result.push_back(static_cast<char32_t>(input[i]));
    }

    return result;
  }

  std::string
  to_string(enum number::unit::type type)
  {
    switch (type)
    {
      case number::unit::type::length:
        return "length";

      case number::unit::type::mass:
        return "mass";

      case number::unit::type::time:
        return "time";
    }

    return "unknown";
  }
}
