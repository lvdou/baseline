#ifndef CRYPT_HURLANT_HASH_MD5_H
#define CRYPT_HURLANT_HASH_MD5_H

#include "../../../include/crypt/hurlant/hurlant_base.h"
#include "../../../include/crypt/hash/IHash.h"

namespace crypt
{
    namespace hurlant
    {
        namespace symmetric
        {
            class MD5: IHash
            {
            public:
            	static const int32_t HASH_SIZE=16;
            	std::size_t getInputSize(){return 64;}
            	std::size_t getHashSize(){return HASH_SIZE;}
            	util::hex::ByteArray hash(util::hex::ByteArray src);
              std::string toString(){return "md5";}
                	
            private:
            	char* core_md5(char* x, std::size_t len);
            };

        };
    }
}
#endif // CRYPT_HURLANT_SYMMETRIC_H
