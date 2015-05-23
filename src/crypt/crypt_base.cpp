
#include <baseline/crypt/crypt_base.h>
#include <openssl/rand.h>

namespace baseline
{
namespace crypt
{

	#pragma comment(lib,"libeay32.lib")
	#pragma comment(lib,"ssleay32.lib")

    //ran.seed(time(NULL));
    #define MODULUS "6968296057"
    #define PUBLIC_EXPONENT 218723
    #define PRIVATE_EXPONENT "298553057"
    #define Salt_Base "0e63ca52b2"

    #define RSA_PRIVATE_KEY_NAME "lib\\cet.pvt"
    #define RSA_PUBLIC_KEY_NAME "lib\\cet.pub"
    #define FILE_SN_NAME "sn.key"
    #define FILE_ACTIVE_NAME "active.key"

/*
    crypt::crypt()
    {
        int ret, top, bottom;
        unsigned long e=RSA_F4;
        BITS=1024;
        rsa_isset_key_public=0;
        rsa_isset_key_private=0;
        memset(rnd_seed,0,sizeof(rnd_seed));
        RSAPADDING = RSA_PKCS1_PADDING;
        prsa = RSA_new();

        BIGNUM *bne;
        BIGNUM *rnd;
        //printf("RSA_generate_key_ex will do...\r\n");
        //prsa = RSA_generate_key(BITS,RSA_F4,0,0); //函数过时了
        bne=BN_new();
        ret=BN_set_word(bne,e);
        ret=BN_pseudo_rand(rnd, BITS, top, bottom);
        if(ret!=1)
            LOG_BASELINE_ERROR<<"[crypt] BN_pseudo_rand err!";
        RAND_seed(rnd_seed, sizeof rnd_seed);
        ret=RSA_generate_key_ex(prsa,BITS,bne,NULL);
        if(ret!=1)
            LOG_BASELINE_ERROR<<"[crypt] RSA_generate_key_ex err!";

        ret=RSA_check_key(prsa);
        if(ret==1)
        {
            FLEN = RSA_size(prsa);
            if(RSAPADDING==RSA_PKCS1_PADDING)
                FLEN-=11;
            else if(RSAPADDING==RSA_X931_PADDING)
                FLEN-=2;
            else if(RSAPADDING==RSA_NO_PADDING)
                FLEN=FLEN;
            else
                FLEN = 0;
        }
        else
        {
            FLEN = 0;
            LOG_BASELINE_ERROR<<"[crypt] RSA_check_key err!";
        }
        //RSA_print_fp(stdout,prsa,11);

        if(load_key_RSA()<=0)
        {
            LOG_BASELINE_ERROR<<"[crypt] load key error!";
        }

        memset(aes_keybuf,0,sizeof(aes_keybuf));
    }

    crypt::~crypt()
    {
        delete[] rnd_seed;
        RSA_free(prsa);
        RSA_free(prirsa);
        RSA_free(pubrsa);
        rsa_isset_key_public=0;
        rsa_isset_key_private=0;
    }

    void crypt::aes_encrypt_to(unsigned char *datain, unsigned char *dataout, int bits)
    {
        int ret;
        ret=strlen((const char*)datain);
        if(ret<=0) return;
        AES_KEY aeskey;
        AES_set_encrypt_key(aes_keybuf,bits,&aeskey);
        AES_encrypt(datain,dataout,&aeskey);
    }

    void crypt::aes_decrypt_to(unsigned char *datain, unsigned char *dataout, int bits)
    {
        int ret;
        ret=strlen((const char*)datain);
        if(ret<=0) return;
        AES_KEY aeskey;
        AES_set_encrypt_key(aes_keybuf,bits,&aeskey);
        AES_decrypt(datain,dataout,&aeskey);
    }

    int crypt::load_key_RSA(void)
    {
        if(load_key_rsa_private()<=0)
        {
            LOG_BASELINE_ERROR<<"[crypt] load private key error!";
        }
        if(load_key_rsa_public()<=0)
        {
            LOG_BASELINE_ERROR<<"[crypt] load public key error!";
            return 0;
        }
        return 1;
    }

    int crypt::rsa_encrypt(unsigned char *datain, unsigned char *dataout)
    {
        int ret;
        ret=strlen((const char*)datain);
        if(ret<=0)
            return 0;
        else
        {
            if(ret<BITS)
                memset(dataout, 0, ret);
            else
                memset(dataout, 0, BITS);
        }

        //char strin[1024]={0};
        //strcat(strin,Salt_Base);
        //strcat(strin,"|");
        //strcat(strin,(const char*)datain);
        RAND_seed(rnd_seed, sizeof rnd_seed);
        //ret = RSA_public_encrypt(strlen((const char*)strin), (unsigned char*)strin, dataout, pubrsa, RSAPADDING);
        ret = RSA_public_encrypt(FLEN, datain, dataout, pubrsa, RSAPADDING);
        if (ret < 0)
        {
            LOG_BASELINE_ERROR<<"[crypt] RSA Encrypt failed!";
            return 0;
        }
            //debugf_s("[RSA]Encrypt ok: %d",ret);

        return ret;
    }

    int crypt::rsa_decrypt(unsigned char *datain, unsigned char *dataout)
    {
        int ret,flen;
        memset(dataout, 0, BITS);

        ret = RSA_private_decrypt(FLEN, datain, dataout, prirsa, RSAPADDING);
        if (ret < 0)
        {
            LOG_BASELINE_ERROR<<"[crypt] rsa Decrypt failed!";
            return 0;
        }
        return ret;
    }

    int crypt::save_key_rsa_public(void)
    {
        if(RSA_check_key(prsa) != 1)
            return 0;
        if((pfpublickey = fopen(RSA_PUBLIC_KEY_NAME, "wb")) == NULL)
            return 0;
        if(PEM_write_RSAPublicKey(pfpublickey, prsa))
           fclose(pfpublickey);
        else
            return 0;
        return 1;
    }

    int crypt::save_key_rsa_private(void)
    {
        if(RSA_check_key(prsa)!= 1)
            return 0;
        if((pfprivatekey = fopen(RSA_PRIVATE_KEY_NAME, "wb")) == NULL)
            return 0;
        if(PEM_write_RSAPrivateKey(pfprivatekey,prsa,NULL,NULL,0,0,NULL))
           fclose(pfprivatekey);
        else
            return 0;
        return 1;
    }

    int crypt::load_key_rsa_public(void)
    {
        if((pfpublickey = fopen(RSA_PUBLIC_KEY_NAME, "rb")) == NULL)
            return 0;
        if(!(pubrsa=PEM_read_RSAPublicKey(pfpublickey, NULL, NULL, NULL)))
            return 0;
        fclose(pfpublickey);
        if(!rsa_check_pub_key("1653298",7))
        {
            LOG_WARN<<"pub key not correct!";
            return 0;
        }
        rsa_isset_key_public=1;
        return 1;
    }

    bool crypt::rsa_check_pub_key(char * strin, int width)
    {
        if(!pubrsa)
            return false;
        if((width>10)||(width<=0))
            return false;
        char str2[10]={0};
        bool istrue=false;

        char *p = BN_bn2dec((pubrsa->n));
        if (p)
        //    debugf_s("p number is %s", p); //1653298
        memcpy(str2,p,width);
        if(strcmp(str2,strin)==0)
            istrue=true;

        OPENSSL_free(p);
        if(istrue)
            return true;
        else
            return false;
    }

    int crypt::load_key_rsa_private(void)
    {
        if((pfprivatekey = fopen(RSA_PRIVATE_KEY_NAME, "rb")) == NULL)
            return 0;
        if(!(prirsa=PEM_read_RSAPrivateKey(pfprivatekey, NULL, NULL, NULL)))
            return 0;
        fclose(pfprivatekey);
        if(RSA_check_key(prirsa)!= 1)
            return 0;
        rsa_isset_key_private=1;
        return 1;
    }

    int crypt::rsa_generate_key_sn(char *datain)
    {
        int ret=0;
        int count=0;
        unsigned char strencode[1024]={0};
        char strmd5[1024]={0};
        char strtemp[1024]={0};
        //printf("datain: %s\r\n",datain);
        count=rsa_encrypt((unsigned char*)datain,strencode);
        if(count>0)
        {
            //ret=md5_hash(strencode,count,strmd5);
            //if(ret>0)
            //{
                //debugf_s("encode md5: %s",(const char*)strmd5);
                if(save_file((const char*)FILE_SN_NAME, (unsigned char*)strencode, count, true)>0)
                    LOG_BASELINE_DEBUG<<"[crypt] sn key generated.";
                else
                    return 0;
            //}
        }
        else
            return 0;
        return 1;
    }

    int crypt::save_conf_file(char *datain, char *path)
    {
        int ret=0;
        int count=0;
        unsigned char strencode[1024]={0};
        char strmd5[1024]={0};
        char strtemp[1024]={0};
        //printf("datain: %s\r\n",datain);
        //count=rsa_encrypt((unsigned char*)datain,strencode);
        //if(count>0)
        {
            if(save_file((const char*)path, (unsigned char*)datain, strlen(datain), false)<=0)
                //debugf_s("conf file saved.");
            //else
                return 0;
        }
    //    else
    //        return 0;
        return 1;
    }


    int crypt::rsa_generate_key_active(char *datain)
    {
        int ret=0;
        int count=0;
        unsigned char strencode[1024]={0};
        char strmd5[1024]={0};

        ret=md5_hash((unsigned char*)datain,strlen(datain),strmd5);
        if(ret>0)
        {
            //debugf_s("encode md5: %s",(const char*)strmd5);
            if(save_file((const char*)FILE_ACTIVE_NAME, (unsigned char*)strmd5, ret,true)>0)
                LOG_BASELINE_DEBUG<<"[crypt] active key generated.";
            else
                return 0;
        }
        else
            return 0;
        return 1;
    }

    int crypt::md5_hash(unsigned char *datain, int inlen, char *dataout)
    {
        int count;
        unsigned char md[16]={0};
        char tmp[3]={'0'};

        MD5(datain,inlen,md);
        memset(dataout,0,sizeof(dataout));
        count=char2Hex(md, sizeof(md), dataout);
        if(count<=0)
            return 0;
        else
            return count;
    }

    int crypt::char2Hex(unsigned char *datain, int inlen, char *dataout)
    {
        int i;
        char tmp[3]={'0'};

        for (i = 0; i < inlen; i++)
        {
            sprintf(tmp,"%2.2X",datain[i]);
            strcat(dataout,tmp);
        }
        return i*2;
    }

    int crypt::save_file(const char* filename, unsigned char* datain, int inlen, bool isshort)
    {
        int count;
        char *path;
        char abpath[512]={0};
    //    if(!(path=_getcwd(NULL, 0)))
           return 0;
        if(isshort)
        {
            strcat(abpath,path);
            strcat(abpath,"\\");
        }
        strcat(abpath,filename);
        FILE *fp;

        try
        {
            if(inlen<=0)
                throw 0;

            //debugf_s("path=%s",abpath);
            if((fp = fopen(abpath, "wb")) == NULL)
                throw 0;
            count=fwrite(datain,inlen,1,fp);
            //debugf_s("path=%s count=%d",abpath,count);
            if(count>0)
            {
                if(fflush(fp)!=0)
                    throw 0;
                //if(fsync(fileno(fp))<0)
                //    return 0;
                fclose(fp);
            }
            else
                throw 0;
        }
        catch(...)
        {
            if(fp)
                fclose(fp);
            return 0;
        }
        return count;
    }

    int crypt::load_file(const char* filename, unsigned char* dataload)
    {
        int count;
        char *path;
        char abpath[512]={0};
        FILE *fp;
    //    if(!(path=getcwd(NULL, 0)))
           return 0;
        strcat(abpath,path);
        strcat(abpath,"\\");
        strcat(abpath,filename);
        if((fp = fopen(abpath, "rb")) == NULL)
            return 0;

        long curpos;
        curpos = ftell(fp);
        fseek(fp, 0L, SEEK_END);
        long filesize = ftell(fp);
        fseek(fp, curpos, SEEK_SET);

        //int filesize = fseek(fp, 0L, SEEK_END);
        fseek(fp, 0, SEEK_SET);
        if(filesize>BITS)
            return 0;
        count=fread(dataload,filesize,1,fp);
        //debugf_s("path=%s filesize=%d count=%d",abpath,filesize,count);
        if(count>0)
           fclose(fp);
        else
            return 0;
        return count;
    }

    int crypt::read_conf_file(unsigned char* dataload, const char* filename)
    {
        int count;
        char *path;
        char abpath[512]={0};
        FILE *fp;
        strcat(abpath,filename);

        try
        {
        if((fp = fopen(abpath, "rb")) == NULL)
            throw 0;
        long curpos;
        curpos = ftell(fp);
        fseek(fp, 0L, SEEK_END);
        long filesize = ftell(fp);
        fseek(fp, curpos, SEEK_SET);

        //int filesize = fseek(fp, 0L, SEEK_END);
        fseek(fp, 0, SEEK_SET);
        if(filesize>BITS)
            throw 0;
        count=fread(dataload,filesize,1,fp);
        //debugf_s("path=%s filesize=%d count=%d",abpath,filesize,count);
        if(count>0)
           fclose(fp);
        else
            throw 0;
        }
        catch(...)
        {
            if(fp)
                fclose(fp);
            return 0;
        }
        return count;
    }
*/
}
}
