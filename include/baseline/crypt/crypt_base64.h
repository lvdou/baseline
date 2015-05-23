#ifndef CRYPT_BASE64_H_INCLUDED
#define CRYPT_BASE64_H_INCLUDED

#include "../../include/common.h"
#include "../../include/util/util.h"
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/transform_width.hpp>

namespace crypt
{
    class Base64
    {
    public:
        Base64(){};
        virtual ~Base64(){};

        bool encode(const string& input, string& output)
        {
            boost::mutex::scoped_lock lock(io_mutex);
            stringstream result;
            copy(Base64EncodeIterator(input.begin()) , Base64EncodeIterator(input.end()), ostream_iterator<char>(result));
            size_t equal_count = (3 - input.length() % 3) % 3;
            for (size_t i = 0; i < equal_count; i++) {
                result.put('=');
            }
            output = result.str();
            return output.empty() == false;
        }

        bool decode(const string& input, string& output)
        {
            boost::mutex::scoped_lock lock(io_mutex);
            stringstream result;
            try
            {
                copy(Base64DecodeIterator(input.begin()) , Base64DecodeIterator(input.end()), ostream_iterator<char>(result));
            }
            catch(...)
            {
                return false;
            }
            output = result.str();
            return output.empty() == false;
        }

    private:
        boost::mutex io_mutex;
        typedef boost::archive::iterators::base64_from_binary<boost::archive::iterators::transform_width<string::const_iterator, 6, 8> > Base64EncodeIterator;
        typedef boost::archive::iterators::transform_width<boost::archive::iterators::binary_from_base64<string::const_iterator>, 8, 6> Base64DecodeIterator;
    };
    typedef boost::serialization::singleton<Base64> so_crypt_Base64;
}
#endif // CRYPT_BASE64_H_INCLUDED
