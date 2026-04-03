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
#pragma once

#include <exception>
#include <optional>
#include <string>
#include <vector>

#include <mpfr.h>

namespace peelo
{
  /**
   * Big precision floating point decimal with optional measurement unit.
   */
  class number
  {
  public:
    /**
     * Exception type thrown when there measurement unit mismatch is
     * encountered.
     */
    class unit_error : public std::exception
    {
    public:
      unit_error(const std::string& message)
        : m_message(message) {}

      inline const char* what() const noexcept
      {
        return m_message.c_str();
      }

    private:
      std::string m_message;
    };

    /**
     * Representation of an measurement unit.
     */
    struct unit
    {
      /**
       * Enumeration of different supported types of measurement.
       */
      enum class type
      {
        length,
        mass,
        time,
      };

      // Supported length units.
      static const unit millimeter;
      static const unit centimeter;
      static const unit meter;
      static const unit kilometer;

      // Supported mass units.
      static const unit milligram;
      static const unit gram;
      static const unit kilogram;

      // Supported time units.
      static const unit millisecond;
      static const unit second;
      static const unit minute;
      static const unit hour;
      static const unit day;

      /** Type of the measurement unit. */
      enum type type;
      /** Measurement unit symbol. */
      std::string symbol;
      /** Multiplier amount. For base unit it would be 1. */
      int multiplier;

      /**
       * Searches for measurement unit based on it's symbol.
       */
      static std::optional<unit> find_by_symbol(const std::string& symbol);

      /**
       * Determines base unit of given type.
       */
      static const unit& base_unit_of(enum type type);

      /**
       * Returns container that has all units of given type.
       */
      static const std::vector<unit>& all_units_of(enum type type);

      /**
       * Tests whether this unit is the base unit of it's type.
       */
      inline bool is_base_unit() const
      {
        return multiplier == 1;
      }

      /**
       * Tests whether two measurement units are equal.
       */
      inline bool equals(const unit& that) const
      {
        return type == that.type && multiplier == that.multiplier;
      }

      /**
       * Equality testing operator.
       */
      inline bool operator==(const unit& that) const
      {
        return equals(that);
      }

      /**
       * Non-equality testing operator.
       */
      inline bool operator!=(const unit& that) const
      {
        return !equals(that);
      }
    };

    using value_type = mpfr_t;
    using unit_type = std::optional<struct unit>;
    using rounding_mode = mpfr_rnd_t;

    static constexpr rounding_mode default_rounding_mode = MPFR_RNDN;

    /**
     * Parses given string into number.
     */
    static number parse(
      const std::string& input,
      int base = 10,
      rounding_mode rounding = default_rounding_mode
    );

    /**
     * Constructs number representing infinity.
     */
    static number inf(const unit_type& unit = std::nullopt);

    /**
     * Constructs number representing NaN.
     */
    static number nan(const unit_type& unit = std::nullopt);

    /**
     * Constructs zero with given optional measurement unit.
     */
    number(const unit_type& unit = std::nullopt);

    /**
     * Constructs copy of existing number.
     */
    number(const number& that);

    /**
     * Constructs number instance from given double precision value and
     * optional measurement unit.
     */
    number(
      double value,
      const unit_type& unit = std::nullopt,
      rounding_mode rounding = default_rounding_mode
    );

    /**
     * Releases all resources allocated by this number instance.
     */
    ~number();

    /**
     * Returns the measurement unit used by this number instance.
     */
    inline const unit_type& measurement_unit() const
    {
      return m_unit;
    }

    /**
     * Constructs copy of the number without measurement unit.
     */
    number without_measurement_unit() const;

    /**
     * Tests whether the number represents infinity.
     */
    inline bool is_inf() const
    {
      return mpfr_inf_p(m_value);
    }

    /**
     * Tests whether the value represents NaN.
     */
    inline bool is_nan() const
    {
      return mpfr_nan_p(m_value);
    }

