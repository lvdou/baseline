#include <baseline/crypt/openssl/symmetric/aes.h>

namespace baseline
{
namespace crypt
{
	namespace symmetric
	{
            aes::aes():pkey_(NULL),ctx_(NULL),padding_(0),key_(NULL),iv_(NULL)
            {
            	OpenSSL_add_all_ciphers();
            }
            aes::aes(int cipher, int enable_padding):pkey_(NULL),ctx_(NULL),padding_(0),key_(NULL),iv_(NULL)
            {
            	OpenSSL_add_all_ciphers();
            	init(cipher, enable_padding);
            }
            aes::~aes()
            {
            	if(pkey_)EVP_PKEY_free(pkey_);
            	if(ctx_)EVP_CIPHER_CTX_free(ctx_);
            	if(cipher_)OPENSSL_free(cipher_);
            	if(key_)OPENSSL_free(key_);
            	if(iv_)OPENSSL_free(iv_);
            }

            bool aes::init(int cipher, int enable_padding)
            {
                reset();
                switch(cipher)
                {
                case options::CIPHER_AES_128_ECB:
                    cipher_=(EVP_CIPHER*)EVP_aes_128_ecb();
                    break;
                case options::CIPHER_AES_128_CBC:
                    cipher_=(EVP_CIPHER*)EVP_aes_128_cbc();
                    break;
                case options::CIPHER_AES_128_CFB1:
                    cipher_=(EVP_CIPHER*)EVP_aes_128_cfb1();
                    break;
                case options::CIPHER_AES_128_CFB8:
                    cipher_=(EVP_CIPHER*)EVP_aes_128_cfb8();
                    break;
                case options::CIPHER_AES_128_CFB128:
                case options::CIPHER_AES_128_CFB:
                    cipher_=(EVP_CIPHER*)EVP_aes_128_cfb128();
                    break;
                case options::CIPHER_AES_128_OFB:
                    cipher_=(EVP_CIPHER*)EVP_aes_128_ofb();
                    break;
                case options::CIPHER_AES_128_CTR:
                    cipher_=(EVP_CIPHER*)EVP_aes_128_ctr();
                    break;
                case options::CIPHER_AES_192_ECB:
                    cipher_=(EVP_CIPHER*)EVP_aes_192_ecb();
                    break;
                case options::CIPHER_AES_192_CBC:
                    cipher_=(EVP_CIPHER*)EVP_aes_192_cbc();
                    break;
                case options::CIPHER_AES_192_CFB1:
                    cipher_=(EVP_CIPHER*)EVP_aes_192_cfb1();
                    break;
                case options::CIPHER_AES_192_CFB8:
                    cipher_=(EVP_CIPHER*)EVP_aes_192_cfb8();
                    break;
                case options::CIPHER_AES_192_CFB128:
                case options::CIPHER_AES_192_CFB:
                    cipher_=(EVP_CIPHER*)EVP_aes_192_cfb128();
                    break;
                case options::CIPHER_AES_192_OFB:
                    cipher_=(EVP_CIPHER*)EVP_aes_192_ofb();
                    break;
                case options::CIPHER_AES_256_ECB:
                    cipher_=(EVP_CIPHER*)EVP_aes_256_ecb();
                    break;
                case options::CIPHER_AES_256_CBC:
                    cipher_=(EVP_CIPHER*)EVP_aes_256_cbc();
                    break;
                case options::CIPHER_AES_256_CFB1:
                    cipher_=(EVP_CIPHER*)EVP_aes_256_cfb1();
                    break;
                case options::CIPHER_AES_256_CFB8:
                    cipher_=(EVP_CIPHER*)EVP_aes_256_cfb8();
                    break;
                case options::CIPHER_AES_256_CFB128:
                case options::CIPHER_AES_256_CFB:
                    cipher_=(EVP_CIPHER*)EVP_aes_256_cfb128();
                    break;
                case options::CIPHER_AES_256_OFB:
                    cipher_=(EVP_CIPHER*)EVP_aes_256_ofb();
                    break;
                case options::CIPHER_AES_256_CTR:
                    cipher_=(EVP_CIPHER*)EVP_aes_256_ctr();
                    break;
                default:
                    cipher_=(EVP_CIPHER*)EVP_aes_256_cbc();
                    break;
                }
                if(!(ctx_ = EVP_CIPHER_CTX_new())) //such as mode_id=EVP_PKEY_RSA
                    return false;
                EVP_CIPHER_CTX_init(ctx_);

                if(enable_padding)
                    padding_=1;
                else
                    padding_=0;
                if(!EVP_CIPHER_CTX_set_padding(ctx_, padding_)) return false;
                return true;
            }

