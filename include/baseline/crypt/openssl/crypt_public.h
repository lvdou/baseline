#ifndef CRYPT_OPENSSL_crypt_public_H
#define CRYPT_OPENSSL_crypt_public_H

//#include "../../include/crypt/openssl/crypt.h"
#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/dsa.h>
#include <openssl/dh.h>
#include <openssl/ec.h>
#include <openssl/rand.h>
#include <openssl/bio.h>
#include <openssl/engine.h>

namespace baseline
{
namespace crypt
{
	namespace openssl
	{
		using namespace crypt;
        enum type_cipher_base{cipher_S_AES=1,cipher_S_BLOWFISH=100,cipher_S_CAST,cipher_S_DES,cipher_S_IDEA,cipher_S_RC2,cipher_S_RC4,cipher_S_RC5,cipher_P_RSA=200,cipher_P_DSA,cipher_P_DH,cipher_P_EC,cipher_D_HMAC=300,cipher_D_MD2,cipher_D_MD4,cipher_D_MD5,cipher_D_MDC2,cipher_D_RIPEMD,cipher_D_SHA};

    class crypt_public
    {
    	private:
    	    EVP_PKEY_CTX* ctx_;
    		EVP_PKEY* pkey_;
    		std::size_t mode_;
    		std::size_t mode_id;
            char rnd_seed_[128];
            std::size_t bits_;
            unsigned long e_;
            int padding_;

        public:
            crypt_public():pkey_(NULL),ctx_(NULL),mode_(0),bits_(0),e_(0),padding_(0)
            {
            	OpenSSL_add_all_ciphers();
            }
            crypt_public(int mode_id, int bits, unsigned long e, int padding):pkey_(NULL),ctx_(NULL),mode_(0),bits_(0),e_(0),padding_(0)
            {
            	OpenSSL_add_all_ciphers();
            	if(!init(mode_id,bits,e,padding))
                {
                    if(pkey_) EVP_PKEY_free(pkey_);
                    if(ctx_)EVP_PKEY_CTX_free(ctx_);
                }
            }
            ~crypt_public()
            {
            	if(pkey_)EVP_PKEY_free(pkey_);
            	if(ctx_)EVP_PKEY_CTX_free(ctx_);
            }

            bool init(int mode_id, int bits, unsigned long e, int padding) //e = RSA_3, RSA_F4(65535)
            {
                reset();
                switch(mode_id)
                {
                case EVP_PKEY_DH:
                case EVP_PKEY_DHX:
                    mode_=cipher_P_DH;
                    break;
                case EVP_PKEY_DSA:
                case EVP_PKEY_DSA1:
                case EVP_PKEY_DSA2:
                case EVP_PKEY_DSA3:
                case EVP_PKEY_DSA4:
                    mode_=cipher_P_DSA;
                    break;
                case EVP_PKEY_EC:
                    mode_=cipher_P_EC;
                    break;
                case EVP_PKEY_RSA:
                case EVP_PKEY_RSA2:
                    mode_=cipher_P_RSA;
                    break;
                default:
                    mode_id=EVP_PKEY_RSA;
                    mode_=cipher_P_RSA;
                    break;
                }
                this->mode_id=mode_id;
                e_=e;
                bits_=bits;
                return true;
            }

            bool creat_key()
            {
                try
                {
                    if(!(ctx_ = EVP_PKEY_CTX_new_id(mode_id, NULL))) //such as mode_id=EVP_PKEY_RSA
                        throw "ctx create error!";;
                    if(!EVP_PKEY_keygen_init(ctx_)) throw "keygen init error!";
                    LOG_DEBUG<<"[crypt public] creat_key(): keygen init complete.";
                    if(!EVP_PKEY_CTX_set_rsa_keygen_bits(ctx_, bits_)) throw "set bits error!";
                    LOG_DEBUG<<"[crypt public] creat_key(): set bits complete.";
                    if(!EVP_PKEY_keygen(ctx_, &pkey_)) throw "keygen error!";
                    LOG_DEBUG<<"[crypt public] creat_key(): keygen complete.";
                    //if(!set_padding(ctx_, padding_)) throw "set padding error!";
                    //LOG_DEBUG<<"[crypt public] creat_key(): set padding complete.";
                }
                catch(std::exception& err)
                {
                    LOG_ERROR<<"[crypt public] creat_key(): "<<err.what();
                    reset();
                }
                catch(...)
                {
                    LOG_ERROR<<"[crypt public] creat_key(): "<<"unknown error";
                    reset();
                }
                return true;
            }

