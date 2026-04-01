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

TEST_CASE("add()")
{
  REQUIRE(
    number(1.0)
      .add(number(2.0))
      .equals(3.0)
  );
  REQUIRE(
    number(1.0, unit::kilometer)
      .add(number(2.0, unit::kilometer))
      .equals(3000.0, unit::meter)
  );

  REQUIRE(
    number(1.0)
      .add(2.0)
      .equals(3.0)
  );
  REQUIRE(
    number(1.0, unit::kilometer)
      .add(2.0, unit::kilometer)
      .equals(3000.0, unit::meter)
  );

  REQUIRE_THROWS_AS(
    number(1.0, unit::meter).add(number(1.0, unit::second)),
    number::unit_error
  );
  REQUIRE_THROWS_AS(
    number(1.0, unit::meter).add(1.0, unit::second),
    number::unit_error
  );
}

TEST_CASE("substract()")
{
  REQUIRE(
    number(5.5)
      .substract(number(2.0))
      .equals(3.5)
  );
  REQUIRE(
    number(5.0, unit::kilometer)
      .substract(number(2.0, unit::kilometer))
      .equals(3000.0, unit::meter)
  );

  REQUIRE(
    number(1.0)
      .substract(2.0)
      .equals(-1.0)
  );
  REQUIRE(
    number(5.0, unit::kilometer)
      .substract(2.0, unit::kilometer)
      .equals(3000.0, unit::meter)
  );

  REQUIRE_THROWS_AS(
    number(1.0, unit::meter).substract(number(0.5, unit::second)),
    number::unit_error
  );
  REQUIRE_THROWS_AS(
    number(1.0, unit::meter).substract(1.0, unit::second),
    number::unit_error
  );
}

TEST_CASE("multiply()")
{
  REQUIRE(
    number(2.0, unit::kilometer)
      .multiply(number(2.0, unit::meter))
      .equals(4000.0, unit::meter)
  );
  REQUIRE(
    number(2.0, unit::kilometer)
      .multiply(2.0)
      .equals(4.0, unit::kilometer)
  );

  REQUIRE_THROWS_AS(
    number(1.0, unit::meter).multiply(number(0.5, unit::second)),
    number::unit_error
  );
  REQUIRE_THROWS_AS(
    number(1.0, unit::meter).multiply(1.0, unit::second),
    number::unit_error
  );
}

TEST_CASE("divide()")
{
  REQUIRE(
    number(4.0, unit::kilometer)
      .divide(number(2.0))
      .equals(2.0, unit::kilometer)
  );
  REQUIRE(
    number(4.0, unit::kilometer)
      .divide(2.0)
      .equals(2.0, unit::kilometer)
  );

  REQUIRE(number(4.0).divide(number()).is_inf());
  REQUIRE(number(2.0).divide(0.0).is_inf());

  REQUIRE_THROWS_AS(
    number(1.0, unit::meter).divide(number(0.5, unit::second)),
    number::unit_error
  );
  REQUIRE_THROWS_AS(
    number(1.0, unit::meter).divide(1.0, unit::second),
    number::unit_error
  );
}

TEST_CASE("modulo()")
{
  REQUIRE(number(-10.0).modulo(3.0).equals(-1.0));
  REQUIRE(number(2.0).modulo(0.0).is_nan());
}

TEST_CASE("Arithmetic operators")
{
  REQUIRE(number(1.0) + number(2.0) == 3.0);
  REQUIRE(number(1.0) + 2.0 == 3.0);

  REQUIRE(number(2.0) - number(1.5) == 0.5);
  REQUIRE(number(2.5) - 0.5 == 2.0);

  REQUIRE(number(2.0) * number(2.0) == 4.0);
  REQUIRE(number(3.0) * 2.0 == 6.0);

  REQUIRE(number(4.0) / number(2.0) == 2.0);
  REQUIRE(number(6.0) / 2.0 == 3.0);

  REQUIRE(number(-10.0) % number(3.0) == -1.0);
  REQUIRE(number(-10.0) % 3.0 == -1.0);
}

TEST_CASE("Arithmetic assignment operators")
{
  number test(1.0, unit::meter);

  test += 1.0;
  REQUIRE(test.equals(2.0, unit::meter));

  test += number(100.0, unit::centimeter);
  REQUIRE(test.equals(3.0, unit::meter));

  test -= 1.0;
  REQUIRE(test.equals(2.0, unit::meter));

  test -= number(100.0, unit::centimeter);
  REQUIRE(test.equals(1.0, unit::meter));

  test.assign(2.0, unit::meter);
  test *= 2.0;
  REQUIRE(test.equals(4.0, unit::meter));

  test.assign(6.0, unit::meter);
  test /= 2.0;
  REQUIRE(test.equals(3.0, unit::meter));

  test.assign(-10, unit::meter);
  test %= 3.0;
  REQUIRE(test.equals(-1.0, unit::meter));

  REQUIRE((-test).equals(1.0, unit::meter));

  REQUIRE((++test).equals(0.0, unit::meter));
  REQUIRE(test.equals(0.0, unit::meter));

  REQUIRE((--test).equals(-1.0, unit::meter));
  REQUIRE(test.equals(-1.0, unit::meter));

  REQUIRE(test++.equals(-1.0, unit::meter));
  REQUIRE(test.equals(0.0, unit::meter));

  REQUIRE(test--.equals(0.0, unit::meter));
  REQUIRE(test.equals(-1.0, unit::meter));
}
