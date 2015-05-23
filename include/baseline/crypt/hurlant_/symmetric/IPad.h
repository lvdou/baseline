#ifndef CRYPT_HURLANT_SYMMETRIC_IPad_H
#define CRYPT_HURLANT_SYMMETRIC_IPad_H

#include "../../../include/crypt/hurlant/hurlant_base.h"

namespace crypt
{
    namespace hurlant
    {
        namespace symmetric
        {

            class IPad
            {
            public:
                virtual void pad(util::hex::ByteArray a)=0;
                virtual void unpad(util::hex::ByteArray a)=0;
                virtual void setBlockSize(std:size_t bs)=0;
            };

        };
    }
}
#endif // CRYPT_HURLANT_SYMMETRIC_H