            bool set_padding(EVP_PKEY_CTX *ctx, int padding) //RSA_PKCS1_PADDING
            {
                switch(mode_)
                {
                case cipher_P_DH:
                    return true;
                    break;
                case cipher_P_DSA:
                    return true;
                    break;
                case cipher_P_RSA:
                default:
                    if(EVP_PKEY_CTX_set_rsa_padding(ctx, padding)) return true;
                }
                return false;
            }

            bool save_key_public_original(std::string& pathname)
            {
                BIO *out = NULL;
                FILE *fp;

                fp = fopen(pathname.c_str(), "w");
                if (!(out = BIO_new_fp(fp, BIO_NOCLOSE)))
                {
                    LOG_ERROR<<"[crypt public] save_key_public_original(): "<<"Error in new BIO";
                    return false;
                }

                BIO_set_flags(out, BIO_FLAGS_WRITE);
                if(!EVP_PKEY_print_public(out,pkey_,4, NULL))
                {
                    LOG_ERROR<<"[crypt public] save_key_public_original(): "<<"Error in save";
                    return false;
                }

                fclose(fp);
                BIO_free(out);
            }
            bool load_key_public_original(std::string& pathname)
            {

            }
            bool save_key_private_original(std::string& pathname)
            {
                BIO *out = NULL;
                FILE *fp;

                fp = fopen(pathname.c_str(), "w");
                if (!(out = BIO_new_fp(fp, BIO_NOCLOSE)))
                {
                    LOG_ERROR<<"[crypt public] save_key_private_original(): "<<"Error in new BIO";
                    return false;
                }

                BIO_set_flags(out, BIO_FLAGS_WRITE);
                if(!EVP_PKEY_print_private(out,pkey_,4, NULL))
                {
                    LOG_ERROR<<"[crypt public] save_key_private_original(): "<<"Error in save";
                    return false;
                }

                fclose(fp);
                BIO_free(out);
            }
            bool load_key_private_original(std::string& pathname);
            bool load_key_private_original(char* data, int len);

            bool encode(unsigned char *datain, std::size_t lenin, unsigned char **dataout, std::size_t &lenout)
            {
                if((!ctx_)||(!pkey_))
                {
                    LOG_ERROR<<"[crypt public] encode(): "<<"ctx or pkey not exist. ought to be init";
                    return false;
                }
                if(!EVP_PKEY_encrypt_init(ctx_))
                {
                    LOG_ERROR<<"[crypt public] encode(): "<<"encrypt init error.";
                    return false;
                }
                if(!set_padding(ctx_, padding_))
                {
                    LOG_ERROR<<"[crypt public] encode(): "<<"set padding error.";
                    return false;
                }
                if(!EVP_PKEY_encrypt(ctx_, NULL, &lenout, datain, lenin))  //Determine buffer length
                {
                    LOG_ERROR<<"[crypt public] encode(): "<<"calculate encode length error.";
                    return false;
                }
                if(!(*dataout = (unsigned char *)OPENSSL_malloc(lenout)))
                {
                    LOG_ERROR<<"[crypt public] encode(): "<<"malloc error.";
                    return false;
                }
                if(!EVP_PKEY_encrypt(ctx_, *dataout, &lenout, datain, lenin))
                {
                    LOG_ERROR<<"[crypt public] encode(): "<<"encode error.";
                    return false;
                }
                return true;
            }

