#include "../../../include/crypt/hurlant/symmetric/CBCMode.h"

namespace crypt
{
    namespace hurlant
    {
        namespace symmetric
        {
            CBCMode::CBCMode(ISymmetricKey key, IPad padding=NULL)
            {
                super(key, padding);
            }

            void CBCMode::encrypt(util::hex::ByteArray src)
            {
                padding.pad(src);
                util::hex::ByteArray vector = getIV4e();
                for (var i:uint=0;i<src.length;i+=blockSize) {
                    for (var j:uint=0;j<blockSize;j++) {
                        src[i+j] ^= vector[j];
                    }
                    key.encrypt(src, i);
                    vector.position=0;
                    vector.writeBytes(src, i, blockSize);
                }
            }

            void CBCMode::decrypt(util::hex::ByteArray src)
            {
                util::hex::ByteArray vector = getIV4d();
                util::hex::ByteArray tmp = = new ByteArray;
                for (var i:uint=0;i<src.length;i+=blockSize) {
                    tmp.position=0;
                    tmp.writeBytes(src, i, blockSize);
                    key.decrypt(src, i);
                    for (var j:uint=0;j<blockSize;j++) {
                        src[i+j] ^= vector[j];
                    }
                    vector.position=0;
                    vector.writeBytes(tmp, 0, blockSize);
                }
                padding.unpad(src);
            }

            std::string toString()
            {
            	return key.toString()+"-cbc";
            }
        };
    }
}
