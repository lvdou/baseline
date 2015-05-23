#ifndef BASELINE_UTIL_POOL_H
#define BASELINE_UTIL_POOL_H

#include <baseline/util/util_base.h>

#include <boost/pool/object_pool.hpp>
#include <boost/pool/pool.hpp>
#include <boost/pool/singleton_pool.hpp>

/* =====================



=======================*/

namespace baseline
{
namespace util
{
    namespace pool //: private boost::noncopyable
    {
        static boost::mutex io_mutex;
        struct pool_tag{};

        typedef boost::singleton_pool<pool_tag, 2> pool_2;
        typedef boost::singleton_pool<pool_tag, 4> pool_4;
        typedef boost::singleton_pool<pool_tag, 8> pool_8;
        typedef boost::singleton_pool<pool_tag, 16> pool_16;
        typedef boost::singleton_pool<pool_tag, 32> pool_32;
        typedef boost::singleton_pool<pool_tag, 64> pool_64;
        typedef boost::singleton_pool<pool_tag, 128> pool_128;
        typedef boost::singleton_pool<pool_tag, 2048> pool_2048;
        typedef boost::singleton_pool<pool_tag, 1024> pool_1024;
        typedef boost::singleton_pool<pool_tag, 512> pool_512;

        template<typename T>
        T malloc(std::size_t length)
        {
            boost::mutex::scoped_lock lock(io_mutex);
            T p;
            switch(length)
            {
            case 2:
                p=(T)pool_2::malloc();
                break;
            case 4:
                p=(T)pool_4::malloc();
                break;
            case 8:
                p=(T)pool_8::malloc();
                break;
            case 16:
                p=(T)pool_16::malloc();
                break;
            case 32:
                p=(T)pool_32::malloc();
                break;
            case 64:
                p=(T)pool_64::malloc();
                break;
            case 128:
                p=(T)pool_128::malloc();
                break;
            case 512:
                p=(T)pool_512::malloc();
                break;
            case 1024:
                p=(T)pool_1024::malloc();
                break;
            case 2048:
                p=(T)pool_2048::malloc();
                break;
            default:
                if(length<33)
                {
                    p=(T)pool_32::malloc();
                }
                else if(length<512)
                {
                    p=(T)pool_512::malloc();
                }
                else
                {
                    p=(T)pool_2048::malloc();
                }
                break;
            }
            return p;
        }

    }
}
}
#endif // UTIL_H
