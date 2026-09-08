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
#include <functional>
#include <stdexcept>

#include "peelo/number.hpp"

#include "./storage_api.hpp"

namespace peelo
{
  using digit_test_function = int(*)(int);

  static inline bool
  is_underscore(int c)
  {
    return c == '_';
  }

  static inline bool
  is_valid_underscore(
    const std::string& input,
    std::size_t index,
    digit_test_function tester
  )
  {
    if (index == 0 || index + 1 >= input.length())
    {
      return false;
    }

    return tester(input[index - 1]) && tester(input[index + 1]);
  }

  static inline std::string
  strip_underscores(const std::string& input)
  {
    std::string result;

    result.reserve(input.length());
    for (const auto c : input)
    {
      if (c != '_')
      {
        result.push_back(c);
      }
    }

    return result;
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
      else if (is_underscore(c))
      {
        if (!is_valid_underscore(encoder(input), i, tester))
        {
          return false;
        }
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
    number& result,
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
      else if (is_underscore(c))
      {
        if (!is_valid_underscore(encoder(input), i, tester))
        {
          throw std::invalid_argument("invalid underscore placement in input");
        }
      }
      else if (!tester(c))
      {
        if (i == 0)
        {
          throw std::invalid_argument("input does not contain a number");
        }

        const auto numeric = strip_underscores(
          encoder(input.substr(0, i))
        );
        const auto unit = number::unit::find_by_symbol(
          encoder(input.substr(i, length - i))
        );

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
      strip_underscores(encoder(input)),
      base,
      rounding,
      std::nullopt,
      dot_seen
    );
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

    parser_backend<char>(input, base, rounding, result, char_encoder);

    return result;
  }

  number
  number::parse(const std::u32string& input, int base, rounding_mode rounding)
  {
    number result;

    parser_backend<char32_t>(input, base, rounding, result, char32_t_encoder);

    return result;
  }
}
