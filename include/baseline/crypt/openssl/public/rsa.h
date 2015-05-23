
/********************************
Example:

    unsigned char msg[1024] = "1234567890abcdef";
    unsigned char* pout=NULL;
    unsigned char* dout=NULL;
    int plen=0, dlen=0;

    crypt::publicing::rsa rsa;
    rsa.init(crypt::publicing::rsa::MODE_CRYPT_RSA,2048,crypt::publicing::rsa::E_RSA_F4,crypt::publicing::rsa::PADDING_PKCS1);
    rsa.load_key_pem();

    rsa.encode((unsigned char*)msg, strlen((char*)msg),(unsigned char**)&pout,plen);
    LOG_BASELINE_DEBUG<<"encode "<<plen<<" bits, is:"<<util::hex::hex_to_text(pout,plen);

    rsa.decode((unsigned char*)pout,plen,(unsigned char**)&dout,dlen);
    LOG_BASELINE_DEBUG<<"decode "<<dlen<<" bits, is:"<<util::hex::hex_to_text(dout,dlen)<<" ori: "<<dout;

********************************/

#ifndef BASELINE_CRYPT_OPENSSL_crypt_public_RSA_H
#define BASELINE_CRYPT_OPENSSL_crypt_public_RSA_H

#include <baseline/crypt/openssl/crypt_base.h>
#include <openssl/evp.h>
#include <openssl/rsa.h>

namespace baseline
{
namespace crypt
{
	namespace publicing
	{
		using namespace crypt;
		//enum MODE_RSA{MODE_CRYPT_RSA=EVP_PKEY_RSA, MODE_CRYPT_RSA2=EVP_PKEY_RSA2};

        class rsa
        {
            private:
                EVP_PKEY_CTX* ctx_;
                EVP_PKEY* pkey_;
                std::size_t mode_;
                char rnd_seed_[128];
                std::size_t bits_;
                unsigned long e_;
                int padding_;

            public:
                enum MODE{MODE_CRYPT_RSA=EVP_PKEY_RSA, MODE_CRYPT_RSA2=EVP_PKEY_RSA2};
                enum PADDING{PADDING_NO=RSA_NO_PADDING, PADDING_PKCS1=RSA_PKCS1_PADDING, PADDING_PKCS1_OAEP=RSA_PKCS1_OAEP_PADDING, PADDING_SSLV23=RSA_SSLV23_PADDING, PADDING_X931=RSA_X931_PADDING};
                enum E{E_RSA_3=RSA_3, E_RSA_F4=RSA_F4};

                rsa();
                ~rsa();

                bool init(int mode, int bits, unsigned long e, int padding);
                bool creat_key();

                bool save_key_public_original(std::string& pathname);
                bool load_key_public_original(std::string& pathname);
                bool save_key_private_original(std::string& pathname);
                bool load_key_private_original(std::string& pathname);
                bool load_key_private_original(char* data, int len);

                bool encode(unsigned char *datain, std::size_t lenin, unsigned char **dataout, std::size_t &lenout);
                bool encode_public(unsigned char *datain, std::size_t lenin, unsigned char **dataout, std::size_t &lenout);
                bool decode(unsigned char *datain, std::size_t lenin, unsigned char **dataout, std::size_t &lenout);
                bool decode_public(unsigned char *datain, std::size_t lenin, unsigned char **dataout, std::size_t &lenout);

                int load_key_pem();

            protected:

            private:
                RSA *pubkey;
                RSA *prikey;
                unsigned char *iv;
                unsigned char* get_key();
                void reset();
                bool check_pub_key(char * strin, int width);
                int load_key_public_pem();
                bool set_padding();
        };
  }
}
}
#endif // CRYPT_OPENSSL_H
