#ifndef CRYPT_HURLANT_SYMMETRIC_IVMode_H
#define CRYPT_HURLANT_SYMMETRIC_IVMode_H

#include "../../../include/crypt/hurlant/hurlant_base.h"
#include "../../../include/crypt/hurlant/symmetric/IMode.h"
#include "../../../include/crypt/hurlant/symmetric/IPad.h"
#include "../../../include/crypt/hurlant/symmetric/ISymmetricKey.h"

namespace crypt
{
    namespace hurlant
    {
        namespace symmetric
        {
            class IVMode
            {
            public:
                IVMode(ISymmetricKey key, IPad padding=NULL);

                std::size_t getBlockSize();
                void dispose();
                void set_IV(util::hex::ByteArray value);
                util::hex::ByteArray get_IV();

            protected:
                ISymmetricKey key;
                IPad padding;
                Random prng;
                util::hex::ByteArray iv;
                util::hex::ByteArray lastIV;
                std::size_t blockSize;

                util::hex::ByteArray getIV4e();
                util::hex::ByteArray getIV4d();

            private:
            }
        };
    }
}
#endif // CRYPT_HURLANT_SYMMETRIC_H
