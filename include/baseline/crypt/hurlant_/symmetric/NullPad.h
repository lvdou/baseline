#ifndef CRYPT_HURLANT_SYMMETRIC_NullPad_H
#define CRYPT_HURLANT_SYMMETRIC_NullPad_H

#include "../../../include/crypt/hurlant/hurlant_base.h"
#include "../../../include/crypt/hurlant/symmetric/IPad.h"

namespace crypt
{
    namespace hurlant
    {
        namespace symmetric
        {

            class NullPad: IPad
            {
            public:
                void pad(util::hex::ByteArray a)
                {
                	return;
                }
                void unpad(util::hex::ByteArray a)
                {
                	return;
                }
                void setBlockSize(std:size_t bs)
                {
                	return;
                }
            };

        };
    }
}
#endif // CRYPT_HURLANT_SYMMETRIC_H
