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
#include <cerrno>
#include <climits>
#include <cstddef>
#include <cstdlib>
#include <stdexcept>
#include <string_view>

#include "peelo/number.hpp"

#include "./storage_api.hpp"

namespace peelo
{
  using digit_test_function = int(*)(int);

  static inline bool
  is_underscore(char c)
  {
    return c == '_';
  }

  static inline int
  as_digit(int c)
  {
    return static_cast<unsigned char>(c);
  }

  static inline bool
  is_valid_underscore(
    std::string_view input,
    std::size_t index,
    digit_test_function tester
  )
  {
    if (index == 0 || index + 1 >= input.length())
    {
      return false;
    }

    return tester(as_digit(input[index - 1]))
      && tester(as_digit(input[index + 1]));
  }

  static std::string
  strip_underscores(std::string_view input)
  {
    for (const char c : input)
    {
      if (is_underscore(c))
      {
        std::string result;

        result.reserve(input.length());
        for (const char ch : input)
        {
          if (!is_underscore(ch))
          {
            result.push_back(ch);
          }
        }

        return result;
      }
    }

    return std::string(input);
  }

  static void
  parse_mpfr_str(
    number& result,
    const std::string& numeric,
    int base,
    number::rounding_mode rounding,
    const number::unit_type& unit
  )
  {
    internal::destroy(result);
    internal::init_mpfr_si(result, 0, unit);
    if (mpfr_set_str(
      internal::mpfr_mut(result),
      numeric.c_str(),
      base,
      rounding
    ) == -1)
    {
      throw std::invalid_argument("input does not contain a number");
    }
  }

  static void
  parse_numeric_token(
    number& result,
    const std::string& numeric,
    int base,
    number::rounding_mode rounding,
    const number::unit_type& unit,
    bool dot_seen
  )
  {
    if (unit || dot_seen)
    {
      parse_mpfr_str(result, numeric, base, rounding, unit);
      return;
    }

    errno = 0;
    char* end = nullptr;
    const long long parsed = std::strtoll(numeric.c_str(), &end, base);

    if (end != numeric.c_str() + numeric.length() || errno == ERANGE)
    {
      parse_mpfr_str(result, numeric, base, rounding, unit);
      return;
    }

    internal::destroy(result);
    internal::init_small(result, static_cast<std::int64_t>(parsed));
  }

  static bool
  validator_backend(std::string_view input, int base)
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
      const char c = input[i];

      if (c == '.')
      {
        if (dot_seen || i == start || i + 1 > length)
        {
          return false;
        }
        dot_seen = true;
      }
      else if (is_underscore(c))
      {
        if (!is_valid_underscore(input, i, tester))
        {
          return false;
        }
      }
      else if (!tester(as_digit(c)))
      {
        return number::unit::find_by_symbol(input.substr(i)).has_value();
      }
    }

    return true;
  }

  static void
  parser_backend(
    std::string_view input,
    int base,
    number::rounding_mode rounding,
    number& result
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
      const char c = input[i];

      if (c == '.')
      {
        if (dot_seen || i == start || i + 1 > length)
        {
          throw std::invalid_argument("multiple `.' seen in the input");
        }
        dot_seen = true;
      }
      else if (is_underscore(c))
      {
        if (!is_valid_underscore(input, i, tester))
        {
          throw std::invalid_argument("invalid underscore placement in input");
        }
      }
      else if (!tester(as_digit(c)))
      {
        if (i == 0)
        {
          throw std::invalid_argument("input does not contain a number");
        }

        const auto numeric = strip_underscores(input.substr(0, i));
        const auto unit = number::unit::find_by_symbol(input.substr(i));

        if (!unit)
        {
          throw std::invalid_argument("unrecognized measurement unit");
        }

        parse_numeric_token(result, numeric, base, rounding, unit, dot_seen);

        return;
      }
    }

    parse_numeric_token(
      result,
      strip_underscores(input),
      base,
      rounding,
      std::nullopt,
      dot_seen
    );
  }

  static std::string
  u32string_to_ascii(const std::u32string& input)
  {
    std::string result;

    result.reserve(input.length());
    for (const char32_t c : input)
    {
      result.push_back(static_cast<char>(c));
    }

    return result;
  }

  bool
  number::is_valid(std::string_view input, int base)
  {
    return validator_backend(input, base);
  }

  bool
  number::is_valid(const std::u32string& input, int base)
  {
    return validator_backend(u32string_to_ascii(input), base);
  }

  number
  number::parse(std::string_view input, int base, rounding_mode rounding)
  {
    number result;

    parser_backend(input, base, rounding, result);

    return result;
  }

  number
  number::parse(const std::u32string& input, int base, rounding_mode rounding)
  {
    number result;

    parser_backend(u32string_to_ascii(input), base, rounding, result);

    return result;
  }
}