            bool decode(unsigned char *datain, std::size_t lenin, unsigned char **dataout, std::size_t &lenout)
            {
                if((!ctx_)||(!pkey_))
                {
                    LOG_ERROR<<"[crypt public] decode(): "<<"ctx or pkey not exist. ought to be init";
                    return false;
                }
                if(!EVP_PKEY_decrypt_init(ctx_)) return false;
                if(!set_padding(ctx_, padding_)) return false;
                if(!EVP_PKEY_decrypt(ctx_, NULL, &lenout, datain, lenin)) //Determine buffer length
                {
                    LOG_ERROR<<"[crypt public] decode(): "<<"calculate decode length error.";
                    return false;
                }
                if(!(*dataout = (unsigned char *)OPENSSL_malloc(lenout)))
                {
                    LOG_ERROR<<"[crypt public] decode(): "<<"malloc error.";
                    return false;
                }
                if(!EVP_PKEY_decrypt(ctx_, *dataout, &lenout, datain, lenin))
                {
                    LOG_ERROR<<"[crypt public] decode(): "<<"decode error.";
                    return false;
                }
                return true;
            }

        int load_key_pem(std::string keypath="cert\\cet_rsa.pvt")
        {
            if((pfprivatekey = fopen(keypath.c_str(), "rb")) == NULL)
                return 0;
            if(prirsa=PEM_read_RSAPrivateKey(pfprivatekey, NULL, NULL, NULL))
            {
                fclose(pfprivatekey);
                if(RSA_check_key(prirsa)!= 1)
                    return 0;
                if(!(pkey_=EVP_PKEY_new()))
                {
                    LOG_ERROR<<"pkey_ not inited";
                    return 0;
                }
                if(!EVP_PKEY_assign_RSA(pkey_,prirsa))
                {
                    LOG_ERROR<<"assign RSA key to pkey eror!";
                    return 0;
                }
            }
            else
            {
                if(!load_key_public_pem(keypath))
                    return 0;
                if(!(pkey_=EVP_PKEY_new()))
                {
                    LOG_ERROR<<"pkey_ not inited";
                    return 0;
                }
                if(!EVP_PKEY_assign_RSA(pkey_,prirsa))
                {
                    LOG_ERROR<<"assign RSA key to pkey eror!";
                    return 0;
                }
            }
            if(!(ctx_ = EVP_PKEY_CTX_new(pkey_,NULL)))
            {
                LOG_ERROR<<"ctx_ not inited";
                return 0;
            }
            return 1;
        }

        protected:
            //RSA *prsa = RSA_new();
            RSA *pubrsa; //RSA *prsa = RSA_new();
            RSA *prirsa;
            FILE *pfprivatekey, *pfpublickey, *pfsnkey, *pfactivekey;

        private:
            unsigned char *iv;
            unsigned char* get_key()
            {
                //RSAPublicKey_dup、RSAPrivateKey_dup
            	if(pkey_)
                    return (unsigned char*)pkey_->pkey.ptr;
                else
                    return NULL;
            }
            void reset()
            {
                if(ctx_)
                {
                    EVP_PKEY_CTX* tctx_=ctx_;
                    ctx_=NULL;
                    EVP_PKEY_CTX_free(tctx_);
                }
                if(pkey_)
                {
                    EVP_PKEY* tkey_=pkey_;
                    pkey_=NULL;
                    EVP_PKEY_free(tkey_);
                }
            }
            bool rsa_check_pub_key(char * strin, int width)
            {
                if(!pubrsa)
                    return false;
                if((width>10)||(width<=0))
                    return false;
                char str2[10]={0};
                bool istrue=false;

                char *p = BN_bn2dec((pubrsa->n));
                //if (p)
                //    LOG_WARN<<"p number is "<<p; //1653298
                memcpy(str2,p,width);
                if(strcmp(str2,strin)==0)
                    istrue=true;

                OPENSSL_free(p);
                if(istrue)
                    return true;
                else
                    return false;
            }

            int load_key_public_pem(std::string keypath="cert\\cet_rsa.pub")
            {
                if((pfpublickey = fopen(keypath.c_str(), "rb")) == NULL)
                    return 0;
                if(!(pubrsa=PEM_read_RSAPublicKey(pfpublickey, NULL, NULL, NULL)))
                    return 0;
                fclose(pfpublickey);
                if(!rsa_check_pub_key("1653298",7))
                {
                    LOG_ERROR<<"pub key not correct!";
                    return 0;
                }
                return 1;
            }

    };
  }
}
}
#endif // CRYPT_OPENSSL_H
