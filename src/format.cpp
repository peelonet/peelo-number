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
  std::string
  number::to_string(const std::string& format, rounding_mode rounding) const
  {
    std::string result;
    char* buffer = nullptr;
    const auto length = mpfr_asprintf(
      &buffer,
      "%.10R*g",
      rounding,
      m_value
    );

    if (length >= 0)
    {
      result.append(buffer, length);
      mpfr_free_str(buffer);
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