    /**
     * Converts the number into double precision value.
     *
     * \throws std::underflow_error If the number is too small to be fitted
     *                              into double precision.
     * \throws std::overflow_error If the number is too large to be fitted into
     *                             double precision.
     */
    explicit operator double() const;

    /**
     * Converts the number into long integer.
     *
     * \throws std::underflow_error If the number is too small to be fitted
     *                              into long integer.
     * \throws std::overflow_error If the number is too large to be fitted into
     *                             long integer.
     */
    explicit operator long() const;

    /**
     * Converts the number into boolean. Non-zero booleans are considered to be
     * trutful, while zero is false.
     */
    inline explicit operator bool() const noexcept
    {
      return mpfr_sgn(m_value) != 0;
    }

    /**
     * Negates boolean conversion. Returns true if the value is zero, false
     * otherwise.
     */
    inline bool operator!() const noexcept
    {
      return mpfr_sgn(m_value) == 0;
    }

    /**
     * Copies value and measurement of another number into this one.
     */
    number& assign(
      const number& that,
      rounding_mode rounding = default_rounding_mode
    );

    /**
     * Copies value of double precision into the number.
     */
    number& assign(
      double value,
      const unit_type& unit = std::nullopt,
      rounding_mode rounding = default_rounding_mode
    );

    /**
     * Copies value and measurement unit of another number into this one.
     */
    inline number& operator=(const number& that)
    {
      return assign(that);
    }

    /**
     * Copies value of double precision into the number. Measurement unit will
     * stay intact.
     */
    inline number& operator=(double value)
    {
      return assign(value);
    }

    /**
     * Tests whether two number instances are equal, with the same value and
     * measurement unit (or lack thereof).
     */
    bool equals(
      const number& that,
      rounding_mode rounding = default_rounding_mode
    ) const noexcept;

    /**
     * Tests whether number is equal with given double precision value, with
     * the same measurement unit (or lack thereof).
     */
    bool equals(
      double value,
      const unit_type& unit = std::nullopt,
      rounding_mode rounding = default_rounding_mode
    ) const noexcept;

    /**
     * Equality testing operator.
     */
    inline bool operator==(const number& that) const noexcept
    {
      return equals(that);
    }

    /**
     * Equality testing operator.
     */
    inline bool operator==(double value) const noexcept
    {
      return equals(value);
    }

    /**
     * Non-equality testing operator.
     */
    inline bool operator!=(const number& that) const noexcept
    {
      return !equals(that);
    }

    /**
     * Non-equality testing operator.
     */
    inline bool operator!=(double value) const noexcept
    {
      return !equals(value);
    }

    /**
     * Compares two numbers against each other.
     *
     * \throws unit_error If measurement units are not compatible.
     */
    int compare(
      const number& that,
      rounding_mode rounding = default_rounding_mode
    ) const;

    /**
     * Compares number against double precision value.
     *
     * \throws unit_error If measurement units are not compatible.
     */
    int compare(
      double value,
      const unit_type& unit = std::nullopt,
      rounding_mode rounding = default_rounding_mode
    ) const;

    /**
     * Less-than testing operator.
     */
    inline bool operator<(const number& that) const
    {
      return compare(that) < 0;
    }

    /**
     * Less-than testing operator.
     */
    inline bool operator<(double value) const
    {
      return compare(value) < 0;
    }

    /**
     * Greater-than testing operator.
     */
    inline bool operator>(const number& that) const
    {
      return compare(that) > 0;
    }

    /**
     * Greater-than testing operator.
     */
    inline bool operator>(double value) const
    {
      return compare(value) > 0;
    }

    /**
     * Less-than-or-equal testing operator.
     */
    inline bool operator<=(const number& that) const
    {
      return compare(that) <= 0;
    }

    /**
     * Less-than-or-equal testing operator.
     */
    inline bool operator<=(double value) const
    {
      return compare(value) <= 0;
    }

