#include <baseline/crypt/openssl/public/rsa.h>
#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/rand.h>
#include <openssl/bio.h>
#include <openssl/engine.h>

namespace baseline
{
namespace crypt
{
	namespace publicing
	{
	    rsa::rsa():pkey_(NULL),ctx_(NULL),bits_(0),e_(0),padding_(0)
        {
            OpenSSL_add_all_ciphers();
        }

        rsa::~rsa()
        {
            if(pkey_)EVP_PKEY_free(pkey_);
            if(ctx_)EVP_PKEY_CTX_free(ctx_);
            delete pubkey;
            delete prikey;
            delete iv;
        }

        bool rsa::init(int mode, int bits, unsigned long e, int padding) //e = RSA_3, RSA_F4(65535)
        {
            reset();
            padding_=padding;
            mode_=mode;
            e_=e;
            bits_=bits;
            return true;
        }

        bool rsa::creat_key()
        {
            try
            {
                if(!(ctx_ = EVP_PKEY_CTX_new_id(mode_, NULL))) //such as mode_id=EVP_PKEY_RSA
                    throw "ctx create error!";
                if(!EVP_PKEY_keygen_init(ctx_)) throw "keygen init error!";
                LOG_BASELINE_DEBUG<<"[crypt public] creat_key(): keygen init complete.";
                if(!EVP_PKEY_CTX_set_rsa_keygen_bits(ctx_, bits_)) throw "set bits error!";
                LOG_BASELINE_DEBUG<<"[crypt public] creat_key(): set bits complete.";
                if(!EVP_PKEY_keygen(ctx_, &pkey_)) throw "keygen error!";
                LOG_BASELINE_DEBUG<<"[crypt public] creat_key(): keygen complete.";
            }
            catch(std::exception& err)
            {
                LOG_BASELINE_ERROR<<"[crypt public] creat_key(): "<<err.what();
                reset();
            }
            catch(...)
            {
                LOG_BASELINE_ERROR<<"[crypt public] creat_key(): "<<"unknown error";
                reset();
            }
            return true;
        }

        bool rsa::save_key_public_original(std::string& pathname)
        {
            BIO *out = NULL;
            FILE *fp;

            fp = fopen(pathname.c_str(), "w");
            if (!(out = BIO_new_fp(fp, BIO_NOCLOSE)))
            {
                LOG_BASELINE_ERROR<<"[crypt public] save_key_public_original(): "<<"Error in new BIO";
                return false;
            }

            BIO_set_flags(out, BIO_FLAGS_WRITE);
            if(!EVP_PKEY_print_public(out,pkey_,4, NULL))
            {
                LOG_BASELINE_ERROR<<"[crypt public] save_key_public_original(): "<<"Error in save";
                return false;
            }

            fclose(fp);
            BIO_free(out);
        }

        bool rsa::save_key_private_original(std::string& pathname)
        {
            BIO *out = NULL;
            FILE *fp;

            fp = fopen(pathname.c_str(), "w");
            if (!(out = BIO_new_fp(fp, BIO_NOCLOSE)))
            {
                LOG_BASELINE_ERROR<<"[crypt public] save_key_private_original(): "<<"Error in new BIO";
                return false;
            }

            BIO_set_flags(out, BIO_FLAGS_WRITE);
            if(!EVP_PKEY_print_private(out,pkey_,4, NULL))
            {
                LOG_BASELINE_ERROR<<"[crypt public] save_key_private_original(): "<<"Error in save";
                return false;
            }

            fclose(fp);
            BIO_free(out);
        }


        bool rsa::encode(unsigned char *datain, std::size_t lenin, unsigned char **dataout, std::size_t &lenout)
        {
            if((!ctx_)||(!pkey_))
            {
                LOG_BASELINE_ERROR<<"[crypt public] encode(): "<<"ctx or pkey not exist. ought to be init";
                return false;
            }
            if(!EVP_PKEY_encrypt_init(ctx_))
            {
                LOG_BASELINE_ERROR<<"[crypt public] encode(): "<<"encrypt init error.";
                return false;
            }
            if(!set_padding())
            {
                LOG_BASELINE_ERROR<<"[crypt public] encode(): "<<"set padding error.";
                return false;
            }
            if(!EVP_PKEY_encrypt(ctx_, NULL, &lenout, datain, lenin))  //Determine buffer length
            {
                LOG_BASELINE_ERROR<<"[crypt public] encode(): "<<"calculate encode length error.";
                return false;
            }
            if(!(*dataout = (unsigned char *)OPENSSL_malloc(lenout+1)))
            {
                LOG_BASELINE_ERROR<<"[crypt public] encode(): "<<"malloc error.";
                return false;
            }
            if(!EVP_PKEY_encrypt(ctx_, *dataout, &lenout, datain, lenin))
            {
                LOG_BASELINE_ERROR<<"[crypt public] encode(): "<<"encode error.";
                return false;
            }
            (*dataout)[lenout]=0;
            //LOG_BASELINE_DEBUG<<"dout"<<**dataout;
            return true;
        }

