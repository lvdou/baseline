#ifndef BASELINE_UTIL_UUID_H
#define BASELINE_UTIL_UUID_H

#include <baseline/util/util_base.h>

#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/string_generator.hpp>

namespace baseline
{
namespace util
{
    namespace uuid //: private boost::noncopyable
    {
        static boost::mt19937 ran;
        static boost::uuids::string_generator sgen;
        //boost::uuids::basic_random_generator<boost::mt19937> gen(boost::mt19937 ran);
        static boost::mutex uuid_mutex_rand;

		std::string by_rand();
        std::string by_time();
        std::string by_name();
        std::string by_string();
        std::string by_null();

		std::size_t u_hash_single(const std::string& str);
		std::size_t u_hash_single();

        //boost::detail::spinlock lock;
    };

    //typedef boost::serialization::singleton<crc> so_util_crc;
    //typedef boost::serialization::singleton<b2c> so_util_b2c;
    //typedef boost::serialization::singleton<time> so_util_time;
}

}

#endif // UTIL_H
