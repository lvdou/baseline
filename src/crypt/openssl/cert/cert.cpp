
#include <baseline/crypt/openssl/cert/cert.h>

namespace baseline
{
namespace crypt
{
	namespace cert
	{
        CRYPT_X509* _loadCert(CRYPT_BIO *cert, int format, char *pwd)
        {
            //ASN1_HEADER *ah=NULL;
            BUF_MEM *buf=NULL;
            X509 *x=NULL;
            if(format == FORMAT_DER)
                x=d2i_X509_bio(cert,NULL);
            else if (format == FORMAT_PEM)
                x=PEM_read_bio_X509(cert,NULL,NULL,NULL); //PEM_read_bio_X509_AUX
            else if (format == FORMAT_P12)
            {
                PKCS12 *p12 = d2i_PKCS12_bio(cert, NULL);
                PKCS12_parse(p12, pwd, NULL, &x, NULL);
                PKCS12_free(p12);
                p12 = NULL;
            }

            if (!x) return NULL;
            //if (ah != NULL) ASN1_HEADER_free(ah);
            if (buf != NULL) BUF_MEM_free(buf);
            return(x);
        }

        //载入证书，输入为文路径或内存数组
        CRYPT_X509* loadCert(char * cert, int certlen, int format, char *pwd)
        {
            BIO * in=NULL;
            X509 * x=NULL;
            if(certlen==0) //输入为磁盘文件
            {
                if((in=BIO_new_file(cert, "r")) == NULL)
                {
                    return NULL;
                }
            }
            else //输入为内存中文件
            {
                if(!(in=BIO_new_mem_buf(cert, certlen))) //只读类型
                {
                    return NULL;
                }
            }
            switch(format)
            {
            case FORMAT_DER:
                x=_loadCert(in, FORMAT_DER, pwd);
                break;
            case FORMAT_PEM:
                x=_loadCert(in, FORMAT_PEM, pwd);
                break;
            case FORMAT_P12:
                break;
            case FORMAT_P7:
                break;
            case FORMAT_CER:
                break;
            case FORMAT_CRT:
                break;
            default:
                x=_loadCert(in, FORMAT_DER, pwd);
            }
            //BIO_reset(in); //恢复bio
            if (!in) BIO_free(in);
            return x;
        }

        CRYPT_PKEY* _loadPrivateKey(BIO *bio, int format, char *pwd)
        {
            EVP_PKEY *pkey=NULL;
            if (format == FORMAT_DER)
            {
                pkey=d2i_PrivateKey_bio(bio, NULL);
            }
            else if (format == FORMAT_PEM)
            {
                pkey=PEM_read_bio_PrivateKey(bio, NULL, NULL, pwd);
            }
            else if (format == FORMAT_P12)
            {
                PKCS12 *p12 = d2i_PKCS12_bio(bio, NULL);
                PKCS12_parse(p12, pwd, &pkey, NULL, NULL);
                PKCS12_free(p12);
                p12 = NULL;
            }
            return(pkey);
        }

        CRYPT_PKEY* _loadPublicKey(BIO *bio, int format, char *pwd)
        {
            EVP_PKEY *pkey=NULL;
            if (format == FORMAT_DER)
            {
                pkey=d2i_PUBKEY_bio(bio, NULL);
            }
            else if (format == FORMAT_PEM)
            {
                pkey=PEM_read_bio_PUBKEY(bio, NULL, NULL, NULL);
            }
            else if (format == FORMAT_P12)
            {
                PKCS12 *p12 = d2i_PKCS12_bio(bio, NULL);
                PKCS12_parse(p12, pwd, &pkey, NULL, NULL);
                PKCS12_free(p12);
                p12 = NULL;
            }
            return(pkey);
        }

        CRYPT_PKEY* loadPrivateKey(char * key, int keylen, int format, char *pwd)
        {
            EVP_PKEY *pkey=NULL;
            BIO * in=NULL;
            if(keylen==0) //输入为磁盘文件
            {
                if((in=BIO_new_file(key, "r")) == NULL)
                {
                    return NULL;
                }
            }
            else //输入为内存中文件
            {
                if(!(in=BIO_new_mem_buf(key,keylen))) //只读类型
                {
                    return NULL;
                }
            }
            switch(format)
            {
            case FORMAT_DER:
                pkey=_loadPrivateKey(in, FORMAT_DER, pwd);
                break;
            case FORMAT_PEM:
                pkey=_loadPrivateKey(in, FORMAT_PEM, pwd);
                break;
            case FORMAT_P12:
                break;
            case FORMAT_P7:
                break;
            case FORMAT_CER:
                break;
            case FORMAT_CRT:
                break;
            default:
                pkey=_loadPrivateKey(in, FORMAT_DER, pwd);
            }
            //BIO_reset(in); //BIO是可读写的，那么该BIO所有数据都会被清空；
            if (in != NULL) BIO_free(in);
            return pkey;
        }

