#ifndef BASELINE_UTIL_BASE64_H_INCLUDED
#define BASELINE_UTIL_BASE64_H_INCLUDED

#include <baseline/util/util_base.h>
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/transform_width.hpp>

namespace baseline
{
namespace util
{
    class Base64
    {
    public:
        Base64(){};
        virtual ~Base64(){};

        bool encode(const std::string& input, std::string& output)
        {
            boost::mutex::scoped_lock lock(io_mutex);
            std::stringstream result;
            copy(Base64EncodeIterator(input.begin()) , Base64EncodeIterator(input.end()), std::ostream_iterator<char>(result));
            size_t equal_count = (3 - input.length() % 3) % 3;
            for (size_t i = 0; i < equal_count; i++) {
                result.put('=');
            }
            output = result.str();
            return output.empty() == false;
        }

        bool encode(unsigned char *datain, int inlen, char *dataout);

        bool decode(const std::string& input, std::string& output)
        {
            boost::mutex::scoped_lock lock(io_mutex);
            std::stringstream result;
            try
            {
                copy(Base64DecodeIterator(input.begin()) , Base64DecodeIterator(input.end()), std::ostream_iterator<char>(result));
            }
            catch(...)
            {
                return false;
            }
            output = result.str();
            return output.empty() == false;
        }

        bool decode(unsigned char *datain, char *dataout);

    private:
        boost::mutex io_mutex;
        typedef boost::archive::iterators::base64_from_binary<boost::archive::iterators::transform_width<std::string::const_iterator, 6, 8> > Base64EncodeIterator;
        typedef boost::archive::iterators::transform_width<boost::archive::iterators::binary_from_base64<std::string::const_iterator>, 8, 6> Base64DecodeIterator;
    };
    typedef boost::serialization::singleton<Base64> so_util_Base64;

}
}
#endif // CRYPT_BASE64_H_INCLUDED