            bool aes::creat_key()
            {
                try
                {
                    EVP_CipherInit_ex(ctx_, cipher_, NULL, key_, iv_, 1);
                    //if(!EVP_PKEY_keygen_init(ctx_)) throw "keygen init error!";
                    //LOG_BASELINE_DEBUG<<"[crypt public] creat_key(): keygen init complete.";
                    //if(!EVP_PKEY_CTX_set_rsa_keygen_bits(ctx_, bits_)) throw "set bits error!";
                    //LOG_BASELINE_DEBUG<<"[crypt public] creat_key(): set bits complete.";
                    //if(!EVP_PKEY_keygen(ctx_, &pkey_)) throw "keygen error!";
                    //LOG_BASELINE_DEBUG<<"[crypt public] creat_key(): keygen complete.";
                    //if(!set_padding(ctx_, padding_)) throw "set padding error!";
                    //LOG_BASELINE_DEBUG<<"[crypt public] creat_key(): set padding complete.";
                }
                catch(std::exception& err)
                {
                    LOG_BASELINE_ERROR<<"[crypt aes] creat_key(): "<<err.what();
                    reset();
                }
                catch(...)
                {
                    LOG_BASELINE_ERROR<<"[crypt aes] creat_key(): "<<"unknown error";
                    reset();
                }
                return true;
            }

            bool aes::set_padding() //1=PKCS#7 padding, 0=no padding
            {
                if(EVP_CIPHER_CTX_set_padding(ctx_, padding_)) return true;
                return false;
            }

