#define BOOST_TEST_MODULE TestGroup4
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

using namespace boost::posix_time;
using namespace boost::gregorian;

const long TEST_TIME1 = 8211723004;
const long TEST_TIME2 = 42437106007;
const long TEST_TIME3 = 1018009440000;
const long TEST_TIME4 = 1052231280000;

//test37
BOOST_AUTO_TEST_CASE(testFactoryMinutes) {
    ptime test(date(1970, 1, 1), time_duration(0, 6, 0));

    BOOST_CHECK_EQUAL(6, test.time_of_day().minutes());
}

//test38
BOOST_AUTO_TEST_CASE(testMinutes) {
    ptime test(date(1970, 1, 1), time_duration(0, 1, 0));

    BOOST_CHECK_EQUAL(1, test.time_of_day().minutes());
}

//test39
BOOST_AUTO_TEST_CASE(testSeconds) {
    ptime test(date(1970, 1, 1), time_duration(0, 0, 1));

    BOOST_CHECK_EQUAL(1, test.time_of_day().seconds());
}

//test40
void check(const boost::gregorian::date& date, int year, int month, int day) {
    BOOST_CHECK_EQUAL(year, static_cast<int>(date.year()));
    BOOST_CHECK_EQUAL(month, static_cast<int>(date.month()));
    BOOST_CHECK_EQUAL(day, static_cast<int>(date.day()));
}

BOOST_AUTO_TEST_CASE(testToDateTime_RI) {
    date base(2005, 6, 9);
    ptime dt(date(2002, 1, 3), time_duration(4, 5, 6, 7));

    ptime test = ptime(base, dt.time_of_day());
    check(base, 2005, 6, 9);
    ptime expected = dt;
    expected = ptime(date(2005, 6, 9), expected.time_of_day());

    BOOST_CHECK_EQUAL(expected, test);
}