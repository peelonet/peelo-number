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

TEST_CASE("equals()")
{
  REQUIRE(number(1.0).equals(number(1.0)));
  REQUIRE(!number(1.0).equals(number(2.0)));
  REQUIRE(number(1.0, unit::meter).equals(number(1.0, unit::meter)));
  REQUIRE(!number(1.0, unit::meter).equals(number(1.0)));
  REQUIRE(!number(1.0, unit::meter).equals(number(1.0, unit::day)));
  REQUIRE(number(1000.0, unit::meter).equals(number(1.0, unit::kilometer)));

  REQUIRE(number(1.0).equals(1.0));
  REQUIRE(!number(1.0).equals(2.0));
  REQUIRE(number(1.0, unit::meter).equals(1.0, unit::meter));
  REQUIRE(!number(1.0, unit::meter).equals(1.0));
  REQUIRE(!number(1.0, unit::meter).equals(1.0, unit::day));
  REQUIRE(number(1000.0, unit::meter).equals(1.0, unit::kilometer));
}

TEST_CASE("Equality testing operators")
{
  REQUIRE(number(1.0) == number(1.0));
  REQUIRE(number(1.0, unit::kilometer) == number(1000.0, unit::meter));
  REQUIRE(number(1.0, unit::meter) != number(1.0));
  REQUIRE(number(1.0, unit::meter) != number(1.0, unit::day));

  REQUIRE(number(2.0) == 2.0);
  REQUIRE(number(2.0, unit::meter) != 2.0);
}

TEST_CASE("compare()")
{
  REQUIRE(number(1.0).compare(number(1.0)) == 0);
  REQUIRE(number(1.0).compare(number(2.0)) == -1);
  REQUIRE(number(1.0).compare(number(0.5)) == 1);
  REQUIRE(number(1.0, unit::meter).compare(number(1.0, unit::kilometer)) == -1);

  REQUIRE(number(1.0).compare(1.0) == 0);
  REQUIRE(number(1.0).compare(2.0) == -1);
  REQUIRE(number(1.0).compare(0.5) == 1);
  REQUIRE(number(1.0, unit::meter).compare(1.0, unit::kilometer) == -1);

  REQUIRE_THROWS_AS(
    number(1.0).compare(number(1.0, unit::meter)),
    number::unit_error
  );
  REQUIRE_THROWS_AS(
    number(1.0, unit::meter).compare(number(1.0, unit::day)),
    number::unit_error
  );

  REQUIRE_THROWS_AS(
    number(1.0).compare(1.0, unit::meter),
    number::unit_error
  );
  REQUIRE_THROWS_AS(
    number(1.0, unit::meter).compare(1.0, unit::day),
    number::unit_error
  );
}

TEST_CASE("Comparison operators")
{
  REQUIRE(number(1.0) < number(2.0));
  REQUIRE(number(2.0) > number(1.0));
  REQUIRE(number(1.0) <= number(2.0));
  REQUIRE(number(2.0) >= number(1.0));
  REQUIRE(number(1.0, unit::meter) < number(1.0, unit::kilometer));

  REQUIRE(number(1.0) < 2.0);
  REQUIRE(number(2.0) > 1.0);
  REQUIRE(number(1.0) <= 2.0);
  REQUIRE(number(2.0) >= 1.0);
}
