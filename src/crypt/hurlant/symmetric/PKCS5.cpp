#include "../../include/crypt/hurlant/symmetric/PKCS5.h"

namespace crypt
{
    namespace hurlant
    {
        namespace symmetric
        {
            PKCS::PKCS(uint32_t blockSize)
            {
                this.blockSize = blockSize;
            }

            void PKCS::pad(util::hex::ByteArray a)
            {
                uint32_t c = blockSize-a.length()%blockSize;
                for (std::size_t i=0;i<c;i++)
                {
                    a[a.length()] = c;
                }
            }

            void PKCS::unpad(util::hex::ByteArray a)
            {
                uint32_t c = a.length%blockSize;
                if (c!=0) throw new Error("PKCS#5::unpad: ByteArray.length isn't a multiple of the blockSize");
                c = a[a.length-1];
                for (std::size_t i=c;i>0;i--)
                {
                    uint32_t v = a[a.length-1];
                    a.length--;
                    if (c!=v) throw new Error("PKCS#5:unpad: Invalid padding value. expected ["+c+"], found ["+v+"]");
                }
            }

            void PKCS::setBlockSize(uint32_t bs)
            {
                blockSize = bs;
            }
        };
    }
}
