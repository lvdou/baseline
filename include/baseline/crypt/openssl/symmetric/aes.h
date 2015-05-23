/*===============================

ECBģʽ: ���տ�����Ŀ��С����Ϊ�����飬����ÿ������ж������ܡ�
    �ŵ㣺�򵥡��ײ��С����ᱻ����
    ȱ�㣺���������ġ����ܶ����Ľ�����������

CBCģʽ��ÿ�����Ŀ�����ǰһ�����Ŀ������ٽ��м��ܡ�ÿ�����Ŀ鶼��������ǰ����������Ŀ顣Ϊ��֤ÿ����Ϣ��Ψһ�ԣ��ڵ�һ��������Ҫ��ʼ��������
    �ŵ㣺��������������,��ȫ�Ժ���ECB���ʺϴ��䳤�ȳ��ı���,��SSL��IPSec�ı�׼��
    ȱ�㣺�����ѡ����ݡ���Ҫ��ʼ������IV

CFBģʽ��������CBC�������Խ��������Ϊ��ͬ���������롣
    �ŵ㣺���������ġ���������ת��Ϊ�������Լ�ʱ���ܴ���С�ڷ��飨block size��������
    ȱ�㣺�������ѡ����͡�Ψһ��IV;

OFBģʽ���ɽ���������ͬ���������롣��������Կ���Ŀ飬Ȼ���������Ŀ������򣬵õ����ġ�
    �ŵ㣺�������ġ���������ת��Ϊ�������Լ�ʱ���ܴ���С�ڷ��������
    ȱ�㣺�������ѡ������ĵ����������ǿ��ܵġ�����

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
