#include "../../../include/crypt/openssl/crypt_openssl.h"
#include <openssl/evp.h>
//#include <openssl/pem.h>
//#include <openssl/md5.h>
//#include <openssl/rand.h>

namespace crypt
{
    crypt_openssl::crypt_openssl()
    {

    }

    crypt_openssl::~crypt_openssl()
    {
        EVP_CIPHER_CTX_cleanup(ctx);
    }

    bool crypt_openssl::init_work_mode(int type_work_mode)
    {
        if(ctx)
            EVP_CIPHER_CTX_cleanup(ctx);

        EVP_CIPHER_CTX_init(ctx);

        //EVP_CipherInit_ex(&ctx, EVP_aes_128_cbc(), NULL, NULL, NULL, do_encrypt);
        //OPENSSL_assert(EVP_CIPHER_CTX_key_length(&ctx) == 16);
        //OPENSSL_assert(EVP_CIPHER_CTX_iv_length(&ctx) == 16);
        //EVP_CipherInit_ex(&ctx, NULL, NULL, key, iv, do_encrypt);
    }

    bool crypt_openssl::set_cipher(std::string name)
    {

    }

    bool crypt_openssl::set_hash()
    {

    }

    bool crypt_openssl::set_envelope()
    {

    }

    bool crypt_openssl::set_sign()
    {

    }

    bool crypt_openssl::encode(unsigned char *datain, int lenin, unsigned char *dataout, int lenout)
    {
        EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, NULL, NULL);
        OPENSSL_assert(EVP_CIPHER_CTX_key_length(ctx) == 16);
        OPENSSL_assert(EVP_CIPHER_CTX_iv_length(ctx) == 16);
        EVP_EncryptInit_ex(ctx, NULL, NULL, key, iv);

        if(!EVP_EncryptUpdate(ctx, dataout, &lenout, datain, lenin))
        {
            return false;
        }
        if(!EVP_EncryptFinal_ex(ctx, dataout, &lenout))
        {
            return false;
        }
    }

    bool crypt_openssl::decode(unsigned char *datain, int lenin, unsigned char *dataout, int lenout)
    {
        EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, NULL, NULL);
        OPENSSL_assert(EVP_CIPHER_CTX_key_length(ctx) == 16);
        OPENSSL_assert(EVP_CIPHER_CTX_iv_length(ctx) == 16);
        EVP_DecryptInit_ex(ctx, NULL, NULL, key, iv);

        if(!EVP_DecryptUpdate(ctx, dataout, &lenout, datain, lenin))
        {
            return false;
        }
        if(!EVP_EncryptFinal_ex(ctx, dataout, &lenout))
        {
            return false;
        }
    }
}
