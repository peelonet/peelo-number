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

TEST_CASE("Validating input")
{
  REQUIRE(number::is_valid("5"));
  REQUIRE(number::is_valid("-5"));
  REQUIRE(number::is_valid(U"-2.5"));
  REQUIRE(number::is_valid(U"15km"));
  REQUIRE(number::is_valid("ffa", 16));
  REQUIRE(!number::is_valid(""));
  REQUIRE(!number::is_valid("12xxx"));
}

TEST_CASE("Parsing simple integers")
{
  REQUIRE(number::parse("123").equals(123));
  REQUIRE(number::parse("+123").equals(123));
  REQUIRE(number::parse("-123").equals(-123));
}

TEST_CASE("Parsing floating point decimals")
{
  REQUIRE(number::parse("2.55").equals(2.55));
  REQUIRE(number::parse("+2.55").equals(2.55));
  REQUIRE(number::parse("-2.55").equals(-2.55));
}

TEST_CASE("Parsing with base different than zero")
{
  REQUIRE(number::parse("fa", 16).equals(250));
  REQUIRE(number::parse("10101", 2).equals(21));
  REQUIRE(number::parse("64", 8).equals(52));
}

TEST_CASE("Parsing measurement units")
{
  REQUIRE(number::parse("13km").equals(13, number::unit::kilometer));
  REQUIRE(number::parse("-5min").equals(-5, number::unit::minute));
}

TEST_CASE("Invalid input parsing")
{
  REQUIRE_THROWS_AS(number::parse(""), std::invalid_argument);
  REQUIRE_THROWS_AS(number::parse("x"), std::invalid_argument);
  REQUIRE_THROWS_AS(number::parse("5x"), std::invalid_argument);
}

TEST_CASE("Parsing Unicode string")
{
  REQUIRE(number::parse(U"2.5").equals(2.5));
  REQUIRE(number::parse(U"13km").equals(13, number::unit::kilometer));
}
