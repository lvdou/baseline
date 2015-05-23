#ifndef CRYPT_HURLANT_PRNG_ARC4_H
#define CRYPT_HURLANT_PRNG_ARC4_H

#include "../../../include/crypt/hurlant/hurlant_base.h"
#include "../../../include/crypt/hurlant/prng/prng.h"
#include "../../../include/crypt/hurlant/symmetric/ICipher.h"

namespace crypt
{
    namespace hurlant
    {
        namespace prng
        {
            class ARC4: IPRNG, IStreamCipher
            {
            public:
                ARC4(util::hex::ByteArray key=NULL); //: i(0),j(0),psize(256);
                std::size_t getPoolSize();
                std::size_t getBlockSize();
                void init(util::hex::ByteArray key);
                std::size_t next();
                void dispose();
                std::string toString();

                void encrypt(util::hex::ByteArray block);
                void decrypt(util::hex::ByteArray block);

            private:
                std::size_t i;
                std::size_t j;
                util::hex::ByteArray S;
                std::size_t psize;
            };
        }
    }
}
#endif // CRYPT_HURLANT_PRNG_H
