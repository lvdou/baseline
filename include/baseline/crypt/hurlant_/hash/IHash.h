#ifndef CRYPT_HURLANT_HASH_IHASH_H
#define CRYPT_HURLANT_HASH_IHASH_H

#include "../../../include/crypt/hurlant/hurlant_base.h"

namespace crypt
{
    namespace hurlant
    {
        namespace symmetric
        {

            class IHash
            {
            public:
                virtual uint32_t getInputSize()=0;
                virtual uint32_t getHashSize()=0;
                virtual util::hex::ByteArray hash(util::hex::ByteArray src)=0;
                virtual std::string toString()=0;
            };

        };
    }
}
#endif // CRYPT_HURLANT_SYMMETRIC_H
