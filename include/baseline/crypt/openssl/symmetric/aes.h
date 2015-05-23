/*===============================

ECB模式: 按照块密码的块大小被分为数个块，并对每个块进行独立加密。
    优点：简单、易并行、误差不会被传送
    缺点：不隐藏明文、可能对明文进行主动攻击

CBC模式：每个明文块先与前一个密文块异或后，再进行加密。每个密文块都依赖于它前面的所有明文块。为保证每条消息的唯一性，在第一个块中需要初始化向量。
    优点：不容易主动攻击,安全性好于ECB，适合传输长度长的报文,是SSL、IPSec的标准。
    缺点：并行难、误差传递、需要初始化向量IV

CFB模式：类似于CBC，但可以将块密码变为自同步的流密码。
    优点：隐藏了明文、分组密码转化为流、可以及时加密传送小于分组（block size）的数据
    缺点：并行困难、误差传送、唯一的IV;

OFB模式：可将块密码变成同步的流密码。它产生密钥流的块，然后将其与明文块进行异或，得到密文。
    优点：隐藏明文、分组密码转化为流、可以及时加密传送小于分组的数据
    缺点：并行困难、对明文的主动攻击是可能的、误差传送

================================*/

/********************************
Example:

    unsigned char key[EVP_MAX_KEY_LENGHT] = "xc2x86x69x6dx88x7cx9axa0x61x1bxbbx3ex20x25xa4x5a";
    unsigned char iv[EVP_MAX_KEY_LENGHT] = "x56x2ex17x99x6dx09x3dx28xddxb3xbax69x5ax2ex6fx58";
    unsigned char msg[1024] = "1234567890abcdef";
    unsigned char out[1024] = {0};
    unsigned char dout2[1024] = {0};
    int outl, doutl;

    crypt::symmetric::aes aes(crypt::symmetric::options::CIPHER_AES_128_CBC,crypt::symmetric::options::PADDING_ENABLE);
    aes.load_key(key,strlen((char*)key),iv,strlen((char*)iv));

    aes.encode((unsigned char*)msg, DATA_LEN, (unsigned char*)out, outl);
    LOG_BASELINE_DEBUG<<"AES Encrypt final: "<<"bits="<<outl<<" is="<<util::hex::hex_to_string((char*)out,outl);

    aes.decode((unsigned char*)out, outl, (unsigned char*)dout2, doutl);
    LOG_BASELINE_DEBUG<<"AES Decrypt final: "<<"bits="<<doutl<<", is="<<dout2<<" hex="<<util::hex::hex_to_string((char*)dout2,doutl);

********************************/



#ifndef BASELINE_CRYPT_OPENSSL_crypt_AES_H
#define BASELINE_CRYPT_OPENSSL_crypt_AES_H

#include <baseline/crypt/openssl/crypt_base.h>
//#include "../../include/crypt/openssl/cert/pkcs.h"
#include <openssl/evp.h>
//#include <openssl/aes.h>

namespace baseline
{
namespace crypt
{
	namespace symmetric
	{
		using namespace crypt;
        namespace options
        {
            enum cipher_128{CIPHER_AES_128_ECB=1,CIPHER_AES_128_CBC,CIPHER_AES_128_CFB1, CIPHER_AES_128_CFB8,CIPHER_AES_128_CFB128,CIPHER_AES_128_CFB,CIPHER_AES_128_OFB,CIPHER_AES_128_CTR};
            enum cipher_192{CIPHER_AES_192_ECB=100,CIPHER_AES_192_CBC,CIPHER_AES_192_CFB1,CIPHER_AES_192_CFB8,CIPHER_AES_192_CFB128,CIPHER_AES_192_CFB,CIPHER_AES_192_OFB,CIPHER_AES_192_CTR};
            enum cipher_256{CIPHER_AES_256_ECB=200,CIPHER_AES_256_CBC,CIPHER_AES_256_CFB1, CIPHER_AES_256_CFB8,CIPHER_AES_256_CFB128,CIPHER_AES_256_CFB,CIPHER_AES_256_OFB,CIPHER_AES_256_CTR};
            enum {PADDING_DISABLE=0,PADDING_ENABLE}; //PADDING_ENABLE = PKCS#7 padding, PADDING_DISABLE = no padding
        }

    class aes
    {
    	private:
    	    EVP_CIPHER_CTX *ctx_;
    	    EVP_CIPHER *cipher_;
    		EVP_PKEY *pkey_;
    		unsigned char *key_;
            unsigned char *iv_;
            int padding_;

        public:
            aes();
            aes(int cipher, int enable_padding);
            ~aes();

            bool init(int cipher, int enable_padding);
            bool creat_key();
            bool set_padding(); //1=PKCS#7 padding, 0=no padding

            bool save_key(std::string& pathname);
            bool save_key(unsigned char* key, int &keylen, unsigned char *iv, int &ivlen);
            bool load_key(unsigned char* key, int keylen, unsigned char *iv, int ivlen);
            bool save_key(std::string &key, std::string &iv);
            void load_key(std::string &key, std::string &iv);

            bool encode(unsigned char *datain, int lenin, unsigned char *dataout, int &lenout);
            bool decode(unsigned char *datain, int lenin, unsigned char *dataout, int &lenout);

        private:
            void reset_key();
            void reset();
    };
  }
}
}
#endif // CRYPT_OPENSSL_H