            bool aes::save_key(std::string& pathname)
            {
                BIO *out = NULL;
                FILE *fp;

                fp = fopen(pathname.c_str(), "w");
                if (!(out = BIO_new_fp(fp, BIO_NOCLOSE)))
                {
                    LOG_BASELINE_ERROR<<"[crypt aes] save_key_public_original(): "<<"Error in new BIO";
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

            bool aes::save_key(unsigned char* key, int &keylen, unsigned char *iv, int &ivlen)
            {
                if(key_)
                {
                    if(keylen) keylen=strlen((char*)key_);
                    strcpy((char*)key,(const char*)key_);
                }
                else
                    return false;
                if(iv_)
                {
                    if(ivlen) ivlen=strlen((char*)iv_);
                    strcpy((char*)iv,(const char*)iv_);
                }
                else
                    return false;
                    return true;
            }

            bool aes::load_key(unsigned char* key, int keylen, unsigned char *iv, int ivlen)
            {
                //if(!ctx_) return false;
                reset_key();
                if((key)&&(keylen>0))
                {
                    key_=new unsigned char[keylen];
                    memcpy(key_,key,keylen);
                }
                else
                    return false;
                if((iv)&&(ivlen>0))
                {
                    iv_=new unsigned char[ivlen];
                    memcpy(iv_,iv,ivlen);
                }
                else
                    return false;

                //if(!EVP_CIPHER_CTX_set_key_length(ctx_,keylen)) return false;
                //LOG_BASELINE_DEBUG<<"set key length to ctx_. ok!";
                return true;
            }

            bool aes::save_key(std::string &key, std::string &iv)
            {
                if(key_)
                {
                    key.clear();
                    key=(char*)key_;
                }
                else
                    return false;
                if(iv_)
                {
                    iv.clear();
                    iv=(char*)iv_;
                }
                else
                    return false;
                return true;
            }

            void aes::load_key(std::string &key, std::string &iv)
            {
                //if(!ctx_) return false;
                reset_key();
                key_=new unsigned char[key.length()];
                strcpy((char*)key_, key.c_str());
                iv_=new unsigned char[iv.length()];
                strcpy((char*)iv_, iv.c_str());
            }

            bool aes::encode(unsigned char *datain, int lenin, unsigned char *dataout, int &lenout)
            {
                if((!ctx_)||(!cipher_))
                {
                    LOG_BASELINE_ERROR<<"[crypt aes] encode(): "<<"ctx or cipher not exist. ought to be init";
                    return false;
                }
                if(!EVP_EncryptInit_ex(ctx_, cipher_, NULL, key_, iv_))
                {
                    LOG_BASELINE_ERROR<<"[crypt aes] encode(): "<<"encrypt init error.";
                    return false;
                }
                if(!set_padding()) return false;
                lenout=0;
                /*
                if(!EVP_EncryptUpdate(ctx_, NULL, &lenout, datain, lenin))
                {
                    LOG_BASELINE_ERROR<<"[crypt aes] encode(): "<<"encrypt circle error.";
                    return false;
                }
                if(!(*dataout = (unsigned char *)OPENSSL_malloc(lenout+EVP_CIPHER_CTX_block_size(ctx_)+1)))
                {
                    LOG_BASELINE_ERROR<<"[crypt aes] decode(): "<<"malloc error.";
                    return false;
                }
                */
                if(!EVP_EncryptUpdate(ctx_, dataout, &lenout, datain, lenin))
                {
                    LOG_BASELINE_ERROR<<"[crypt aes] encode(): "<<"encrypt circle error.";
                    return false;
                }
                int tmpint=0;
                if(!EVP_EncryptFinal_ex(ctx_, dataout + lenout, &tmpint))
                {
                    LOG_BASELINE_ERROR<<"[crypt aes] encode(): "<<"encrypt final error.";
                    return false;
                }
                lenout = lenout + tmpint;
                dataout[lenout]=0;
                return true;
            }

            bool aes::decode(unsigned char *datain, int lenin, unsigned char *dataout, int &lenout)
            {
                if((!ctx_)||(!cipher_))
                {
                    LOG_BASELINE_ERROR<<"[crypt aes] encode(): "<<"ctx or cipher not exist. ought to be init";
                    return false;
                }
                if(!EVP_DecryptInit_ex(ctx_, cipher_, NULL, key_, iv_))
                {
                    LOG_BASELINE_ERROR<<"[crypt aes] encode(): "<<"encrypt init error.";
                    return false;
                }
                if(!set_padding()) return false;
                lenout=0;
                /*
                if(!EVP_DecryptUpdate(ctx_, NULL, &lenout, datain, lenin))
                {
                    LOG_BASELINE_ERROR<<"[crypt aes] encode(): "<<"encrypt circle error.";
                    return false;
                }
                if(!(*dataout = (unsigned char *)OPENSSL_malloc(lenout+EVP_CIPHER_CTX_block_size(ctx_)+1)))
                {
                    LOG_BASELINE_ERROR<<"[crypt aes] decode(): "<<"malloc error.";
                    return false;
                }
                */
                if(!EVP_DecryptUpdate(ctx_, dataout, &lenout, datain, lenin))
                {
                    LOG_BASELINE_ERROR<<"[crypt aes] encode(): "<<"encrypt circle error.";
                    return false;
                }
                int tmpint=0;
                if(!EVP_DecryptFinal_ex(ctx_, dataout + lenout, &tmpint))
                {
                    LOG_BASELINE_ERROR<<"[crypt aes] encode(): "<<"encrypt final error.";
                    return false;
                }
                lenout = lenout + tmpint;
                dataout[lenout]=0;
                return true;
            }

            void aes::reset_key()
            {
                if(key_)
                {
                    unsigned char* tkey_=key_;
                    key_=NULL;
                    OPENSSL_free(tkey_);
                }
                if(iv_)
                {
                    unsigned char* tiv_=iv_;
                    iv_=NULL;
                    OPENSSL_free(iv_);
                }
            }

            void aes::reset()
            {
                if(ctx_)
                {
                    EVP_CIPHER_CTX* tctx_=ctx_;
                    ctx_=NULL;
                    EVP_CIPHER_CTX_free(tctx_);
                }
                if(pkey_)
                {
                    EVP_PKEY* tpkey_=pkey_;
                    pkey_=NULL;
                    EVP_PKEY_free(tpkey_);
                }
                if(cipher_)
                {
                    EVP_CIPHER *tcipher_=cipher_;
                    cipher_=NULL;
                    OPENSSL_free(tcipher_);
                }
                if(key_)
                {
                    unsigned char* tkey_=key_;
                    key_=NULL;
                    OPENSSL_free(tkey_);
                }
                if(iv_)
                {
                    unsigned char* tiv_=iv_;
                    iv_=NULL;
                    OPENSSL_free(iv_);
                }
            }
  }
}
}
