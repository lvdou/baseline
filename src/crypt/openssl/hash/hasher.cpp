
#include <baseline/crypt/openssl/hash/hasher.h>

namespace baseline
{
namespace crypt
{
    namespace hashs
    {

            hasher::hasher():ctx_(NULL),md_(NULL)
            {
                OpenSSL_add_all_digests();
            }
            hasher::hasher(int cipher):ctx_(NULL),md_(NULL)
            {
                OpenSSL_add_all_digests();
                init(cipher);
            }
            hasher::~hasher()
            {
                if(ctx_)EVP_MD_CTX_destroy(ctx_);
                if(md_)delete md_;
                EVP_cleanup();
            }

            bool hasher::init(int cipher)
            {
                reset();
                switch(cipher)
                {
                //case options::CIPHER_MD2:
                    //md_=(EVP_MD*)EVP_md2();
                    //break;
                case options::CIPHER_MD4:
                    md_=(EVP_MD*)EVP_md4();
                    break;
                case options::CIPHER_MD5:
                    md_=(EVP_MD*)EVP_md5();
                    break;
                case options::CIPHER_SHA:
                    md_=(EVP_MD*)EVP_sha();
                    break;
                case options::CIPHER_SHA1:
                    md_=(EVP_MD*)EVP_sha1();
                    break;
                case options::CIPHER_SHA224:
                    md_=(EVP_MD*)EVP_sha224();
                    break;
                case options::CIPHER_SHA256:
                    md_=(EVP_MD*)EVP_sha256();
                    break;
                case options::CIPHER_SHA384:
                    md_=(EVP_MD*)EVP_sha384();
                    break;
                case options::CIPHER_SHA512:
                    md_=(EVP_MD*)EVP_sha512();
                    break;
                case options::CIPHER_DSS:
                    md_=(EVP_MD*)EVP_dss();
                    break;
                case options::CIPHER_DSS1:
                    md_=(EVP_MD*)EVP_dss1();
                    break;
                case options::CIPHER_ECDSA:
                    md_=(EVP_MD*)EVP_ecdsa();
                    break;
                //case options::CIPHER_MDC2:
                 //   md_=(EVP_MD*)EVP_mdc2();
                 //   break;
                case options::CIPHER_RIPEMD160:
                    md_=(EVP_MD*)EVP_ripemd160();
                    break;
                case options::CIPHER_WHIRLPOOL:
                    md_=(EVP_MD*)EVP_whirlpool();
                    break;
                default:
                    md_=(EVP_MD*)EVP_sha256();
                }
                if(!(ctx_ = EVP_MD_CTX_create()))
                    return false;
                return true;
            }

            bool hasher::encode(unsigned char *datain, std::size_t lenin, unsigned char *dataout, unsigned int &lenout)
            {
                if(!md_)
                {
                    LOG_BASELINE_ERROR<<"[crypt hasher] encode(): "<<"md not exist. ought to be init";
                    return false;
                }
                if(!ctx_)
                {
                    LOG_BASELINE_DEBUG<<"[crypt hasher] encode(): begin create";
                    ctx_ = EVP_MD_CTX_create();
                }
                if(!EVP_DigestInit_ex(ctx_, md_, NULL))
                {
                    LOG_BASELINE_ERROR<<"[crypt hasher] encode(): "<<"Digest Init error";
                    return false;
                }
                if(!EVP_DigestUpdate(ctx_, datain, lenin))
                {
                    LOG_BASELINE_ERROR<<"[crypt hasher] encode(): "<<"Digest cycle error";
                    return false;
                }
                if(!EVP_DigestFinal_ex(ctx_, dataout, &lenout))
                {
                    LOG_BASELINE_ERROR<<"[crypt hasher] encode(): "<<"Digest final error";
                    return false;
                }
                EVP_MD_CTX_cleanup(ctx_);
                return true;
            }
            unsigned char* hasher::encode(unsigned char *datain, std::size_t lenin, unsigned int &lenout)
            {
                if(!md_)
                {
                    LOG_BASELINE_ERROR<<"[crypt hasher] encode(): "<<"md not exist. ought to be init";
                    return NULL;
                }
                if(!ctx_)
                {
                    LOG_BASELINE_DEBUG<<"[crypt hasher] encode(): begin create";
                    ctx_ = EVP_MD_CTX_create();
                }
                if(!EVP_DigestInit_ex(ctx_, md_, NULL))
                {
                    LOG_BASELINE_ERROR<<"[crypt hasher] encode(): "<<"Digest Init error";
                    return NULL;
                }
                if(!EVP_DigestUpdate(ctx_, datain, lenin))
                {
                    LOG_BASELINE_ERROR<<"[crypt hasher] encode(): "<<"Digest cycle error";
                    return NULL;
                }
                lenout=0;
                unsigned char *dataout=new unsigned char[ctx_->digest->md_size+1];
                if(!EVP_DigestFinal_ex(ctx_, dataout, &lenout))
                {
                    LOG_BASELINE_ERROR<<"[crypt hasher] encode(): "<<"Digest final error";
                    return NULL;
                }
                EVP_MD_CTX_cleanup(ctx_);
                return dataout;
            }
            bool hasher::encode(std::string& strin, std::string& strout)
            {
                unsigned int lenout=0;
                unsigned char *dataout=encode((unsigned char*)strin.c_str(),(std::size_t)strin.length(),lenout);
                if(dataout)
                {
                    char *p=util::hex::hex_to_ascii((char*)dataout, lenout);
                    strout=p;
                    delete p;
                }
                delete dataout;
                return true;
            }
            std::string hasher::encode(std::string& strin)
            {
                unsigned int lenout=0;
                std::string strout="";
                unsigned char *dataout=encode((unsigned char*)strin.c_str(),(std::size_t)strin.length(),(unsigned int &)lenout);
                if(dataout)
                {
                    char *p=util::hex::hex_to_ascii((char*)dataout, lenout);
                    strout=p;
                    delete p;
                }
                delete dataout;
                return strout;
            }

            void hasher::reset_ctx()
            {
                if(ctx_)
                {
                    EVP_MD_CTX* tctx_=ctx_;
                    ctx_=NULL;
                    EVP_MD_CTX_destroy(tctx_);
                }
            }
            void hasher::reset()
            {
                if(ctx_)
                {
                    EVP_MD_CTX* tctx_=ctx_;
                    ctx_=NULL;
                    EVP_MD_CTX_destroy(tctx_);
                }
                if(md_)
                {
                    EVP_MD *tmd_=md_;
                    md_=NULL;
                    OPENSSL_free(tmd_);
                    //delete tmd_;
                }
            }
    }
}
}
