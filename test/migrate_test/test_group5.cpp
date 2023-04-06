#define BOOST_TEST_MODULE TestGroup5
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


//test41
BOOST_AUTO_TEST_CASE(testWithField_DateTimeFieldType_int_1) {
    date test(date(2004, 6, 9));
    date result(date(2006, 6, 9));

    BOOST_CHECK_EQUAL(date(2004, 6, 9), test);
    BOOST_CHECK_EQUAL(date(2006, 6, 9), result);
}

date_duration test_date_duration;
time_duration test_time_duration;
date base_date(2002, 1, 1);

//test42
BOOST_AUTO_TEST_CASE(testPeriodStaticsYears) {
    test_date_duration = years(1).get_offset(base_date);

    BOOST_CHECK_EQUAL(test_date_duration, date_duration(365));
}

//test43
BOOST_AUTO_TEST_CASE(testPeriodStaticsMonths) {
    test_date_duration = months(1).get_offset(base_date);

    BOOST_CHECK_EQUAL(test_date_duration, date_duration(31));
}

//test44
BOOST_AUTO_TEST_CASE(testPeriodStaticsDays) {
    test_date_duration = days(1);

    BOOST_CHECK_EQUAL(test_date_duration, date_duration(1));
}

//test45
BOOST_AUTO_TEST_CASE(testPeriodStaticsHours) {
    test_time_duration = hours(1);

    BOOST_CHECK_EQUAL(test_time_duration, time_duration(1, 0, 0));
}

//test46
BOOST_AUTO_TEST_CASE(testPeriodStaticsMinutes) {
   test_time_duration = minutes(1);

    BOOST_CHECK_EQUAL(test_time_duration, time_duration(0, 1, 0));
}

//test47
BOOST_AUTO_TEST_CASE(testPeriodStaticsSeconds) {
    test_time_duration = seconds(1);

    BOOST_CHECK_EQUAL(test_time_duration, time_duration(0, 0, 1));
}

//test48
BOOST_AUTO_TEST_CASE(testPeriodStaticsMillisec) {
    test_time_duration = millisec(1);

    BOOST_CHECK_EQUAL(test_time_duration, time_duration(0, 0, 0, 1000));
}

//test49
BOOST_AUTO_TEST_CASE(testMinusFields) {
    time_duration test_time_duration;
    test_time_duration = hours(3) - hours(1);
    BOOST_CHECK_EQUAL(test_time_duration, hours(2));

    test_time_duration = minutes(3) - minutes(1);
    BOOST_CHECK_EQUAL(test_time_duration, minutes(2));

    test_time_duration = seconds(3) - seconds(1);
    BOOST_CHECK_EQUAL(test_time_duration, seconds(2));

    test_time_duration = milliseconds(3) - milliseconds(1);
    BOOST_CHECK_EQUAL(test_time_duration, milliseconds(2));
}

//test50
BOOST_AUTO_TEST_CASE(testPlusFields) {
    time_duration test_time_duration;
    test_time_duration = hours(3) + hours(1);
    BOOST_CHECK_EQUAL(test_time_duration, hours(4));

    test_time_duration = minutes(3) + minutes(1);
    BOOST_CHECK_EQUAL(test_time_duration, minutes(4));

    test_time_duration = seconds(3) + seconds(1);
    BOOST_CHECK_EQUAL(test_time_duration, seconds(4));

    test_time_duration = milliseconds(3) + milliseconds(1);
    BOOST_CHECK_EQUAL(test_time_duration, milliseconds(4));
}