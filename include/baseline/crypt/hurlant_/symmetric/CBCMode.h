#ifndef CRYPT_HURLANT_SYMMETRIC_CBCMODE_H
#define CRYPT_HURLANT_SYMMETRIC_CBCMODE_H

#include "../../../include/crypt/hurlant/hurlant_base.h"
#include "../../../include/crypt/hurlant/symmetric/IMode.h"
#include "../../../include/crypt/hurlant/symmetric/IVMode.h"
#include "../../../include/crypt/hurlant/symmetric/IPad.h"
#include "../../../include/crypt/hurlant/symmetric/ISymmetricKey.h"

namespace crypt
{
    namespace hurlant
    {
        namespace symmetric
        {
            class CBCMode: IVMode,IMode
            {
            public:
                CBCMode(ISymmetricKey key, IPad padding=NULL);

                void encrypt(util::hex::ByteArray src);
                void decrypt(util::hex::ByteArray src);
								std::string toString();
            }
        };
    }
}
#endif // CRYPT_HURLANT_SYMMETRIC_H
