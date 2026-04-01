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
#include <catch2/catch_test_macros.hpp>

#include "peelo/number.hpp"

using number = peelo::number;
using unit = peelo::number::unit;

TEST_CASE("Parsing simple integers")
{
  REQUIRE(long(number::parse("123")) == 123);
  REQUIRE(long(number::parse("+123")) == 123);
  REQUIRE(long(number::parse("-123")) == -123);
}

TEST_CASE("Parsing floating point decimals")
{
  REQUIRE(double(number::parse("2.55")) == 2.55);
  REQUIRE(double(number::parse("+2.55")) == 2.55);
  REQUIRE(double(number::parse("-2.55")) == -2.55);
}

TEST_CASE("Parsing with base different than zero")
{
  REQUIRE(long(number::parse("fa", 16)) == 250);
  REQUIRE(long(number::parse("10101", 2)) == 21);
  REQUIRE(long(number::parse("64", 8)) == 52);
}

TEST_CASE("Parsing measurement units")
{
  const auto result = number::parse("13km");

  REQUIRE(!!result.measurement_unit());
  REQUIRE(*result.measurement_unit() == unit::kilometer);
  REQUIRE(!number::parse("1xxx").measurement_unit());
}
