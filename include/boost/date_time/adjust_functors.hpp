#ifndef _DATE_TIME_ADJUST_FUNCTORS_HPP___
#define _DATE_TIME_ADJUST_FUNCTORS_HPP___

/* Copyright (c) 2002,2003 CrystalClear Software, Inc.
 * Use, modification and distribution is subject to the
 * Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)
 * Author: Jeff Garland, Bart Garst
 * $Date$
 */

#include <limits>
#include <boost/cstdint.hpp>
#include <boost/integer.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/type_traits/common_type.hpp>
#include <boost/type_traits/is_unsigned.hpp>
#include <boost/date_time/compiler_config.hpp>
#include <boost/date_time/date.hpp>
#include <boost/date_time/wrapping_int.hpp>
#include <boost/date_time/special_defs.hpp>

namespace boost {
namespace date_time {
namespace detail {

// The type trait promotes an unsigned integer type to the nearest larger
// signed integer type that can encompass all values of the original type
template< typename T, bool HasLargerType = (sizeof(T) < sizeof(boost::uintmax_t)) >
struct promote_to_signed
{
  typedef typename boost::int_t< std::numeric_limits< T >::digits * 2 >::exact type;
};

// This specialization is the best we can do if T cannot be promoted. Integer
// overflow can happen in this case.
template< typename T >
struct promote_to_signed< T, false >
{
  typedef boost::intmax_t type;
};

} // namespace detail

  //! Functor to iterate a fixed number of days
  template<class date_type>
  class day_functor
  {
  public:
    typedef typename date_type::duration_type duration_type;
    day_functor(int f) : f_(f) {}
    duration_type get_offset(const date_type& d) const
    {
      // why is 'd' a parameter???
      // fix compiler warnings
      d.year();
      return duration_type(f_);
    }
    duration_type get_neg_offset(const date_type& d) const
    {
      // fix compiler warnings
      d.year();
      return duration_type(-f_);
    }
  private:
    int f_;
  };


  //! Provides calculation to find next nth month given a date
  /*! This adjustment function provides the logic for 'month-based'
   *  advancement on a ymd based calendar.  The policy it uses
   *  to handle the non existant end of month days is to back
   *  up to the last day of the month.  Also, if the starting
   *  date is the last day of a month, this functor will attempt
   *  to adjust to the end of the month.

   */
  template<class date_type>
  class month_functor
  {
  public:
    typedef int difference_type;
    typedef typename date_type::duration_type duration_type;
    typedef typename date_type::calendar_type cal_type;
    typedef typename cal_type::ymd_type ymd_type;
    typedef typename cal_type::day_type day_type;
    typedef typename cal_type::month_type month_type;
    typedef typename cal_type::year_type year_type;

  private:
    typedef typename year_type::value_policy year_value_policy;
    typedef typename year_value_policy::value_type year_rep_type;
    typedef typename mpl::eval_if<
      is_unsigned<year_rep_type>,
      date_time::detail::promote_to_signed<year_rep_type>,
      mpl::identity<year_rep_type>
    >::type signed_year_rep_type;
    typedef typename boost::common_type<difference_type, signed_year_rep_type>::type year_common_type;
    typedef date_time::wrapping_int2<typename month_type::value_type,1,12> month_wrapping_int;