        bool rsa::decode(unsigned char *datain, std::size_t lenin, unsigned char **dataout, std::size_t &lenout)
        {
            if((!ctx_)||(!pkey_))
            {
                LOG_BASELINE_ERROR<<"[crypt public] decode(): "<<"ctx or pkey not exist. ought to be init";
                return false;
            }
            if(!EVP_PKEY_decrypt_init(ctx_)) return false;
            if(!set_padding()) return false;
            if(!EVP_PKEY_decrypt(ctx_, NULL, &lenout, datain, lenin)) //Determine buffer length
            {
                LOG_BASELINE_ERROR<<"[crypt public] decode(): "<<"calculate decode length error.";
                return false;
            }
            if(!(*dataout = (unsigned char *)OPENSSL_malloc(lenout+1)))
            {
                LOG_BASELINE_ERROR<<"[crypt public] decode(): "<<"malloc error.";
                return false;
            }
            if(!EVP_PKEY_decrypt(ctx_, *dataout, &lenout, datain, lenin))
            {
                LOG_BASELINE_ERROR<<"[crypt public] decode(): "<<"decode error.";
                return false;
            }
            (*dataout)[lenout]=0;
            //LOG_BASELINE_DEBUG<<"dout"<<**dataout;
            return true;
        }

        bool rsa::encode_public(unsigned char *datain, std::size_t lenin, unsigned char **dataout, std::size_t &lenout)
        {

        }

        int rsa::load_key_pem()
        {
            FILE *pfprivatekey;
            if((pfprivatekey = fopen("cert\\cet_rsa.pvt", "rb")) == NULL)
                return 0;
            if(prikey=PEM_read_RSAPrivateKey(pfprivatekey, NULL, NULL, NULL))
            {
                fclose(pfprivatekey);
                delete pfprivatekey;
                if(RSA_check_key(prikey)!= 1)
                    return 0;
                if(!(pkey_=EVP_PKEY_new()))
                {
                    LOG_BASELINE_ERROR<<"pkey_ not inited";
                    return 0;
                }
                if(!EVP_PKEY_assign_RSA(pkey_,prikey))
                {
                    LOG_BASELINE_ERROR<<"assign RSA key to pkey eror!";
                    return 0;
                }
            }
            else
            {
                if(!load_key_public_pem())
                    return 0;
                if(!(pkey_=EVP_PKEY_new()))
                {
                    LOG_BASELINE_ERROR<<"pkey_ not inited";
                    return 0;
                }
                if(!EVP_PKEY_assign_RSA(pkey_,prikey))
                {
                    LOG_BASELINE_ERROR<<"assign RSA key to pkey eror!";
                    return 0;
                }
            }
            if(!(ctx_ = EVP_PKEY_CTX_new(pkey_,NULL)))
            {
                LOG_BASELINE_ERROR<<"ctx_ not inited";
                return 0;
            }
            return 1;
        }

        unsigned char* rsa::get_key()
        {
            if(pkey_)
                return (unsigned char*)pkey_->pkey.ptr;
            else
                return NULL;
        }

        void rsa::reset()
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

        bool rsa::check_pub_key(char * strin, int width)
        {
            if(!pubkey)
                return false;
            if((width>10)||(width<=0))
                return false;
            char str2[10]={0};
            bool istrue=false;

            char *p = BN_bn2dec((pubkey->n));
            memcpy(str2,p,width);
            if(strcmp(str2,strin)==0)
                istrue=true;

            OPENSSL_free(p);
            if(istrue)
                return true;
            else
                return false;
        }

        int rsa::load_key_public_pem()
        {
            FILE *pfpublickey;
            if((pfpublickey = fopen("cert\\cet_rsa.pvt", "rb")) == NULL)
                return 0;
            if(!(pubkey=PEM_read_RSAPublicKey(pfpublickey, NULL, NULL, NULL)))
                return 0;
            fclose(pfpublickey);
            delete pfpublickey;
            if(!check_pub_key("1653298",7))
            {
                LOG_BASELINE_ERROR<<"pub key not correct!";
                return 0;
            }
            return 1;
        }

        bool rsa::set_padding() //RSA_PKCS1_PADDING
        {
            if(EVP_PKEY_CTX_set_rsa_padding(ctx_, padding_))
                return true;
            return false;
        }
    }
}
}
