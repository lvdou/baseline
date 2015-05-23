/******************************
example:

    char_a test_mall()
    {
        char_a p(new char[128]);
        strcpy(p.get(),"reset i=0,loop back.");
        return p;
    }

    void main()
    {
        char_a a1 = test_mall();
        LOG_BASELINE_DEBUG << "malloc  :" << a1.get();
        a1[5]=0x00;
        LOG_BASELINE_DEBUG << "malloc  :" << a1.get();
    }

******************************/


#ifndef BASELINE_UTIL_BASE_H
#define BASELINE_UTIL_BASE_H

#include <baseline/common.h>
#include <baseline/util/util_log.h>
#include <baseline/util/util_ByteArray.h>

#include <boost/format.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/noncopyable.hpp>
//#include <boost/shared_ptr.hpp>
//#include <boost/thread/mutex.hpp>
#include <boost/functional/hash.hpp>
#include <boost/pool/object_pool.hpp>

#include <boost/pool/pool.hpp>
#include <boost/pool/singleton_pool.hpp>
#include<boost/pool/pool_alloc.hpp>

#define char_a baseline::util::share_array

namespace baseline
{
namespace util
{
    typedef boost::shared_array<char> share_array;

    static std::size_t is_bits(){return sizeof(std::size_t) * 8;}

    template <typename T>
    static std::size_t  hash( const T& x)
    {
        boost::hash<T> hasher;
        return hasher(x);
    }

    static std::string nullstring("");

    //pool
    //struct pool_tag{};
    //typedef boost::pool<boost::default_user_allocator_new_delete> pool_memory;
   // typedef boost::object_pool<struct pool_tag, boost::default_user_allocator_new_delete> pool_object;
    //typedef boost::singleton_pool<struct pool_tag, sizeof(std::size_t), boost::default_user_allocator_new_delete> so_pool_object;
}
}
#endif // UTIL_H
