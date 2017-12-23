/* Copyright (c) 2017 James E. King III
 * Use, modification and distribution is subject to the
 * Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)
 */
 
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/time_serialize.hpp>
#include <boost/lexical_cast.hpp>
#include "../testfrmwk.hpp"
#include <fstream>

using namespace boost;
using namespace posix_time;

void check_filesize(const char* filename, std::ifstream::pos_type expectedSize)
{
    std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
    check_equal("check file size is " + boost::lexical_cast<std::string>(expectedSize), in.tellg(), expectedSize);
}

int main() {
    const char *fname = "time_duration_serialization.v0";
    time_duration td(12, 13, 52, 123456);

#if BOOST_DATE_TIME_POSIX_TIME_DURATION_VERSION == 0
    std::ofstream ofs(fname, std::ios_base::binary | std::ios_base::out | std::ios_base::trunc);
    boost::archive::binary_oarchive oa(ofs);
    oa << td;
    ofs.close();

#if defined(_MSC_VER)
    check_filesize(fname, 62);
#endif
#else
    std::ifstream ifs(fname, std::ios_base::binary | std::ios_base::in);
    boost::archive::binary_iarchive ia(ifs);
    time_duration tmp;
    ia >> tmp;
    ifs.close();
    check_equal("read older version structure ok", td, tmp);

    std::ofstream ofs("time_duration_serialization.v1", std::ios_base::binary | std::ios_base::out | std::ios_base::trunc);
    boost::archive::binary_oarchive oa(ofs);
    oa << td;
    ofs.close();

#if defined(_MSC_VER)
    check_filesize("time_duration_serialization.v1", 74);
#endif
#endif

    return printTestStats();
}
