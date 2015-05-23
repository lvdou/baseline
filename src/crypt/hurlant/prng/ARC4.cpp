#include "../../../include/crypt/hurlant/prng/ARC4.h"


namespace crypt
{
    namespace hurlant
    {
        namespace prng
        {
            ARC4::ARC4(util::hex::ByteArray key=NULL): i(0),j(0),psize(256)
            {
                S = new util::hex::ByteArray;
                if (key)
                {
                    init(key);
                }
            }

            std::size_t ARC4::getPoolSize()
            {
                return psize;
            }

            std::size_t ARC4::getBlockSize()
            {
                return 1;
            }

            void ARC4::init(util::hex::ByteArray key)
            {
                std::size_t i;
                std::size_t j;
                std::size_t t;
                for (i=0; i<256; ++i)
                {
                    S[i] = i;
                }
                j=0;
                for (i=0; i<256; ++i)
                {
                    j = (j + S[i] + key[i%key.length]) & 255;
                    t = S[i];
                    S[i] = S[j];
                    S[j] = t;
                }
                this.i=0;
                this.j=0;
            }

            std::size_t ARC4::next()
            {
                std::size_t t;
                i = (i+1)&255;
                j = (j+S[i])&255;
                t = S[i];
                S[i] = S[j];
                S[j] = t;
                return S[(t+S[i])&255];
            }

            void ARC4::encrypt(util::hex::ByteArray block)
            {
                std::size_t i = 0;
                while (i<block.length)
                {
                    block[i++] ^= next();
                }
            }

            void ARC4::decrypt(util::hex::ByteArray block)
            {
                encrypt(block);
            }

            void ARC4::dispose()
            {
                std::size_t i = 0;
                if (S!=null)
                {
                    for (i=0;i<S.length;i++)
                    {
                        S[i] = Math.random()*256;
                    }
                    S.length=0;
                    S = null;
                }
                this.i = 0;
                this.j = 0;
                Memory.gc();
            }

            std::string ARC4::toString()
            {
                return "rc4";
            }
        }
    }
}
