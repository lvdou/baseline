#ifndef CRYPT_HURLANT_H
#define CRYPT_HURLANT_H

#include "../../../include/crypt/crypt_base.h"
#include "../../../include/crypt/hurlant/symmetric/symmetric.h"


namespace crypt
{
    namespace hurlant
    {
        namespace type
        {
            enum type_mode{Mode_CBC=1,Mode_ECB,Mode_CFB,Mode_CFB8,Mode_OFB,Mode_CTR};
            enum type_hash{Hash_MD5=1,Hash_MD2,Hash_SHA,Hash_SHA1,Hash_SHA224,Hash_SHA256};
            enum type_pad{Pad_NULL=1,Pad_PKCS5};
            enum type_key{Key_simple=1,Key_AES,Key_AES128,Key_AES192,Key_AES256,Key_BF,Key_BLOWFISH,Key_DES,Key_DES_EDE,Key_DES_EDE3,Key_3DES,Key_DES3,Key_XTEA,Key_RC4};
        }

        class hurlant_crypt
        {
        public:
            hurlant_crypt(){};
            ICipher getCipher(type::type_key name, util::hex::ByteArray key, IPad pad=NULL);
            Ipad getPad(type::type_pad name);
            IHash getHash(type::type_hash name);
            uint32_t getKeySize(type::type_key name);

        private:
            Base64 b64;
            IMode getMode(type::type_mode name, ISymmetricKey alg, IPad padding=NULL);
        };

        util::hex::ByteArray& toHex(std::string strin);
    };

}
#endif // CRYPT_HURLANT_H
