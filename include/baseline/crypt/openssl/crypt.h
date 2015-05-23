#ifndef BASELINE_CRYPT_OPENSSL_H
#define BASELINE_CRYPT_OPENSSL_H

#include <baseline/crypt/openssl/public/rsa.h>
#include <baseline/crypt/openssl/symmetric/aes.h>
#include <baseline/crypt/openssl/hash/hasher.h>
#include <baseline/crypt/openssl/cert/cert.h>
#include <baseline/crypt/openssl/cert/pkcs.h>

/*
namespace crypt
{
    class crypt_openssl: crypt
    {
        public:
            crypt_openssl();
            virtual ~crypt_openssl();
            bool init_work_mode(int type_work_mode);
            bool set_cipher(std::string name);
            bool set_hash();
            bool set_envelope();
            bool set_sign();

            bool create_key(int type_key_authority);
            bool check_key(int type_key_authority);

            bool save_key(std::string& filename, int type_key_authority);
            bool load_key(std::string& filename, int type_key_authority);
            bool load_key(char* data, int len, int type_key_authority);

            bool encode(unsigned char *datain, int lenin, unsigned char *dataout, int lenout);
            bool decode(unsigned char *datain, int lenin, unsigned char *dataout, int lenout);

        protected:
        private:
            EVP_CIPHER_CTX *ctx;
            unsigned char *key;
            unsigned char *iv;
    };
}

*/

#endif // CRYPT_OPENSSL_H
