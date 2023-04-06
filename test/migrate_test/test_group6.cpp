#define BOOST_TEST_MODULE TestGroup6
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/test/included/unit_test.hpp> 
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <iostream>
#include <boost/algorithm/string.hpp>
#include <string>
#include <vector>
#include <boost/test/test_tools.hpp>

using namespace std;

using namespace boost::posix_time;
using namespace boost::gregorian;

const long TEST_TIME1 = 8211723004;
const long TEST_TIME2 = 42437106007;
const long TEST_TIME3 = 1018009440000;
const long TEST_TIME4 = 1052231280000;

//test51
BOOST_AUTO_TEST_CASE(testPropertyToIntervalYear) {

    ptime test(date(2004, 6, 9), time_duration(13, 23, 43, 53));

    date start_of_year = test.date() - date_duration(test.date().day_of_year() - 1);
    date end_of_year = start_of_year + years(1);

    BOOST_CHECK_EQUAL(start_of_year, date(2004, 1, 1));
    BOOST_CHECK_EQUAL(end_of_year, date(2005, 1, 1));
    BOOST_CHECK_EQUAL(test, ptime(date(2004, 6, 9), time_duration(13, 23, 43, 53)));
}


//test52
void check(const boost::gregorian::date& date, int year, int month, int day) {
    BOOST_CHECK_EQUAL(year, static_cast<int>(date.year()));
    BOOST_CHECK_EQUAL(month, static_cast<int>(date.month()));
    BOOST_CHECK_EQUAL(day, static_cast<int>(date.day()));
}

BOOST_AUTO_TEST_CASE(testPropertySetTextYear) {
    date test(1972, 6, 9);
    date copy(1970, test.month(), test.day());

    check(test, 1972, 6, 9);
    check(copy, 1970, 6, 9);
}

//test53
BOOST_AUTO_TEST_CASE(testPropertySetYear) {
    ptime test(date(2004, 6, 9), time_duration(0, 0, 0, 0));
    ptime copy = test;
    copy -= years(30);

    std::stringstream ss;
    ss << copy;

    BOOST_CHECK_EQUAL("1974-Jun-09 00:00:00", ss.str());
}


//test54
BOOST_AUTO_TEST_CASE(testPropertyAddYear) {
    ptime test(date(2004, 6, 9), time_duration(0, 0, 0, 0));
    ptime copy = test;
    copy += years(9);

    std::stringstream ss;
    ss << copy;

    BOOST_CHECK_EQUAL("2013-Jun-09 00:00:00", ss.str());
}


//test55
BOOST_AUTO_TEST_CASE(testPropertySetTextDayOfYear) {
    ptime test(date(2004, 6, 9), time_duration(0, 0, 0, 0));
    ptime copy = test;
    copy -= days(12);

    std::stringstream ss;
    ss << copy;

    BOOST_CHECK_EQUAL("2004-May-28 00:00:00", ss.str());
}

//test56
BOOST_AUTO_TEST_CASE(testPropertyAddDayOfYear) {
    ptime test(date(2004, 6, 9), time_duration(0, 0, 0, 0));

    BOOST_CHECK_EQUAL(12, test.date().day_of_week() + 9);
}

//test57
BOOST_AUTO_TEST_CASE(testPropertySetTextDayOfWeek) {
    ptime test(date(2004, 6, 9), time_duration(0, 0, 0, 0));

    date::day_of_week_type new_day_of_week1 = Monday; 
    ptime new_date1 = test + days(new_day_of_week1 - test.date().day_of_week());
    std::stringstream s;
    s << new_date1;
    BOOST_CHECK_EQUAL("2004-Jun-07 00:00:00", s.str());

    date::day_of_week_type new_day_of_week2 = Tuesday; 
    ptime new_date2 = test + days(new_day_of_week2 - test.date().day_of_week());
    std::stringstream ss;
    ss << new_date2;
    BOOST_CHECK_EQUAL("2004-Jun-08 00:00:00", ss.str());
}


//test58
BOOST_AUTO_TEST_CASE(testPropertySetTextDayOfWeek2) {
    ptime test(date(2004, 6, 9), time_duration(0, 0, 0, 0));

    date::day_of_week_type new_day_of_week1 = Monday; 
    ptime new_date1 = test + days(new_day_of_week1 - test.date().day_of_week());
    std::stringstream s;
    s << new_date1;
    BOOST_CHECK_EQUAL("2004-Jun-07 00:00:00", s.str());
}


//test59
BOOST_AUTO_TEST_CASE(testPropertySetTextDayOfWeek3) {
    ptime test(date(2004, 6, 9), time_duration(0, 0, 0, 0));

    date::day_of_week_type new_day_of_week1 = Tuesday; 
    ptime new_date1 = test + days(new_day_of_week1 - test.date().day_of_week());
    std::stringstream s;
    s << new_date1;
    BOOST_CHECK_EQUAL("2004-Jun-08 00:00:00", s.str());
}

//test60
BOOST_AUTO_TEST_CASE(testPropertyAddDayOfWeek) {
    ptime test(date(2004, 6, 9), time_duration(0, 0, 0, 0));

    BOOST_CHECK_EQUAL(4, test.date().day_of_week() + 1);
}