        CRYPT_PKEY* loadPublicKey(char * key, int keylen, int format, char *pwd)
        {
            EVP_PKEY *pkey=NULL;
            BIO * in=NULL;
            if(keylen==0) //输入为磁盘文件
            {
                if((in=BIO_new_file(key, "r")) == NULL)
                {
                    return NULL;
                }
            }
            else //输入为内存中文件
            {
                if(!(in=BIO_new_mem_buf(key,keylen))) //只读类型
                {
                    return NULL;
                }
            }
            switch(format)
            {
            case FORMAT_DER:
                pkey=_loadPublicKey(in, FORMAT_DER, pwd);
                break;
            case FORMAT_PEM:
                pkey=_loadPublicKey(in, FORMAT_PEM, pwd);
                break;
            case FORMAT_P12:
                break;
            case FORMAT_P7:
                break;
            case FORMAT_CER:
                break;
            case FORMAT_CRT:
                break;
            default:
                pkey=_loadPublicKey(in, FORMAT_DER, pwd);
            }
            //BIO_reset(in); //BIO是可读写的，那么该BIO所有数据都会被清空；
            if (in != NULL) BIO_free(in);
            return pkey;
        }

        CRYPT_PKEY* loadKey(char * key, int keylen, int format, char *pwd)
        {
            return loadPrivateKey(key, keylen, format, pwd);
        }

        //检查密钥对
        bool check_cert_pair(char *pubCert, int publen, int pubFormat, char *priKey, int prilen, int priFormat, char *pwd)
        {
            X509 * theCert=NULL;
            EVP_PKEY * theKey=NULL;

            theKey=loadPrivateKey(priKey,prilen,priFormat,pwd);
            theCert=loadCert(pubCert,publen,pubFormat,pwd);

            bool ret=false;
            try
            {
                ret=X509_check_private_key(theCert,theKey);
            }
            catch(...)
            {
                ret=false;
            }
            X509_free(theCert);
            EVP_PKEY_free(theKey);
            return ret;
        }

        //用根证书验证证书
        bool check_cert_withRoot(char *pubCert, int publen, int pubFormat, char *rootCert, int rootlen, int rootFormat, char *pwd)
        {
            OpenSSL_add_all_algorithms();
            X509 *x509=loadCert(pubCert,publen,pubFormat,pwd);
            X509 *root=loadCert(rootCert,rootlen,rootFormat,pwd);
            EVP_PKEY * pkey=X509_get_pubkey(root);
            int ret=X509_verify(x509,pkey);
            EVP_PKEY_free(pkey);
            X509_free(x509);
            X509_free(root);
            if(ret==1)
                return true;
            else
                return false;
        }

        // 检查证书有效期,在有效期内返回真，否则返回假
        bool check_cert_inlife(char *pubCert, int publen, int pubFormat, time_t time, char *pwd)
        {
             X509 *x509=loadCert(pubCert,publen,pubFormat,pwd);
             //time_t ct=time.GetTime();
             asn1_string_st *before=X509_get_notBefore(x509);
             asn1_string_st *after=X509_get_notAfter(x509);
             ASN1_UTCTIME *be=ASN1_STRING_dup(before);
             ASN1_UTCTIME *af=ASN1_STRING_dup(after);
             bool bf;
             if(ASN1_UTCTIME_cmp_time_t(be,time)>=0||ASN1_UTCTIME_cmp_time_t(af,time)<=0)
                bf=false;
             else
                bf=true;
             M_ASN1_UTCTIME_free(be);
             M_ASN1_UTCTIME_free(af);
             X509_free(x509);
             return bf;
        }

        // 转换私钥编码格式
        bool convert_key_format(char *oldKey, int oldlen, int oldFormat ,char *newKey, int newFormat, char *pwd)
        {
            EVP_PKEY *key=NULL;
            BIO *biout=NULL;
            int ret;

            if ((biout=BIO_new_file(newKey, "w")) == NULL)
                return false;

            if(key=loadPrivateKey(oldKey,oldlen,oldFormat,pwd))
            {
                if(newFormat==FORMAT_DER)
                    ret=i2d_PrivateKey_bio(biout,key);
                if(newFormat==FORMAT_PEM)
                    ret=PEM_write_bio_PrivateKey(biout, key, NULL, NULL, 0, 0, NULL);
            }
            else
                return false;

            BIO_free(biout);
            EVP_PKEY_free(key);
            return true;
        }

    }
}
}
