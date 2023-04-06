#define BOOST_TEST_MODULE TestGroup1
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
// using namespace std;
// using namespace boost;

#include <boost/test/test_tools.hpp>

using namespace boost::posix_time;
using namespace boost::gregorian;

// 2230-Mar-22 02:10:04
const long TEST_TIME1 = 8211723004;
// 3314-Oct-12 05:00:07
const long TEST_TIME2 = 42437106007;

//test1
BOOST_AUTO_TEST_CASE(test_property_get_year)
{
    ptime test(date(1972, 6, 9), time_duration(10, 20, 30, 40));

    BOOST_CHECK_EQUAL(1972, test.date().year());
}

//test2
BOOST_AUTO_TEST_CASE(test_counted_time_system_compare_to_year)
{
    ptime test1 = from_time_t(TEST_TIME1);
    ptime test2 = from_time_t(TEST_TIME2);

    BOOST_CHECK(test1.date().year() < test2.date().year());
    BOOST_CHECK(test2.date().year() > test1.date().year());
    BOOST_CHECK(test1.date().year() == test1.date().year());
}


//test3
BOOST_AUTO_TEST_CASE(test_property_equals)
{
    ptime test1(date(2005, 11, 8), time_duration(10, 20, 30, 40));
    ptime test2(date(2005, 11, 9), time_duration(10, 20, 30, 40));
    
    BOOST_CHECK_EQUAL(false, test1.date().day() == test1.date().year());
    BOOST_CHECK_EQUAL(false, test1.date().day() == test1.date().month());
    BOOST_CHECK_EQUAL(true, test1.date().day() == test1.date().day());
    BOOST_CHECK_EQUAL(false, test1.date().day() == test2.date().year());
    BOOST_CHECK_EQUAL(false, test1.date().day() == test2.date().month());
    BOOST_CHECK_EQUAL(false, test1.date().day() == test2.date().day());
    
    BOOST_CHECK_EQUAL(false, test1.date().month() == test1.date().year());
    BOOST_CHECK_EQUAL(true, test1.date().month() == test1.date().month());
    BOOST_CHECK_EQUAL(false, test1.date().month() == test1.date().day());
    BOOST_CHECK_EQUAL(false, test1.date().month() == test2.date().year());
    BOOST_CHECK_EQUAL(true, test1.date().month() == test2.date().month());
    BOOST_CHECK_EQUAL(false, test1.date().month() == test2.date().day());
}


//test4
BOOST_AUTO_TEST_CASE(test_abs)
{
    BOOST_CHECK_EQUAL(246L, time_duration(milliseconds(246L)).abs().total_milliseconds());
    BOOST_CHECK_EQUAL(0L, time_duration(milliseconds(0L)).abs().total_milliseconds());
    BOOST_CHECK_EQUAL(246L, time_duration(milliseconds(-246L)).abs().total_milliseconds());
}


//test5
BOOST_AUTO_TEST_CASE(test_year)
{
    date test_date(2000, 1, 1);
    date another_test_date(2005, 1, 1);

    BOOST_CHECK_EQUAL(2000, test_date.year());
    BOOST_CHECK_EQUAL("2000", std::to_string(test_date.year()));
    BOOST_CHECK_EQUAL(2005, another_test_date.year());
}

//test6
BOOST_AUTO_TEST_CASE(testPropertySetTextDayOfYear)
{
    ptime test(date(2004, 6, 9), time_duration(0, 0, 0));
    date test_date = test.date();
    date copy_date = test_date;

    // set day of year 
    int target_day_of_year = 12;
    copy_date = date(copy_date.year(), 1, 1) + days(target_day_of_year - 1);
    ptime copy(copy_date, time_duration(0, 0, 0));

    // convert to str
    std::stringstream test_ss, copy_ss;
    test_ss << test;
    copy_ss << copy;

    BOOST_CHECK_EQUAL("2004-Jun-09 00:00:00", test_ss.str());
    BOOST_CHECK_EQUAL("2004-Jan-12 00:00:00", copy_ss.str());
}

//test7
BOOST_AUTO_TEST_CASE(testPropertyGetMaxMinValuesDayOfYear)
{
    ptime test(date(2004, 6, 9), time_duration(0, 0, 0));

    BOOST_CHECK_EQUAL(1, test.date().day_of_year().min());
    BOOST_CHECK_EQUAL(366, test.date().day_of_year().max());

    test = ptime(date(2002, 6, 9), time_duration(0, 0, 0));

    BOOST_CHECK_EQUAL(1, test.date().day_of_year().min());
    // BOOST_CHECK_EQUAL(365, test.date().day_of_year().max());
    BOOST_CHECK_EQUAL(366, test.date().day_of_year().max());
}

//test8
BOOST_AUTO_TEST_CASE(testPropertySetDayOfYear)
{
    ptime test(date(2004, 6, 9), time_duration(0, 0, 0));
    date copy = test.date() + days(12 - test.date().day_of_year());
    ptime copy_as_ptime(copy, time_duration(0, 0, 0));

    std::stringstream test_ss, copy_ss;
    test_ss << test;
    copy_ss << copy_as_ptime;

    BOOST_CHECK_EQUAL(test_ss.str(), "2004-Jun-09 00:00:00");
    BOOST_CHECK_EQUAL(copy_ss.str(), "2004-Jan-12 00:00:00");
}


//test9
BOOST_AUTO_TEST_CASE(testPropertyCompareToDayOfYear)
{
    ptime test1 = from_time_t(TEST_TIME1);
    ptime test2 = from_time_t(TEST_TIME2);
    BOOST_CHECK(test1.date().day_of_year() < test2.date().day_of_year());
    BOOST_CHECK(test2.date().day_of_year() > test1.date().day_of_year());
    BOOST_CHECK(test1.date().day_of_year() == test1.date().day_of_year());

    ptime dt1 = from_time_t(TEST_TIME1);
    ptime dt2 = from_time_t(TEST_TIME2);
    BOOST_CHECK(test1.date().day_of_year() < dt2.date().day_of_year());
    BOOST_CHECK(test2.date().day_of_year() > dt1.date().day_of_year());
    BOOST_CHECK(test1.date().day_of_year() == dt1.date().day_of_year());
}


//test10
BOOST_AUTO_TEST_CASE(test_dayOfYear)
{
    date d1(2004, 6, 9);

    BOOST_CHECK_EQUAL(d1.day_of_year(), d1.day_of_year());

    // Check if the day of the year is correct
    BOOST_CHECK_EQUAL(d1.day_of_year(), 161);

    BOOST_CHECK_EQUAL(d1.year(), 2004);
}