    /**
     * Greater-than-or-equal testing operator.
     */
    inline bool operator>=(const number& that) const
    {
      return compare(that) >= 0;
    }

    /**
     * Gerater-than-or-equal testing operator.
     */
    inline bool operator>=(double value) const
    {
      return compare(value) >= 0;
    }

    /**
     * Adds another number into this one and returns result.
     *
     * \throws unit_error If measurement units are not compatible.
     */
    number add(
      const number& that,
      rounding_mode rounding = default_rounding_mode
    ) const;

    /**
     * Adds double precision value into the number and returns result.
     *
     * \throws unit_error If measurement units are not compatible.
     */
    number add(
      double value,
      const unit_type& unit = std::nullopt,
      rounding_mode rounding = default_rounding_mode
    ) const;

    /**
     * Addition operator.
     *
     * \throws unit_error If measurement units are not compatible.
     */
    inline number operator+(const number& that) const
    {
      return add(that);
    }

    /**
     * Addition operator.
     *
     * \throws unit_error If measurement units are not compatible.
     */
    inline number operator+(double value) const
    {
      return add(value);
    }

    /**
     * Addition assignment operator.
     *
     * \throws unit_error If measurement units are not compatible.
     */
    inline number& operator+=(const number& that)
    {
      return assign(add(that));
    }

    /**
     * Addition assignment operator.
     *
     * \throws unit_error If measurement units are not compatible.
     */
    inline number& operator+=(double value)
    {
      return assign(add(value));
    }

    /**
     * Substracts another number from this one and returns result.
     *
     * \throws unit_error If measurement units are not compatible.
     */
    number substract(
      const number& that,
      rounding_mode rounding = default_rounding_mode
    ) const;

    /**
     * Substracts double precision value from the number and returns result.
     *
     * \throws unit_error If measurement units are not compatible.
     */
    number substract(
      double value,
      const unit_type& unit = std::nullopt,
      rounding_mode rounding = default_rounding_mode
    ) const;

    /**
     * Substraction operator.
     *
     * \throws unit_error If measurement units are not compatible.
     */
    inline number operator-(const number& that) const
    {
      return substract(that);
    }

    /**
     * Substraction operator.
     */
    inline number operator-(double value) const
    {
      return substract(value);
    }

    /**
     * Substraction assignment operator.
     */
    inline number& operator-=(const number& that)
    {
      return assign(substract(that));
    }

    /**
     * Substraction assignment operator.
     */
    inline number& operator-=(double value)
    {
      return assign(substract(value));
    }

    /**
     * Multiplies number with another one and returns result.
     *
     * \throws unit_error If measurement units are not compatible.
     */
    number multiply(
      const number& that,
      rounding_mode rounding = default_rounding_mode
    ) const;

    /**
     * Multiplies number with double precision value and returns result.
     *
     * \throws unit_error If measurement units are not compatible.
     */
    number multiply(
      double value,
      const unit_type& unit = std::nullopt,
      rounding_mode rounding = default_rounding_mode
    ) const;

    /**
     * Multiplication operator.
     *
     * \throws unit_error If measurement units are not compatible.
     */
    inline number operator*(const number& that) const
    {
      return multiply(that);
    }

    /**
     * Multiplication operator.
     */
    inline number operator*(double value) const
    {
      return multiply(value);
    }

    /**
     * Multiplication assignment operator.
     */
    inline number& operator*=(const number& that)
    {
      return assign(multiply(that));
    }

    /**
     * Multiplication assignment operator.
     */
    inline number& operator*=(double value)
    {
      return assign(multiply(value));
    }

    /**
     * Divides the number with another one and returns result.
     *
     * \throws unit_error If measurement units are not compatible.
     */
    number divide(
      const number& that,
      rounding_mode rounding = default_rounding_mode
    ) const;

