#ifndef CRYPT_HURLANT_SYMMETRIC_ICipher_H
#define CRYPT_HURLANT_SYMMETRIC_ICipher_H

#include "../../../include/crypt/hurlant/hurlant_base.h"

namespace crypt
{
    namespace hurlant
    {
        namespace symmetric
        {
            class ICipher
            {
            public:
                virtual std::size_t getBlockSize()=0;
                virtual void encrypt(util::hex::ByteArray src)=0;
                virtual void decrypt(util::hex::ByteArray src)=0;
                virtual void dispose()=0;
                virtual std::string toString()=0;
            };
        }
    }
}
#endif // CRYPT_HURLANT_SYMMETRIC_H
