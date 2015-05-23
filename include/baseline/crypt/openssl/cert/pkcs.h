
/**********************************

    *.DER   二进制DER编码的证书
    *.PEM   ASCII(Base64)编码的各种X.509证书
    *.KEY   PCSK7、8、12的公钥和私钥，可以是DER、PEM编码
    *.CER   CRT证书，微软型式，可以是DER、PEM编码
    *.CRT   基本等同于CER证书。

***********************************/


#ifndef BASELINE_CRYPT_OPENSSL_crypt_PKCS_H
#define BASELINE_CRYPT_OPENSSL_crypt_PKCS_H

//#include "../../include/crypt/openssl/crypt_base.h"
#include <baseline/crypt/openssl/cert/cert.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/x509.h>
#include <openssl/asn1.h>
#include <openssl/pkcs12.h>
#include <openssl/pkcs7.h>

namespace baseline
{
namespace crypt
{
	namespace cert
	{
		using namespace crypt;

        // 根据公私钥生成P12格式证书
        bool pkcs12_creat_cert(char *pubCert, int pubFormat, char *priKey, int priFormat, char *p12Cert, char *pwd);
        // 解析P12格式证书为公钥和私钥
        bool pkcs12_prase_cert_tofile(char *p12Cert,char *pwd, char *pubCert, char *priKey, int format);
        // 解析P12证书文件，结果存储在字符串对象中(PEM编码)
        bool pkcs12_prase_pem_tomem(char *p12Cert,char *pwd, std::string *pubCert, std::string *priKey);
        // 更改P12格式证书的密码
        bool pkcs12_cert_change_password(char *p12Cert,char *oldPwd, char *newPwd);

    }
}
}
#endif // CRYPT_OPENSSL_H
