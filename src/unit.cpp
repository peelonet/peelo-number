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
#include <unordered_map>

#include "peelo/number.hpp"

namespace peelo
{
  // Supported length units.
  const number::unit number::unit::millimeter =
  {
    number::unit::type::length,
    "mm",
    -1000
  };
  const number::unit number::unit::centimeter =
  {
    number::unit::type::length,
    "cm",
    -100
  };
  const number::unit number::unit::meter =
  {
    number::unit::type::length,
    "m",
    1
  };
  const number::unit number::unit::kilometer =
  {
    number::unit::type::length,
    "km",
    1000
  };

  // Supported mass units.
  const number::unit number::unit::milligram =
  {
    number::unit::type::mass,
    "mg",
    -1000000
  };
  const number::unit number::unit::gram =
  {
    number::unit::type::mass,
    "g",
    -1000
  };
  const number::unit number::unit::kilogram =
  {
    unit::type::mass,
    "kg",
    1
  };

  // Supported time units.
  const number::unit number::unit::millisecond =
  {
    number::unit::type::time,
    "ms",
    -1000
  };
  const number::unit number::unit::second =
  {
    unit::type::time,
    "s",
    1
  };
  const number::unit number::unit::minute =
  {
    unit::type::time,
    "min",
    60
  };
  const number::unit number::unit::hour =
  {
    unit::type::time,
    "h",
    3600
  };
  const number::unit number::unit::day =
  {
    unit::type::time,
    "d",
    86400
  };

  static const std::unordered_map<std::string, number::unit> symbol_mapping =
  {
    // Length units.
    { "mm", number::unit::millimeter },
    { "cm", number::unit::centimeter },
    { "m", number::unit::meter },
    { "km", number::unit::kilometer },

    // Mass units.
    { "mg", number::unit::milligram },
    { "g", number::unit::gram },
    { "kg", number::unit::kilogram },

    // Time units.
    { "ms", number::unit::millisecond },
    { "s", number::unit::second },
    { "min", number::unit::minute },
    { "h", number::unit::hour },
    { "d", number::unit::day },
  };

  std::optional<number::unit>
  number::unit::find_by_symbol(const std::string& symbol)
  {
    const auto entry = symbol_mapping.find(symbol);

    if (entry != std::end(symbol_mapping))
    {
      return entry->second;
    }

    return std::nullopt;
  }

  static const std::vector<number::unit> all_length_units =
  {
    number::unit::millimeter,
    number::unit::centimeter,
    number::unit::meter,
    number::unit::kilometer,
  };

  static const std::vector<number::unit> all_mass_units =
  {
    number::unit::kilogram,
    number::unit::gram,
    number::unit::milligram
  };

  static const std::vector<number::unit> all_time_units =
  {
    number::unit::day,
    number::unit::hour,
    number::unit::minute,
    number::unit::second,
    number::unit::millisecond
  };

  const number::unit&
  number::unit::base_unit_of(enum type type)
  {
    switch (type)
    {
      case number::unit::type::length:
        return number::unit::meter;

      case number::unit::type::mass:
        return number::unit::kilogram;

      case number::unit::type::time:
        return number::unit::second;
    }

    // Just to keep the compiler happy.
    return number::unit::meter;
  }

  const std::vector<number::unit>&
  number::unit::all_units_of(enum type type)
  {
    switch (type)
    {
      case number::unit::type::length:
        return all_length_units;

      case number::unit::type::mass:
        return all_mass_units;

      case number::unit::type::time:
        return all_time_units;
    }

    // Just to keep the compiler happy.
    return all_length_units;
  }
}
