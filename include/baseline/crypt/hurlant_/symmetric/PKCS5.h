#ifndef CRYPT_HURLANT_SYMMETRIC_PKCS5_H
#define CRYPT_HURLANT_SYMMETRIC_PKCS5_H

#include "../../../include/crypt/hurlant/hurlant_base.h"
#include "../../../include/crypt/hurlant/symmetric/IPad.h"

namespace crypt
{
    namespace hurlant
    {
        namespace symmetric
        {

            class PKCS5: IPad
            {
            public:
                PKCS5();
                void pad(util::hex::ByteArray a);
                void unpad(util::hex::ByteArray a);
                void setBlockSize(uint32_t bs);
            };
            private:
                uint32_t blockSize;
        };
    }
}
#endif // CRYPT_HURLANT_SYMMETRIC_H
