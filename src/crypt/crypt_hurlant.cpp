#include "../../include/crypt/crypt_hurlant.h"

namespace crypt
{
    namespace hurlant
    {
        util::hex::ByteArray& toHex(std::string strin)
        {
            std::string hex = util::regex::remove(strin,"(\\s|:)+");
            util::hex::ByteArray a; //var a:ByteArray = new ByteArray;
            if (hex.length()&1==1) hex="0"+hex;
            for (int i=0;i<hex.length();i+=2)
            {
                int n=boost::lexical_cast<int>(hex.substr(i,2).c_str(), 2);
                a.write((char*)&n,2);
            }
            return a;
        }
    };

}
