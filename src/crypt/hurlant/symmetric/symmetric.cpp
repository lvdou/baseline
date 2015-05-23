#include "../../../include/crypt/hurlant/symmetric/IVMode.h"

namespace crypt
{
    namespace hurlant
    {
        namespace symmetric
        {
            IVMode::IVMode(ISymmetricKey key, IPad padding=NULL)
            {
                this.key=key;
                blockSize=key.getBlockSize();
                if(padding==NULL)
                    padding=new PKCS5(blockSize);  //???
                else
                    padding.setBlockSize(blockSize);
                this.padding=padding;

                prng=new Random;  //???
                iv=NULL;
                lastIV=new util::hex::ByteArray;
            }

            std::size_t IVMode::getBlockSize()
            {
                return key.getBlockSize();
            }

            void IVMode::dispose()
            {
                std::size_t i;
                if (iv != null)
                {
                    for (i=0;i<iv.length;i++)
                    {
                        iv[i] = prng.nextByte();
                    }
                    iv.length=0;
                    iv = null;
                }

                if (lastIV != null)
                {
                    for (i=0;i<iv.length;i++)
                    {
                        lastIV[i] = prng.nextByte();
                    }
                    lastIV.length=0;
                    lastIV=null;
                }
                key.dispose();
                key = null;
                padding = null;
                prng.dispose();
                prng = null;
                Memory.gc();   //???
            }

            void IVMode::set_IV(util::hex::ByteArray value)
            {
                iv = value;
                lastIV.empty();
                lastIV.writeBytes(iv);
            }

            util::hex::ByteArray IVMode::get_IV()
            {
                return lastIV;
            }

            util::hex::ByteArray IVMode::getIV4e()
            {
                util::hex::ByteArray vec=new util::hex::ByteArray;
                if(iv)
                {
                    vec.writeBytes(iv);
                }
                else
                {
                    prng.nextBytes(vec, blockSize);
                }
                lastIV.length=0;
                lastIV.writeBytes(vec);
                return vec;
            }

            util::hex::ByteArray IVMode::getIV4d()
            {
                util::hex::ByteArray vec=new util::hex::ByteArray;
                if(iv)
                {
                    vec.writeBytes(iv);
                }
                else
                {
                    return NULL;
                }
                return vec;
            }
        };
    }
}
