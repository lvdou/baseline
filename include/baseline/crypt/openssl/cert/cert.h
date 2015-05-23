#ifndef BASELINE_CRYPT_OPENSSL_crypt_CERT_H
#define BASELINE_CRYPT_OPENSSL_crypt_CERT_H

#include <baseline/crypt/openssl/crypt_base.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/x509.h>
#include <openssl/asn1.h>
#include <openssl/pkcs12.h>
#include <openssl/pkcs7.h>
#include <openssl/pem.h>


/*----------------------------------

*.DER   二进制DER编码的证书
*.PEM   ASCII(Base64)编码的各种X.509证书
*.KEY   PCSK7、8、12的公钥和私钥，可以是DER、PEM编码
*.CER   CRT证书，微软型式，可以是DER、PEM编码
*.CRT   基本等同于CER证书。

-----------------------------------*/

namespace baseline
{
namespace crypt
{
	namespace cert
	{
		using namespace crypt;

        enum certFormat{FORMAT_DER=1,FORMAT_PEM,FORMAT_P7,FORMAT_P12,FORMAT_CRT,FORMAT_CER};

        CRYPT_X509* _loadCert(CRYPT_BIO *cert, int format, char *pwd);
        //载入证书，输入为文路径或内存数组
        CRYPT_X509* loadCert(char * cert, int certlen, int format, char *pwd);
        CRYPT_PKEY* _loadPrivateKey(BIO *bio, int format, char *pwd);
        CRYPT_PKEY* _loadPublicKey(BIO *bio, int format, char *pwd);
        CRYPT_PKEY* loadPrivateKey(char * key, int keylen, int format, char *pwd);
        CRYPT_PKEY* loadPublicKey(char * key, int keylen, int format, char *pwd);
        CRYPT_PKEY* loadKey(char * key, int keylen, int format, char *pwd);
        //检查密钥对
        bool check_cert_pair(char *pubCert, int publen, int pubFormat, char *priKey, int prilen, int priFormat, char *pwd);
        //用根证书验证证书
        bool check_cert_withRoot(char *pubCert, int publen, int pubFormat, char *rootCert, int rootlen, int rootFormat, char *pwd);
        // 检查证书有效期,在有效期内返回真，否则返回假
        bool check_cert_inlife(char *pubCert, int publen, int pubFormat, time_t time, char *pwd);
        // 转换私钥编码格式
        bool convert_key_format(char *oldKey, int oldlen, int oldFormat ,char *newKey, int newFormat, char *pwd);
    }
}
}
#endif // CRYPT_OPENSSL_H
