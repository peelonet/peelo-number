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
#include <cctype>
#include <cstddef>
#include <functional>
#include <stdexcept>

#include "peelo/number.hpp"

namespace peelo
{
  using digit_test_function = int(*)(int);

  template<class CharT>
  static bool
  validator_backend(
    const std::basic_string<CharT>& input,
    int base,
    const std::function<
      std::string(const std::basic_string<CharT>&)
    >& encoder
  )
  {
    const auto length = input.length();
    std::size_t start;
    digit_test_function tester = std::isdigit;
    bool dot_seen = false;

    if (base == 16)
    {
      tester = std::isxdigit;
    }

    if (!length)
    {
      return false;
    }
    if (input[0] == '+' || input[0] == '-')
    {
      start = 1;
      if (length < 2)
      {
        return false;
      }
    } else {
      start = 0;
    }
    for (std::size_t i = start; i < length; ++i)
    {
      const auto& c = input[i];

      if (c == '.')
      {
        if (dot_seen || i == start || i + 1 > length)
        {
          return false;
        }
        dot_seen = true;
      }
      else if (!tester(c))
      {
        return number::unit::find_by_symbol(
          encoder(input.substr(i, length - i))
        ).has_value();
      }
    }

    return true;
  }

  template<class CharT>
  static void
  parser_backend(
    const std::basic_string<CharT>& input,
    int base,
    number::rounding_mode rounding,
    number::value_type value,
    number::unit_type& unit,
    const std::function<
      std::string(const std::basic_string<CharT>&)
    >& encoder
  )
  {
    const auto length = input.length();
    std::size_t start;
    digit_test_function tester = std::isdigit;
    bool dot_seen = false;

    if (base == 16)
    {
      tester = std::isxdigit;
    }

    if (!length)
    {
      throw std::invalid_argument("input contains nothing");
    }
    else if (input[0] == '+' || input[0] == '-')
    {
      start = 1;
    } else {
      start = 0;
    }

    for (std::size_t i = start; i < length; ++i)
    {
      const auto& c = input[i];

      if (c == '.')
      {
        if (dot_seen || i == start || i + 1 > length)
        {
          throw std::invalid_argument("multiple `.' seen in the input");
        }
        dot_seen = true;
      }
      else if (!tester(c))
      {
        if (i == 0)
        {
          throw std::invalid_argument("input does not contain a number");
        }
        if (mpfr_set_str(
          value,
          encoder(input.substr(0, i)).c_str(),
          base,
          rounding
        ) == -1)
        {
          throw std::invalid_argument("input does not contain a number");
        }
        unit = number::unit::find_by_symbol(
          encoder(input.substr(i, length - i))
        );
        if (!unit)
        {
          throw std::invalid_argument("unrecognized measurement unit");
        }
        return;
      }
    }

    if (mpfr_set_str(
      value,
      encoder(input).c_str(),
      base,
      rounding
    ) == -1)
    {
      throw std::invalid_argument("input does not contain a number");
    }
  }

  static inline std::string
  char_encoder(const std::string& input)
  {
    return input;
  }

  static inline std::string
  char32_t_encoder(const std::u32string& input)
  {
    const auto length = input.length();
    std::string result;

    result.reserve(length);
    for (std::u32string::size_type i = 0; i < length; ++i)
    {
      result.push_back(static_cast<char>(input[i]));
    }

    return result;
  }

  bool
  number::is_valid(const std::string& input, int base)
  {
    return validator_backend<char>(input, base, char_encoder);
  }

  bool
  number::is_valid(const std::u32string& input, int base)
  {
    return validator_backend<char32_t>(input, base, char32_t_encoder);
  }

  number
  number::parse(const std::string& input, int base, rounding_mode rounding)
  {
    number result;

    parser_backend<char>(
      input,
      base,
      rounding,
      result.m_value,
      result.m_unit,
      char_encoder
    );

    return result;
  }

  number
  number::parse(const std::u32string& input, int base, rounding_mode rounding)
  {
    number result;

    parser_backend<char32_t>(
      input,
      base,
      rounding,
      result.m_value,
      result.m_unit,
      char32_t_encoder
    );

    return result;
  }
}
