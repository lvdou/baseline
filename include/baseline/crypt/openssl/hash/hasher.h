
/********************************
Example:

    unsigned char key[EVP_MAX_KEY_LENGHT] = "xc2x86x69x6dx88x7cx9axa0x61x1bxbbx3ex20x25xa4x5a";
    unsigned char iv[EVP_MAX_KEY_LENGHT] = "x56x2ex17x99x6dx09x3dx28xddxb3xbax69x5ax2ex6fx58";
    unsigned char msg[1024] = "1234567890abcdef";
    unsigned char out[1024] = {0};
    unsigned char dout2[1024] = {0};
    int outl, doutl;

    crypt::hashs::hasher md5(crypt::hashs::options::CIPHER_MD5);
    md5.init(crypt::hashs::options::CIPHER_SHA256);
    md5.encode((unsigned char*)msg, strlen(msg), (unsigned char*)out, outl);
    LOG_BASELINE_DEBUG << "MD5 " << outl << " bits, is:" << util::hex::hex_to_text(out,outl);

********************************/


#ifndef BASELINE_CRYPT_OPENSSL_crypt_HASHS_HASHER_H
#define BASELINE_CRYPT_OPENSSL_crypt_HASHS_HASHER_H

#include <baseline/crypt/openssl/crypt_base.h>
//#include "../../include/crypt/openssl/cert/pkcs.h"
//#include <openssl/md4.h>
//#include <openssl/md5.h>
//#include <openssl/mdc2.h>
//#include <openssl/sha.h>
#include <openssl/evp.h>

namespace baseline
{
namespace crypt
{
    namespace hashs
    {
        using namespace crypt;
        namespace options
        {
            enum cipher_hash {CIPHER_MD4=1, CIPHER_MD5, CIPHER_SHA, CIPHER_SHA1, CIPHER_DSS, CIPHER_DSS1, CIPHER_ECDSA, CIPHER_SHA224, CIPHER_SHA256, CIPHER_SHA384, CIPHER_SHA512, CIPHER_MDC2, CIPHER_RIPEMD160, CIPHER_WHIRLPOOL};
        }

        class hasher
        {
        private:
            EVP_MD_CTX *ctx_;
            EVP_MD *md_;

        public:
            hasher();
            hasher(int cipher);
            ~hasher();

            bool init(int cipher);
            bool encode(unsigned char *datain, std::size_t lenin, unsigned char *dataout, unsigned int &lenout);
            unsigned char* encode(unsigned char *datain, std::size_t lenin, unsigned int &lenout);
            bool encode(std::string& strin, std::string& strout);
            std::string encode(std::string& strin);

        private:
            void reset_ctx();
            void reset();
        };
    }
}
}
#endif // CRYPT_OPENSSL_H
