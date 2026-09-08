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

TEST_CASE("small storage: parse integers")
{
  const number n = number::parse("42");

  REQUIRE(n.equals(42));
  REQUIRE(n.compare(number(41)) > 0);
  REQUIRE(n.to_string() == "42");
}

TEST_CASE("small storage: parse integers with underscores")
{
  const number n = number::parse("15_000_000");

  REQUIRE(n.equals(15000000));
  REQUIRE(n.to_string() == "15000000");
}

TEST_CASE("small storage: overflow promotes")
{
  const auto a = number::parse("9223372036854775807");
  const auto b = number::parse("1");
  const auto c = a + b;

  REQUIRE(c.equals(number::parse("9223372036854775808")));
}

TEST_CASE("small storage: copy is cheap")
{
  const number original(42);
  const number copy(original);
  number assigned(0);

  assigned = original;

  REQUIRE(copy.equals(42));
  REQUIRE(assigned.equals(42));
}

TEST_CASE("small storage: move preserves value")
{
  number original(42);
  const number moved(std::move(original));

  REQUIRE(moved.equals(42));
}

TEST_CASE("small storage: fibonacci-like loop")
{
  number a(0);
  number b(1);

  for (int i = 0; i < 19; ++i)
  {
    const number c = a + b;

    a = b;
    b = c;
  }

  REQUIRE(b.equals(6765));
}

TEST_CASE("small storage: integer constructors")
{
  REQUIRE(number(0).equals(0.0));
  REQUIRE(number(std::int64_t{100}).equals(100.0));
  REQUIRE(number(1.0).equals(1.0));
}

TEST_CASE("small storage: increment and decrement")
{
  number n(41);

  REQUIRE((++n).equals(42));
  REQUIRE((n++).equals(42));
  REQUIRE(n.equals(43));
  REQUIRE((--n).equals(42));
  REQUIRE((n--).equals(42));
  REQUIRE(n.equals(41));
}

TEST_CASE("small storage: multiply stays small")
{
  REQUIRE((number(6) * number(7)).equals(42));
}

TEST_CASE("small storage: negate")
{
  REQUIRE((-number(42)).equals(-42));
  REQUIRE((-number(0)).equals(0));
}
