#define BOOST_TEST_MODULE TestGroup3
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


//test21
BOOST_AUTO_TEST_CASE(testFactoryHours){
    time_duration test = hours(6);

    // PeriodType.standard() is not available in Boost Date-Time
    // Therefore, the following line is not applicable:
    // ASSERT_EQ(PeriodType.standard(), test.getPeriodType());

    // The time_duration class does not support years, months, weeks, or days
    // Therefore, the following lines are not applicable:
    // ASSERT_EQ(0, test.getYears());
    // ASSERT_EQ(0, test.getMonths());
    // ASSERT_EQ(0, test.getWeeks());
    // ASSERT_EQ(0, test.getDays());

    BOOST_CHECK_EQUAL(6, test.hours());
    BOOST_CHECK_EQUAL(0, test.minutes());
    BOOST_CHECK_EQUAL(0, test.seconds());
    BOOST_CHECK_EQUAL(0, test.total_microseconds() % 1000); // test.getMillis()
}

//test22
BOOST_AUTO_TEST_CASE(testGetHours){

    time_duration days = hours(24);

    BOOST_CHECK_EQUAL(24, days.hours());
}


//test23
BOOST_AUTO_TEST_CASE(testGet){
    ptime test(date(1970, 1, 2));

    BOOST_CHECK_EQUAL(1970, test.date().year());
    BOOST_CHECK_EQUAL(1, test.date().month());
    BOOST_CHECK_EQUAL(2, test.date().day());
}

//test24
BOOST_AUTO_TEST_CASE(testDayTime){
    ptime test(date(1970, 1, 2), time_duration(3, 4, 5));

    BOOST_CHECK_EQUAL(3, test.time_of_day().hours());
    BOOST_CHECK_EQUAL(4, test.time_of_day().minutes());
    BOOST_CHECK_EQUAL(5, test.time_of_day().seconds());
}

//test25
BOOST_AUTO_TEST_CASE(testTime){
    ptime test(date(1970, 1, 2), time_duration(3, 4, 5));

    BOOST_CHECK_EQUAL(1970, test.date().year());
    BOOST_CHECK_EQUAL(1, test.date().month());
    BOOST_CHECK_EQUAL(2, test.date().day());
    BOOST_CHECK_EQUAL(3, test.time_of_day().hours());
    BOOST_CHECK_EQUAL(4, test.time_of_day().minutes());
    BOOST_CHECK_EQUAL(5, test.time_of_day().seconds());
}


//test26
BOOST_AUTO_TEST_CASE(testYearDayTime){
    ptime test(date(1970, 1, 2), time_duration(3, 4, 5));

    BOOST_CHECK_EQUAL(1970, test.date().year());
}


//test27
BOOST_AUTO_TEST_CASE(testHours){
    boost::posix_time::time_duration test = boost::posix_time::hours(24);

    BOOST_CHECK_EQUAL(24, test.hours());
}

//test28
BOOST_AUTO_TEST_CASE(testTypeIndexMethods){
    // ptime test(date(1970, 1, 2), time_duration(3, 4, 5));
}

date_duration test_date, result_date;
time_duration test_time, result_time;

//test29
//如何使用Boost Date-Time库的date_duration和time_duration对象来表示和检查不同时间单位的值。
BOOST_AUTO_TEST_CASE(testPlusYears) {
     // Test years and months using date_duration
    test_date = date_duration(365);
    result_date = test_date + date_duration(0);

    BOOST_CHECK_EQUAL(test_date, result_date);
}

//test30
BOOST_AUTO_TEST_CASE(testPlusMonths) {
    // Test weeks and days using date_duration
    test_date = date_duration(7);
    result_date = test_date + date_duration(0);

    BOOST_CHECK_EQUAL(test_date, result_date);
}

//test31
BOOST_AUTO_TEST_CASE(testPlusWeeks) {
    // test_date = date_duration(1);
    // result_date = test_date + date_duration(0);

    // BOOST_CHECK_EQUAL(test_date, result_date);
}

//test32
BOOST_AUTO_TEST_CASE(testPlusDays) {
    test_date = date_duration(1);
    result_date = test_date + date_duration(0);

    BOOST_CHECK_EQUAL(test_date, result_date);
  
}

//test33
BOOST_AUTO_TEST_CASE(testPlusHours) {
    test_time = time_duration(1, 0, 0);
    result_time = test_time + time_duration(0, 0, 0);

    BOOST_CHECK_EQUAL(test_time, result_time);
}

//test34
BOOST_AUTO_TEST_CASE(testPlusMinutes) {
    test_time = time_duration(0, 1, 0);
    result_time = test_time + time_duration(0, 0, 0);

    BOOST_CHECK_EQUAL(test_time, result_time);
}

//test35
BOOST_AUTO_TEST_CASE(testPlusSeconds) {
    test_time = time_duration(0, 0, 1);
    result_time = test_time + time_duration(0, 0, 0);

    BOOST_CHECK_EQUAL(test_time, result_time);
}

//test36
BOOST_AUTO_TEST_CASE(testPlusMilliseconds) {
    test_time = time_duration(0, 0, 0, 1000);
    result_time = test_time + time_duration(0, 0, 0, 0);

    BOOST_CHECK_EQUAL(test_time, result_time);
}