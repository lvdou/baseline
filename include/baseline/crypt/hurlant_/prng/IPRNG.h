#ifndef CRYPT_HURLANT_PRNG_IPRNG_H
#define CRYPT_HURLANT_PRNG_IPRNG_H

#include "../../../include/crypt/hurlant/hurlant_base.h"
#include "../../../include/crypt/hurlant/prng/IPRNG.h"

namespace crypt
{
    namespace hurlant
    {
        namespace prng
        {
            class IPRNG
            {
            public:
                virtual std::size_t getPoolSize();
                virtual void init(util::hex::ByteArray key);
                virtual std::size_t next();
                virtual void dispose();
                virtual std::string toString();
            };
        }
    }
}
#endif // CRYPT_HURLANT_PRNG_H
