
#include <baseline/crypt/openssl/cert/pkcs.h>

namespace baseline
{
namespace crypt
{
	namespace cert
	{
        // 根据公私钥生成P12格式证书
        bool pkcs12_creat_cert(char *pubCert, int pubFormat, char *priKey, int priFormat, char *p12Cert, char *pwd)
        {
            EVP_PKEY *key=NULL;
            X509 *cert=NULL;
            PKCS12 *p12;

            cert=loadCert(pubCert,0,pubFormat,pwd);
            key=loadPrivateKey(priKey,0,priFormat,pwd);

            SSLeay_add_all_algorithms();
            p12=PKCS12_create(pwd,"(SecPass)", key, cert, NULL, 0,0,0,0,0);
            if(!p12)
            {
                X509_free(cert);
                EVP_PKEY_free(key);
                return false;
            }

            FILE * fp=fopen(p12Cert, "wb");
            i2d_PKCS12_fp(fp, p12);
            PKCS12_free(p12);
            fclose(fp);

            X509_free(cert);
            EVP_PKEY_free(key);
            EVP_cleanup();
            return true;
        }

        // 解析P12格式证书为公钥和私钥
        bool pkcs12_prase_cert_tofile(char *p12Cert,char *pwd, char *pubCert, char *priKey, int format)
        {
            EVP_PKEY *key=NULL;
            X509 *cert=NULL;
            STACK_OF(X509) *ca = NULL;
            BIO * bio=NULL,*bioCert=NULL,*bioKey=NULL;
            PKCS12 *p12=NULL;
            int i,j;

            SSLeay_add_all_algorithms();
            bio=BIO_new_file(p12Cert, "r");
            p12 = d2i_PKCS12_bio(bio, NULL);
            if (!PKCS12_parse(p12,pwd, &key, &cert, &ca))
            {
                BIO_free(bio);
                return false;
            }
            PKCS12_free(p12);

            bioCert=BIO_new_file(pubCert, "w");
            bioKey=BIO_new_file(priKey, "w");

            if(format==FORMAT_PEM)
            {
                i=PEM_write_bio_X509(bioCert,cert);
                j=PEM_write_bio_PrivateKey(bioKey, key, NULL, NULL, 0, 0, NULL);
            }
            if(format==FORMAT_DER)
            {
                i=i2d_X509_bio(bioCert,cert);
                j=i2d_PrivateKey_bio(bioKey,key);
            }

            BIO_free(bio);
            BIO_free(bioCert);
            BIO_free(bioKey);
            X509_free(cert);
            EVP_PKEY_free(key);
            EVP_cleanup();
            return true;
        }

        // 解析P12证书文件，结果存储在字符串对象中(PEM编码)
        bool pkcs12_prase_pem_tomem(char *p12Cert,char *pwd, std::string *pubCert, std::string *priKey)
        {
            EVP_PKEY *key=NULL;
            X509 *cert=NULL;
            STACK_OF(X509) *ca = NULL;
            BIO * bio=NULL,*bioCert=NULL,*bioKey=NULL;
            PKCS12 *p12=NULL;
            BUF_MEM *bptr;

            SSLeay_add_all_algorithms();
            bio=BIO_new_file(p12Cert, "r");
            p12 = d2i_PKCS12_bio(bio, NULL);
            PKCS12_parse(p12,pwd, &key, &cert, &ca);
            PKCS12_free(p12);

            bioKey=BIO_new(BIO_s_mem());
            BIO_set_close(bioKey, BIO_CLOSE);
            bioCert=BIO_new(BIO_s_mem());
            BIO_set_close(bioCert, BIO_CLOSE);

            char buf[4096];

            PEM_write_bio_X509(bioCert,cert);
            BIO_get_mem_ptr(bioCert, &bptr);
            memset(buf,0,4096);
            memcpy(buf,bptr->data,bptr->length);
            *pubCert=buf;

            PEM_write_bio_PrivateKey(bioKey, key, NULL, NULL, 0, 0, NULL);
            BIO_get_mem_ptr(bioKey, &bptr);
            memset(buf,0,4096);
            memcpy(buf,bptr->data,bptr->length);
            *priKey=buf;

            BIO_free(bioCert);
            BIO_free(bioKey);
            X509_free(cert);
            EVP_PKEY_free(key);
            EVP_cleanup();
            return true;
        }

        // 更改P12格式证书的密码
        bool pkcs12_cert_change_password(char *p12Cert,char *oldPwd, char *newPwd)
        {
            EVP_PKEY *key=NULL;
            X509 *cert=NULL;
            STACK_OF(X509) *ca = NULL;
            BIO * bio=NULL;
            PKCS12 *p12=NULL;

            SSLeay_add_all_algorithms();
            bio=BIO_new_file(p12Cert, "r");
            p12 = d2i_PKCS12_bio(bio, NULL);
            if (!PKCS12_parse(p12,oldPwd, &key, &cert, &ca))
            {
                BIO_free(bio);
                return false;
            }
            PKCS12_free(p12);

            p12=PKCS12_create(newPwd,"(SecPass)", key, cert, NULL, 0,0,0,0,0);
            if(!p12)
            {
                X509_free(cert);
                EVP_PKEY_free(key);
                return false;
            }

            FILE * fp=fopen(p12Cert, "wb");
            i2d_PKCS12_fp(fp, p12);
            PKCS12_free(p12);
            fclose(fp);

            X509_free(cert);
            EVP_PKEY_free(key);
            EVP_cleanup();
            return true;
        }

    }
}
}
