#ifndef CRYPT_HURLANT_PRNG_Random_H
#define CRYPT_HURLANT_PRNG_Random_H

#include "../../../include/crypt/hurlant/hurlant_base.h"
#include "../../../include/crypt/hurlant/prng/prng.h"

namespace crypt
{
    namespace hurlant
    {
        namespace prng
        {
            class Random
            {
            public:
                Random(IPRNG prng=NULL); //: i(0),j(0),psize(256);
                void seed(std::size_t x);
                void autoSeed();
                void nextBytes(util::hex::ByteArray buffer, std::size_t length);
                std::size_t nextByte();
                void dispose();
                std::string toString();

            private:
                IPRNG state;
                bool ready;
                util::hex::ByteArray pool;
                std::size_t psize;
                std::size_t pptr;
                bool seeded;

            };
        }
    }
}
#endif // CRYPT_HURLANT_PRNG_H
