#include "../../include/crypt/hurlant/hurlant_base.h"
#include "../../include/crypt/hurlant/hurlant.h"

namespace crypt
{
    namespace hurlant
    {
        ICipher hurlant_crypt::getCipher(type::type_key name, util::hex::ByteArray key, IPad pad=NULL)
        {
            switch (keys[0]) {
				case "aes":
				case "aes128":
				case "aes192":
				case "aes256":
					keys.shift();
					if (key.length*8==keys[0]) {
						// support for "aes-128-..." and such.
						keys.shift();
					}
					return getMode(keys[0], new AESKey(key), pad);
				break;
				case "bf":
				case "blowfish":
					keys.shift();
					return getMode(keys[0], new BlowFishKey(key), pad);
				case "des":
					keys.shift();
					if (keys[0]!="ede" && keys[0]!="ede3")
                    {
						return getMode(keys[0], new DESKey(key), pad);
					}
					if (keys.length==1)
					{
						keys.push("ecb"); // default mode for 2tdes and 3tdes with openssl enc
					}
				case "3des":
				case "des3":
					keys.shift();
					return getMode(keys[0], new TripleDESKey(key), pad);
				case "xtea":
					keys.shift();
					return getMode(keys[0], new XTeaKey(key), pad);
				break;
				case "rc4":
					keys.shift();
					return new ARC4(key);
				break;
			}
			return null;
        }

        uint32_t getKeySize(type::type_key name)
        {
			switch (name)
			{
				case type::Key_simple:
					return 8; //???
				case type::Key_AES128:
					return 16;
				case type::Key_AES192:
					return 24;
				case type::Key_AES256:
					return 32;
				case type::Key_AES:
					return 16;
				case type::Key_BF:
				case type::Key_BLOWFISH:
					return 16;
				case type::Key_DES_EDE:
                    return 16;
                case type::Key_DES_EDE3:
                    return 24;
                case type::Key_DES:
                    return 8;
				case type::Key_3DES:
				case type::Key_DES3:
					return 24;
				case type::Key_XTEA:
					return 8;
				case type::Key_RC4:
					return 16; //???
                default:
					return 0; // why not.
			}
        }

        Ipad hurlant_crypt::getPad(std::string name)
        {
            switch(name)
            {
				case type::Pad_NULL:
					return new NullPad;
				case type::Pad_PKCS5:
				default:
					return new PKCS5;
			}
        }

        IHash hurlant_crypt::getHash(type::type_hash name)
        {
            switch(name)
            {
				case type::Hash_MD2 :
					return new MD2;
				case type::Hash_SHA: // let's hope you didn't mean sha-0
				case type::Hash_SHA1:
					return new SHA1;
				case type::Hash_SHA224:
					return new SHA224;
				case type::Hash_SHA256:
					return new SHA256;
                case type::Hash_MD5:
                default:
					return new MD5;
			}
			return null;
        }

        IMode getMode(type::type_mode name, ISymmetricKey alg, IPad padding=NULL)
        {
            switch (Mode_type)
            {
				case type::Mode_ECB:
					return new ECBMode(alg, padding);
				case type::Mode_CFB:
					return new CFBMode(alg, padding);
				case type::Mode_CFB8:
					return new CFB8Mode(alg, padding);
				case type::Mode_OFB:
					return new OFBMode(alg, padding);
				case type::Mode_CTR:
					return new CTRMode(alg, padding);
				case type::Mode_CBC:
				default:
					return new CBCMode(alg, padding);
			}
        }

        util::hex::ByteArray& toHex(std::string strin)
        {
            std::string shex = util::regex::remove(strin,"(\\s|:)+");
            util::hex::ByteArray a; //var a:ByteArray = new ByteArray;
             if(shex.length()%2) shex="0"+shex;
            for (int i=0;i<shex.length();i+=2)
            {
                int n=boost::lexical_cast<int>(shex.substr(i,2).c_str(), 2);
                a.write((char*)&n,2);
            }
            return a;
        }
    };

}
