// Copyright 2026 Treston Cuzzort
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or https://www.boost.org/LICENSE_1_0.txt)

#include <boost/date_time/posix_time/posix_time_duration.hpp>
#include <boost/core/lightweight_test.hpp>
#include <boost/cstdint.hpp>
#include <limits>

typedef boost::posix_time::time_duration duration;
typedef boost::int64_t integer;

duration ticks(integer value)
{
  return duration(0, 0, 0, value);
}

template<class Integer>
void check_scalar(Integer scalar)
{
  const integer value = static_cast<integer>(scalar);
  const duration original = ticks(10);
  BOOST_TEST_EQ((original * scalar).ticks(), 10 * value);
  BOOST_TEST_EQ(original.ticks(), 10);
  duration changed = original;
  BOOST_TEST_EQ((changed *= scalar).ticks(), 10 * value);
  BOOST_TEST_EQ(changed.ticks(), 10 * value);
  if (value != 0)
  {
    const duration dividend = ticks(-1234567890123LL);
    BOOST_TEST_EQ((dividend / scalar).ticks(), -1234567890123LL / value);
    BOOST_TEST_EQ(dividend.ticks(), -1234567890123LL);
    changed = dividend;
    BOOST_TEST_EQ((changed /= scalar).ticks(), -1234567890123LL / value);
    BOOST_TEST_EQ(changed.ticks(), -1234567890123LL / value);
  }

  const duration specials[] = {
    duration(boost::date_time::pos_infin),
    duration(boost::date_time::neg_infin),
    duration(boost::date_time::not_a_date_time)
  };
  for (unsigned i = 0; i < 3; ++i)
  {
    const duration expected = value == 0 || i == 2 ? specials[2]
      : specials[value < 0 ? 1 - i : i];
    BOOST_TEST(specials[i] * scalar == expected);
    changed = specials[i];
    BOOST_TEST((changed *= scalar) == expected);
    BOOST_TEST(changed == expected);
    if (value != 0)
    {
      BOOST_TEST(specials[i] / scalar == expected);
      changed = specials[i];
      BOOST_TEST((changed /= scalar) == expected);
      BOOST_TEST(changed == expected);
    }
  }
}

// Scalar bit patterns that are special values when stored in an adapter
// remain ordinary integers when used as multiplication/division operands.
template<class Integer>
void check_limits()
{
  typedef boost::date_time::int_adapter<Integer> adapter;
  const Integer values[] = {
    (std::numeric_limits<Integer>::min)(),
    (std::numeric_limits<Integer>::max)(),
    static_cast<Integer>((std::numeric_limits<Integer>::max)() - 1)
  };
  for (unsigned i = 0; i < 3; ++i)
  {
    const Integer scalar = values[i];
    // For unsigned adapters zero is the representation of negative infinity.
    if (std::numeric_limits<Integer>::is_signed)
      BOOST_TEST_EQ((adapter(0) * scalar).as_number(), 0);
    if (scalar != 0)
    {
      const adapter positive = adapter::pos_infinity();
      const adapter expected = scalar > 0 ? positive : adapter::neg_infinity();
      BOOST_TEST(positive * scalar == expected);
      BOOST_TEST(positive / scalar == expected);
      BOOST_TEST((adapter::not_a_number() * scalar).is_nan());
      BOOST_TEST((adapter::not_a_number() / scalar).is_nan());
      BOOST_TEST_EQ((adapter(10) / scalar).as_number(), 0);
    }
  }
  // The existing int overload must still retain the sign of a negative int
  // even when the adapter's own representation is unsigned.
  BOOST_TEST(adapter::pos_infinity() * -2 == adapter::neg_infinity());
  BOOST_TEST(adapter::pos_infinity() / -2 == adapter::neg_infinity());
}

int main()
{
  check_scalar(0);
  check_scalar(3);
  check_scalar(-3);
  check_scalar(static_cast<short>(-3));
  check_scalar(static_cast<unsigned short>(3));
  check_scalar(static_cast<signed char>(-3));
  check_scalar(static_cast<unsigned char>(3));
  check_scalar(true);
  check_scalar(false);
  check_scalar(3L);
  check_scalar(-3L);
  check_scalar(3UL);
  check_scalar(3LL);
  check_scalar(-3LL);
  check_scalar(3ULL);
  check_scalar(static_cast<boost::uint32_t>(2793555578UL));
  check_scalar(static_cast<boost::uint64_t>(2793555578ULL));
  check_scalar(static_cast<boost::uint64_t>(4294967297ULL));
  check_scalar(static_cast<boost::uint64_t>(1099511627779ULL));
  check_scalar(static_cast<integer>(-2793555578LL));
  check_scalar(static_cast<integer>(-4294967297LL));
  check_scalar(static_cast<integer>(-1099511627779LL));
  check_scalar(static_cast<integer>(2147483648LL));
  check_scalar(static_cast<integer>(-2147483648LL));

  check_limits<boost::int64_t>();
  check_limits<boost::uint64_t>();
  check_limits<int>();
  check_limits<unsigned>();

  const integer extremes[] = {
    (std::numeric_limits<integer>::min)(),
    (std::numeric_limits<integer>::max)()
  };
  for (unsigned i = 0; i < 2; ++i)
  {
    const integer scalar = extremes[i];
    duration value = ticks(0);
    BOOST_TEST_EQ((value * scalar).ticks(), 0);
    BOOST_TEST_EQ((value *= scalar).ticks(), 0);
    BOOST_TEST_EQ(value.ticks(), 0);
    value = ticks(10);
    BOOST_TEST_EQ((value / scalar).ticks(), 0);
    BOOST_TEST_EQ((value /= scalar).ticks(), 0);
    BOOST_TEST_EQ(value.ticks(), 0);
  }

  // Existing int member signatures remain usable by typed member pointers.
  typedef boost::date_time::time_duration<duration,
    boost::posix_time::time_res_traits> base_duration;
  duration (base_duration::*multiply)(int) const = &base_duration::operator*;
  duration (base_duration::*divide)(int) const = &base_duration::operator/;
  duration (base_duration::*multiply_assign)(int) = &base_duration::operator*=;
  duration (base_duration::*divide_assign)(int) = &base_duration::operator/=;
  duration value = ticks(10);
  BOOST_TEST_EQ((value.*multiply)(2).ticks(), 20);
  BOOST_TEST_EQ((value.*divide)(2).ticks(), 5);
  BOOST_TEST_EQ((value.*multiply_assign)(2).ticks(), 20);
  BOOST_TEST_EQ((value.*divide_assign)(2).ticks(), 10);

  return boost::report_errors();
}
