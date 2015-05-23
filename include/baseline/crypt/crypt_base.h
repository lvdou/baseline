#ifndef BASELINE_CRYPT_BASE_H
#define BASELINE_CRYPT_BASE_H

#include <baseline/common.h>
#include <baseline/util/util.h>

#include <boost/lexical_cast.hpp>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/md5.h>
#include <openssl/aes.h>


/*----------------------------------

*.DER   二进制DER编码的证书
*.PEM   ASCII(Base64)编码的各种X.509证书
*.KEY   PCSK7、8、12的公钥和私钥，可以是DER、PEM编码
*.CER   CRT证书，微软型式，可以是DER、PEM编码
*.CRT   基本等同于CER证书。

-----------------------------------*/

namespace baseline
{
namespace crypt
{
    //enum crypt_lib{Lib_openssl=1,Lib_ActionScript_Hurlant,Lib_Java_Crypto};
    //enum type_mode{Mode_CBC=1,Mode_ECB,Mode_CFB,Mode_CFB8,Mode_OFB,Mode_CTR};
    //enum type_hash{Hash_MD5=1,Hash_MD2,Hash_SHA,Hash_SHA1,Hash_SHA224,Hash_SHA256,Hash_SHA512};
    //enum type_pad{Pad_NULL=1,Pad_PKCS5,Pad_PKCS7,Pad_PKCS12};
    //enum type_key_mode{Key_simple=1,Key_AES,Key_AES128,Key_AES192,Key_AES256,Key_BF,Key_BLOWFISH,Key_DES,Key_DES_EDE,Key_DES_EDE3,Key_3DES,Key_DES3,Key_XTEA,Key_RC4};
    //enum type_key_authority{Key_autho_symmetry,Key_autho_public,Key_autho_private};
    //enum type_work_mode{Work_chiper=1,Work_hash,Work_sign,Work_envelope};
    //enum type_cipher_base{cipher_S_AES=1,cipher_S_BLOWFISH=100,cipher_S_CAST,cipher_S_DES,cipher_S_IDEA,cipher_S_RC2,cipher_S_RC4,cipher_S_RC5,cipher_P_RSA=200,cipher_P_DSA,cipher_P_DH,cipher_P_EC,cipher_D_HMAC=300,cipher_D_MD2,cipher_D_MD4,cipher_D_MD5,cipher_D_MDC2,cipher_D_RIPEMD,cipher_D_SHA};

    struct _conf_cipher
    {
        char* o_envelope;   //秘钥信封
        char* o_chiper;     //加解密
        char* o_hash;       //摘要
        char* o_sign;       //签名
        char* u_publib_keys;
        char* u_private_keys;
        char* u_rand;
        char* u_pad;
        char* u_IV;
        char* u_CA;
    };

    class crypt
    {
    private:
        _conf_cipher config;

    public:
        util::Base64 Base64;

        crypt(){};
        crypt(std::string cipher_name){};
        virtual ~crypt(){};

        virtual bool init_work_mode(std::size_t type_work_mode)=0;
        //virtual bool set_options(std::size_t option_type, )=0;

    protected:
        virtual bool set_cipher(std::string name)=0;
        virtual bool set_hash()=0;
        virtual bool set_envelope()=0;
        virtual bool set_sign()=0;

    protected:
    private:
		int FLEN;
        int BITS;
        char rnd_seed[128];
    };

		class aes
		{
        public:
            enum {ecb, cbc, cfb128, cfb1, cfb8, ofb128, ctr128};
            void encrypt_to(unsigned char *datain, unsigned char *dataout, int bits);
            void decrypt_to(unsigned char *datain, unsigned char *dataout, int bits);
        private:
            char *key;
		};

		class rsa
		{
        public:
            enum padding_type{PKCS1_PADDING=1, SSLV23_PADDING, NO_PADDING, PKCS1_OAEP_PADDING, X931_PADDING, PKCS1_PSS_PADDING};
            int rsa_isset_key_public;
            int rsa_isset_key_private;

            int save_key_rsa_public(void);
            int save_key_rsa_private(void);
            int save_file(const char* filename, unsigned char* datain, int inlen, bool isshort);
            int load_key_rsa_public(void);
            int load_key_rsa_private(void);
            int load_key_RSA(void);
            bool check_pub_key(char * strin, int width);

            //encrypt
            int generate_key_sn(char *datain);
            int encrypt(unsigned char *datain, unsigned char *dataout);
            //decrypt
            int decrypt(unsigned char *datain, unsigned char *dataout);
            int generate_key_active(char *datain);

        private:
            RSA *prsa;
            RSA *pubrsa;
            RSA *prirsa;
            FILE *private_key, *public_key, *sn_key, *active_key;
            int padding;
		};

    //typedef boost::serialization::singleton<crypt> so_crypt_cipher;
}
}
#endif // CRYPT_H
