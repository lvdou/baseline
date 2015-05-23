
#include <baseline/util/util_uuid.h>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

namespace baseline
{
namespace util
{
    namespace uuid
    {
        std::string by_rand()
        {
            boost::mutex::scoped_lock lock(uuid_mutex_rand);
            boost::uuids::basic_random_generator<boost::mt19937> gen(&ran);
            //boost::lock_guard<boost::detail::spinlock> guard(lock);
            boost::uuids::uuid u = gen();
            //usleep(1000);
            return  boost::uuids::to_string(u);
        }
        std::size_t u_hash_single(const std::string& str)
        {
            boost::hash<std::string> hasher;
            return hasher(str);
        }
        std::size_t u_hash_single()
        {
            boost::hash<std::string> hasher;
            return hasher(by_rand());
        }
    }

}

}
