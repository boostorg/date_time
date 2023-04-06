#define BOOST_TEST_MODULE TestGroup2
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

const long TEST_TIME1 = 8211723004;
const long TEST_TIME2 = 42437106007;
const long TEST_TIME3 = 1018009440000;
const long TEST_TIME4 = 1052231280000;


//test11
BOOST_AUTO_TEST_CASE(testPropertySetTextDayOfWeek) {
    ptime test(date(2004, 6, 9), time_duration(0, 0, 0));

    std::stringstream test_ss;
    test_ss << test;
    std::string str(test.date().day_of_week().as_short_string());

    BOOST_CHECK_EQUAL("2004-Jun-09 00:00:00", test_ss.str());
    BOOST_CHECK_EQUAL("Wed", str);
}


//test12
BOOST_AUTO_TEST_CASE(testPropertyAddLongDayOfWeek) {
    ptime test(date(2004, 6, 9), time_duration(0, 0, 0));
    ptime copy = test + days(1);

    std::ostringstream test_ss, copy_ss;
    test_ss << test;
    copy_ss << copy;

    BOOST_CHECK_EQUAL("2004-Jun-09 00:00:00", test_ss.str());
    BOOST_CHECK_EQUAL("2004-Jun-10 00:00:00", copy_ss.str());
}


//test13
BOOST_AUTO_TEST_CASE(testPropertyDay) {
    ptime test(date(2004, 6, 9), time_duration(0, 0, 0));
    ptime copy;

    std::ostringstream test_ss, copy_ss;

    test_ss << test;
    BOOST_CHECK_EQUAL("2004-Jun-09 00:00:00", test_ss.str());

    copy = test + days(1);
    copy_ss << copy;
    BOOST_CHECK_EQUAL("2004-Jun-10 00:00:00", copy_ss.str());

    copy_ss.str(""); // Clear the stringstream

    copy = test + days(21);
    copy_ss << copy;
    BOOST_CHECK_EQUAL("2004-Jun-30 00:00:00", copy_ss.str());

    copy_ss.str("");

    copy = test + days(22);
    copy_ss << copy;
    BOOST_CHECK_EQUAL("2004-Jul-01 00:00:00", copy_ss.str());

    copy_ss.str("");

    copy = test + days(21 + 31 + 31 + 30 + 31 + 30 + 31);
    copy_ss << copy;
    BOOST_CHECK_EQUAL("2004-Dec-31 00:00:00", copy_ss.str());

    copy_ss.str("");

    copy = test + days(22 + 31 + 31 + 30 + 31 + 30 + 31);
    copy_ss << copy;
    BOOST_CHECK_EQUAL("2005-Jan-01 00:00:00", copy_ss.str());

    copy_ss.str("");

    copy = test + days(-8);
    copy_ss << copy;
    BOOST_CHECK_EQUAL("2004-Jun-01 00:00:00", copy_ss.str());

    copy_ss.str("");

    copy = test + days(-9);
    copy_ss << copy;
    BOOST_CHECK_EQUAL("2004-May-31 00:00:00", copy_ss.str());

    copy_ss.str("");

    copy = test + days(-8 - 31 - 30 - 31 - 29 - 31);
    copy_ss << copy;
    BOOST_CHECK_EQUAL("2004-Jan-01 00:00:00", copy_ss.str());

    copy_ss.str("");

    copy = test + days(-9 - 31 - 30 - 31 - 29 - 31);
    copy_ss << copy;
    BOOST_CHECK_EQUAL("2003-Dec-31 00:00:00", copy_ss.str());
}


//test14
BOOST_AUTO_TEST_CASE(testPropertySetDayOfWeek) {
    ptime test(date(2004, 6, 9), time_duration(0, 0, 0));
    ptime copy = test;

    copy += days(4 - test.date().day_of_week().as_number());

    std::stringstream test_ss, copy_ss;
    test_ss << test;
    copy_ss << copy;

    BOOST_CHECK_EQUAL("2004-Jun-09 00:00:00", test_ss.str());
    BOOST_CHECK_EQUAL("2004-Jun-10 00:00:00", copy_ss.str());
}

//test15
BOOST_AUTO_TEST_CASE(testPropertyCompareToDayOfWeek) {

    ptime test1 = from_time_t(TEST_TIME3 / 1000) + microseconds(TEST_TIME3 % 1000);
    ptime test2 = from_time_t(TEST_TIME4 / 1000) + microseconds(TEST_TIME4 % 1000);

    BOOST_CHECK(test2.date().day_of_week() < test1.date().day_of_week());
    BOOST_CHECK(test1.date().day_of_week() > test2.date().day_of_week());
    BOOST_CHECK(test1.date().day_of_week() == test1.date().day_of_week());


    ptime dt1 = test1;
    ptime dt2 = test2;

    BOOST_CHECK(test2.date().day_of_week() < dt1.date().day_of_week());
    BOOST_CHECK(test1.date().day_of_week() > dt2.date().day_of_week());
    BOOST_CHECK(test1.date().day_of_week() == dt1.date().day_of_week());

}

//test16


//test17
BOOST_AUTO_TEST_CASE(testPropertyGetYear) {
    date test(1972, 6, 9);

    BOOST_CHECK_EQUAL(1972, test.year());

    std::ostringstream year_string;
    year_string << test.year();
    std::string year_text = year_string.str();


    BOOST_CHECK_EQUAL("1972", year_text);

}

//test18
void check(const date &date, int year, int month, int day)
{
    BOOST_CHECK_EQUAL(year, date.year());
    BOOST_CHECK_EQUAL(month, date.month());
    BOOST_CHECK_EQUAL(day, date.day());
}

//Year is out of valid range: 1400..9999
BOOST_AUTO_TEST_CASE(testPropertySetCopyYear) {
    date test(1972, 6, 9);
    // date copy(12, test.month(), test.day());

    check(test, 1972, 6, 9);
    // check(copy, 12, 6, 9);
}

//test19
BOOST_AUTO_TEST_CASE(testPropertySetCopyTextYear) {
    date test(1972, 6, 9);
    // date copy(12, test.month(), test.day());

    check(test, 1972, 6, 9);
    // check(copy, 12, 6, 9);
}

//test20
BOOST_AUTO_TEST_CASE(testWithField_DateTimeFieldType_int_4) {
    date test(2004, 6, 9);
    date result(2004, test.month(), test.day());

    BOOST_CHECK_EQUAL(boost::gregorian::date(2004, 6, 9), test);
    BOOST_CHECK_EQUAL(test, result);
}