    /**
     * Divides the number with double precision value and returns result.
     *
     * \throws unit_error If measurement units are not compatible.
     */
    number divide(
      double value,
      const unit_type& unit = std::nullopt,
      rounding_mode rounding = default_rounding_mode
    ) const;

    /**
     * Division operator.
     *
     * \throws unit_error If measurement units are not compatible.
     */
    inline number operator/(const number& that) const
    {
      return divide(that);
    }

    /**
     * Division operator.
     */
    inline number operator/(double value) const
    {
      return divide(value);
    }

    /**
     * Division assignment operator.
     */
    inline number& operator/=(const number& that)
    {
      return assign(divide(that));
    }

    /**
     * Division assignment operator.
     */
    inline number& operator/=(double value)
    {
      return assign(divide(value));
    }

    /**
     * Computes the floating-point remainder between this number and the other
     * one and returns result.
     */
    number modulo(
      const number& that,
      rounding_mode rounding = default_rounding_mode
    ) const;

    /**
     * Modulo operator.
     */
    inline number operator%(const number& that) const
    {
      return modulo(that);
    }

    /**
     * Modulo assignment operator.
     */
    inline number& operator%=(const number& that)
    {
      return assign(modulo(that));
    }

    /**
     * Negates sign of the number and returns result.
     */
    number negate(rounding_mode rounding = default_rounding_mode) const;

    /**
     * Negation operator.
     */
    inline number operator-() const
    {
      return negate();
    }

    /**
     * Prefix incrementation operator.
     */
    number& operator++();

    /**
     * Postfix decrementation operator.
     */
    number& operator--();

    /**
     * Postfix incrementation operator.
     */
    number operator++(int);

    /**
     * Postfix decrementation operator.
     */
    number operator--(int);

    // Exponential functions.
    number exp(rounding_mode rounding = default_rounding_mode) const;
    number exp2(rounding_mode rounding = default_rounding_mode) const;
    number expm1(rounding_mode rounding = default_rounding_mode) const;
    number log(rounding_mode rounding = default_rounding_mode) const;
    number log2(rounding_mode rounding = default_rounding_mode) const;
    number log10(rounding_mode rounding = default_rounding_mode) const;
    number log1p(rounding_mode rounding = default_rounding_mode) const;

    // Power functions.
    number pow(
      const number& exp,
      rounding_mode rounding = default_rounding_mode
    ) const;
    number sqrt(rounding_mode rounding = default_rounding_mode) const;
    number cbrt(rounding_mode rounding = default_rounding_mode) const;
    number hypot(
      const number& that,
      rounding_mode rounding = default_rounding_mode
    ) const;

    // Trigonometric functions.
    number cos(rounding_mode rounding = default_rounding_mode) const;
    number sin(rounding_mode rounding = default_rounding_mode) const;
    number tan(rounding_mode rounding = default_rounding_mode) const;
    number acos(rounding_mode rounding = default_rounding_mode) const;
    number asin(rounding_mode rounding = default_rounding_mode) const;
    number atan(rounding_mode rounding = default_rounding_mode) const;
    number atan2(
      const number& that,
      rounding_mode rounding = default_rounding_mode
    ) const;

    // Hyperbolic functions.
    number sinh(rounding_mode rounding = default_rounding_mode) const;
    number cosh(rounding_mode rounding = default_rounding_mode) const;
    number tanh(rounding_mode rounding = default_rounding_mode) const;
    number asinh(rounding_mode rounding = default_rounding_mode) const;
    number acosh(rounding_mode rounding = default_rounding_mode) const;
    number atanh(rounding_mode rounding = default_rounding_mode) const;

    std::string to_string(
      const std::string& format = "%.10R*g",
      rounding_mode rounding = default_rounding_mode
    ) const;

  private:
    /** Actual value of the number. */
    value_type m_value;
    /** Measurement unit used by the number. */
    unit_type m_unit;
  };

  /**
   * Returns textual description of measurement unit type.
   */
  std::string to_string(enum number::unit::type type);
}
