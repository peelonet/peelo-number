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

TEST_CASE("Removal of measurement unit")
{
  REQUIRE(!number(1, unit::day).without_measurement_unit().measurement_unit());
}

TEST_CASE("Conversion to double precision")
{
  REQUIRE(double(number(5.5)) == 5.5);
  REQUIRE(double(number(5.0, unit::kilometer)) == 5000.0);
  // TODO: Over and underflow testing.
}

TEST_CASE("Conversion to long integer")
{
  REQUIRE(long(number(5.0)) == 5);
  REQUIRE(long(number(1, unit::day)) == 86400);
  REQUIRE(long(number(2.2)) == 2);
  REQUIRE(long(number(2.6)) == 3);
  // TODO: Over and underflow testing.
}

TEST_CASE("Conversion to boolean")
{
  REQUIRE(bool(number(1.0)) == true);
  REQUIRE(bool(number(0.0)) == false);
  REQUIRE(!number(1.0) == false);
  REQUIRE(!number(0.0) == true);
}
