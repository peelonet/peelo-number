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

#include "peelo/number.hpp"

namespace peelo
{
  using digit_tester_callback = int(*)(int);

  number
  number::parse(const std::string& input, int base, rounding_mode rounding)
  {
    const auto length = input.length();
    std::string::size_type start;
    bool dot_seen = false;
    number result;
    digit_tester_callback tester = std::isdigit;

    if (!length)
    {
      return result;
    }
    else if (input[0] == '+' || input[0] == '-')
    {
      start = 1;
    } else {
      start = 0;
    }
    if (base == 16)
    {
      tester = std::isxdigit;
    }
    for (std::string::size_type i = start; i < length; ++i)
    {
      const auto& c = input[i];

      if (c == '.')
      {
        if (dot_seen || i == start || i + 1 > length)
        {
          break;
        }
        dot_seen = true;
      }
      else if (!tester(c))
      {
        mpfr_init_set_str(
          result.m_value,
          input.substr(0, i).c_str(),
          base,
          rounding
        );
        result.m_unit = unit::find_by_symbol(input.substr(i, length - i));

        return result;
      }
    }
    mpfr_init_set_str(result.m_value, input.c_str(), base, rounding);

    return result;
  }
}