  public:
    month_functor(difference_type f) : f_(f), origDayOfMonth_(0) {}
    duration_type get_offset(const date_type& d) const
    {
      ymd_type ymd(d.year_month_day());
      if (origDayOfMonth_ == 0) {
        origDayOfMonth_ = ymd.day;
        day_type endOfMonthDay(cal_type::end_of_month_day(ymd.year,ymd.month));
        if (endOfMonthDay == ymd.day) {
          origDayOfMonth_ = -1; //force the value to the end of month
        }
      }
      month_wrapping_int wi(ymd.month);
      //calc the year wrap around, add() returns the number of wraps
      year_common_type year_offset = wi.add(static_cast< year_common_type >(f_));
      year_common_type year = year_offset + ymd.year; //calculate resulting year
      // We have to mimic the similar checks in constrained_value here because
      // the year value may exceed the range of year_rep_type and get truncated
      // when passed to the year_type constructor. By performing the checks on the full-sized
      // integers here we avoid integer overflow.
      if (BOOST_UNLIKELY(year + 1 < static_cast< year_common_type >((year_value_policy::min)() + 1))) {
        year_value_policy::on_error(ymd.year, static_cast< year_rep_type >(year_offset), CV::min_violation);
        return duration_type(not_a_date_time);
      }
      if (BOOST_UNLIKELY(year > static_cast< year_common_type >((year_value_policy::max)()))) {
        year_value_policy::on_error(ymd.year, static_cast< year_rep_type >(year_offset), CV::max_violation);
        return duration_type(not_a_date_time);
      }

      //find the last day for the new month
      day_type resultingEndOfMonthDay(cal_type::end_of_month_day(static_cast< year_rep_type >(year), wi.as_int()));
      //original was the end of month -- force to last day of month
      if (origDayOfMonth_ == -1) {
        return date_type(static_cast< year_rep_type >(year), wi.as_int(), resultingEndOfMonthDay) - d;
      }
      day_type dayOfMonth = origDayOfMonth_;
      if (dayOfMonth > resultingEndOfMonthDay) {
        dayOfMonth = resultingEndOfMonthDay;
      }
      return date_type(static_cast< year_rep_type >(year), wi.as_int(), dayOfMonth) - d;
    }
    //! Returns a negative duration_type
    duration_type get_neg_offset(const date_type& d) const
    {
      ymd_type ymd(d.year_month_day());
      if (origDayOfMonth_ == 0) {
        origDayOfMonth_ = ymd.day;
        day_type endOfMonthDay(cal_type::end_of_month_day(ymd.year,ymd.month));
        if (endOfMonthDay == ymd.day) {
          origDayOfMonth_ = -1; //force the value to the end of month
        }
      }
      month_wrapping_int wi(ymd.month);
      //calc the year wrap around, subtract() returns the number of wraps
      year_common_type year_offset = wi.subtract(static_cast< year_common_type >(f_));
      year_common_type year = year_offset + ymd.year; //calculate resulting year
      // We have to mimic the similar checks in constrained_value here because
      // the year value may exceed the range of year_rep_type and get truncated
      // when passed to the year_type constructor. By performing the checks on the full-sized
      // integers here we avoid integer overflow.
      if (BOOST_UNLIKELY(year + 1 < static_cast< year_common_type >((year_value_policy::min)() + 1))) {
        year_value_policy::on_error(ymd.year, static_cast< year_rep_type >(year_offset), CV::min_violation);
        return duration_type(not_a_date_time);
      }
      if (BOOST_UNLIKELY(year > static_cast< year_common_type >((year_value_policy::max)()))) {
        year_value_policy::on_error(ymd.year, static_cast< year_rep_type >(year_offset), CV::max_violation);
        return duration_type(not_a_date_time);
      }

      //find the last day for the new month
      day_type resultingEndOfMonthDay(cal_type::end_of_month_day(static_cast< year_rep_type >(year), wi.as_int()));
      //original was the end of month -- force to last day of month
      if (origDayOfMonth_ == -1) {
        return date_type(static_cast< year_rep_type >(year), wi.as_int(), resultingEndOfMonthDay) - d;
      }
      day_type dayOfMonth = origDayOfMonth_;
      if (dayOfMonth > resultingEndOfMonthDay) {
        dayOfMonth = resultingEndOfMonthDay;
      }
      return date_type(static_cast< year_rep_type >(year), wi.as_int(), dayOfMonth) - d;
    }

  private:
    difference_type f_;
    mutable short origDayOfMonth_;
  };


  //! Functor to iterate a over weeks
  template<class date_type>
  class week_functor
  {
  public:
    typedef typename date_type::duration_type duration_type;
    typedef typename date_type::calendar_type calendar_type;
    week_functor(int f) : f_(f) {}
    duration_type get_offset(const date_type& d) const
    {
      // why is 'd' a parameter???
      // fix compiler warnings
      d.year();
      return duration_type(f_*calendar_type::days_in_week());
    }
    duration_type get_neg_offset(const date_type& d) const
    {
      // fix compiler warnings
      d.year();
      return duration_type(-f_*calendar_type::days_in_week());
    }
  private:
    int f_;
  };

  //! Functor to iterate by a year adjusting for leap years
  template<class date_type>
  class year_functor
  {
  public:
    //typedef typename date_type::year_type year_type;
    typedef typename date_type::duration_type duration_type;
    year_functor(int f) : _mf(f * 12) {}
    duration_type get_offset(const date_type& d) const
    {
      return _mf.get_offset(d);
    }
    duration_type get_neg_offset(const date_type& d) const
    {
      return _mf.get_neg_offset(d);
    }
  private:
    month_functor<date_type> _mf;
  };


} }//namespace date_time


#endif

