#include "../../../include/crypt/hurlant/prng/Random.h"


namespace crypt
{
    namespace hurlant
    {
        namespace prng
        {
            Random::Random(IPRNG prng=NULL): ready(false),seeded(false)
            {
                if (prng==null) prng = ARC4;
                state = new IPRNG;
                psize= state.getPoolSize();
                pool = new util::hex::ByteArray;
                pptr = 0;
                while (pptr <psize)
                {
                    std::size_t t = 65536*Math.random();
                    pool[pptr++] = t >>> 8;
                    pool[pptr++] = t&255;
                }
                pptr=0;
                seed();
            }

            void seed(std::size_t x)
            {
                if (x==0)
                {
                    x = new Date().getTime();
                }
                pool[pptr++] ^= x & 255;
                pool[pptr++] ^= (x>>8)&255;
                pool[pptr++] ^= (x>>16)&255;
                pool[pptr++] ^= (x>>24)&255;
                pptr %= psize;
                seeded = true;
            }

            void autoSeed()
            {
                util::hex::ByteArray b = new util::hex::ByteArray;
                b.writeUnsignedInt(System.totalMemory);
                b.writeUTF(Capabilities.serverString);
                b.writeUnsignedInt(getTimer());
                b.writeUnsignedInt((new Date).getTime());
                util::hex::ByteArray a = Font.enumerateFonts(true);
                for each (var f:Font in a) {
                    b.writeUTF(f.fontName);
                    b.writeUTF(f.fontStyle);
                    b.writeUTF(f.fontType);
                }
                b.position=0;
                while (b.bytesAvailable>=4)
                {
                    seed(b.readUnsignedInt());
                }
            }

            void nextBytes(util::hex::ByteArray buffer, std::size_t length)
            {
                while (length--)
                {
                    buffer.writeByte(nextByte());
                }
            }

            std::size_t nextByte()
            {
                if (!ready)
                {
                    if (!seeded)
                    {
                        autoSeed();
                    }
                    state.init(pool);
                    pool.length = 0;
                    pptr = 0;
                    ready = true;
                }
                return state.next();
            }

            void Random::dispose()
            {
                for (std::size_t i=0;i<pool.length;i++)
                {
                    pool[i] = Math.random()*256;

                }
                pool.length=0;
                pool = NULL;
                state.dispose();
                state = NULL;
                psize = 0;
                pptr = 0;
                //Memory.gc();
            }

            std::string Random::toString()
            {
                return "random-"+state.toString();
            }
        }
    }
}
