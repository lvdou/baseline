#ifndef CRYPT_OPENSSL_H
#define CRYPT_OPENSSL_H

#include "../../include/crypt/openssl/crypt_openssl.h"
#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/x509.h>
#include <openssl/des.h>

namespace crypt
{
	namespace openssl
	{
    class sign
    {
        public:
            sign();
            virtual ~sign();

            bool create_key(int type_key_authority);
            bool check_key(int type_key_authority);

            bool save_key(std::string& filename, int type_key_authority);
            bool load_key(std::string& filename, int type_key_authority);
            bool load_key(char* data, int len, int type_key_authority);

            bool encode(unsigned char *datain, int lenin, unsigned char *dataout, int lenout)
            {
            	
            }
            
            bool decode(unsigned char *datain, int lenin, unsigned char *dataout, int lenout);

        protected:
        private:
            EVP_CIPHER_CTX *ctx;
            unsigned char *key;
            unsigned char *iv;
    };
  }
}
#endif // CRYPT_OPENSSL_H
