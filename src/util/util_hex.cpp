
#include <baseline/util/util_hex.h>
#include <boost/algorithm/hex.hpp>

namespace baseline
{
namespace util
{
    namespace hex
    {
        share_array hton_share(char* indata, int inlen)
        {
            boost::mutex::scoped_lock lock(hex_mutex);
            std::size_t i=inlen;
            char *out=new char[i];
            char *p=indata;
            out+=inlen;
            for(;i>0;--i)
            {
                --out;
                *out=*p;
                ++p;
            }
            share_array s_str(out);
            return s_str;
        }
        share_array ntoh_share(char* indata, int inlen)
        {
            return hton_share(indata,inlen);
        }
        char* hton(char* indata, int inlen)
        {
            boost::mutex::scoped_lock lock(hex_mutex);
            std::size_t i=inlen;
            char *out=new char[i];
            char *p=indata;
            out+=inlen;
            for(;i>0;--i)
            {
                --out;
                *out=*p;
                ++p;
            }
            return out;
        }
        char* ntoh(char* indata, int inlen)
        {
            return hton(indata,inlen);
        }

        share_array hex_to_ascii_share(char* indata, int inlen)
        {
            boost::mutex::scoped_lock lock(hex_mutex);
            char* out=new char[inlen*2+1];
            try
            {
                boost::algorithm::hex(indata,indata+inlen,out);
                out[inlen*2]=0;
                share_array  s_str(out);
                return s_str;
            }
            catch(...)
            {
                memset(out,0,inlen*2+1);
                share_array  s_str(out);
                return s_str;
            }
        }
        char* hex_to_ascii(char* indata, int inlen)
        {
            boost::mutex::scoped_lock lock(hex_mutex);
            char *out=new char[inlen*2+1];
            try
            {
                boost::algorithm::hex(indata,indata+inlen,out);
                out[inlen*2]=0;
            }
            catch(...)
            {
                //delete out;
                return NULL;
            }
            return out;
        }
        std::string hex_to_ascii_string(char* indata, int inlen)
        {
            boost::mutex::scoped_lock lock(hex_mutex);
            std::string str="";
            char out[inlen*2+1];
            try
            {
                boost::algorithm::hex(indata,indata+inlen,out);
                out[inlen*2]=0;
                str=out;
            }
            catch(...)
            {
                //delete out;
                return str;
            }
            return str;
        }
        share_array hex_from_ascii_share(char* indata, int inlen)
        {
            boost::mutex::scoped_lock lock(hex_mutex);
            char *out=new char[inlen/2+1];
            //memset(out,0,inlen/2+1);
            try
            {
                boost::algorithm::unhex(indata,indata+inlen,out);
                out[inlen/2]=0;
                share_array  s_str(out);
                return s_str;
            }
            catch(...)
            {
                memset(out,0,inlen/2+1);
                share_array  s_str(out);
                return s_str;
            }
        }
        char* hex_from_ascii(char* indata, int inlen)
        {
            boost::mutex::scoped_lock lock(hex_mutex);
            char *out=new char[inlen/2+1];
            //memset(out,0,inlen/2+1);
            try
            {
                boost::algorithm::unhex(indata,indata+inlen,out);
                out[inlen/2]=0;
            }
            catch(...)
            {
                return NULL;
            }
            return out;
        }

        share_array stringToArray_share(std::string strin)
        {
            boost::mutex::scoped_lock lock(string_mutex);
            util::Hex hexer;
            strin=util::regex::remove(strin,"(\\s|:)+");
            if(strin.length()%2) strin="0"+strin;
            share_array out=hex_from_ascii_share((char*)strin.c_str(),strin.length());
            return out;
        }
        char* stringToArray(std::string strin)
        {
            boost::mutex::scoped_lock lock(string_mutex);
            util::Hex hexer;
            strin=util::regex::remove(strin,"(\\s|:)+");
            if(strin.length()%2) strin="0"+strin;
            return hex_from_ascii((char*)strin.c_str(),strin.length());
        }

        /*
        share_array hex_to_array_text(char* indata, int inlen)
        {
            boost::mutex::scoped_lock lock(hex_mutex);
            char* tostr=new char[inlen*2+1];
            memset(tostr,0,inlen*2+1);
            char *p=indata;
            for(int i=0;i<inlen;++i)
            {
                Byte_to_text((char)*p, tostr);
                tostr+=2;
                ++p;
            }
            p=NULL;
            tostr-=inlen*2;
            share_array  s_str(tostr);
            return s_str;
        }
        share_array hex_from_array_text(char* indata, int inlen)
        {
            boost::mutex::scoped_lock lock(hex_mutex);
            char *tohex=new char[inlen/2];
            memset(tohex,0,inlen/2);
            char *p=indata;
            for(int i=0;i<inlen;++i)
            {
                Byte_from_text(p, tohex);
                tohex++;
                p+=2;
            }
            p=NULL;
            tohex-=inlen*2;
            share_array  s_str(tohex);
            return s_str;
        }
        */

        void Byte_to_ascii(char from, char* to)
        {
            boost::mutex::scoped_lock lock(byte_mutex);
            int left, right;
            unsigned int udec=from;
            if (from<0){udec=128-abs(from)+128;}
            left=udec/16;
            right=udec%16;
            if(left<10)
                to[0]=left+48;
            else
                to[0]=left-10+65;
            if(right<10)
                to[1]=right+48;
            else
                to[1]=right-10+65;
            //to[2]=0x20;
            return;
        }
        void Byte_from_ascii(char *from, char* to)
        {
            boost::mutex::scoped_lock lock(byte_mutex);
            uint8_t left, right;
            char *p=from;
            if(*p<58)
                left=*p-48;
            else
                left=*p-55;
            ++p;
            if(*p<58)
                right=*p-48;
            else
                right=*p-55;
            *to=left*16+right;
            return;
        }
        share_array Byte_to_ascii_share(char dec)
        {
            boost::mutex::scoped_lock lock(byte_mutex);
            char *p=new char[3];
            //memset(p,0,3);
            boost::format formater("%X");
            formater % (int)dec;
            memcpy(p,formater.str().c_str(),2);
            p[2]=0;
            share_array s_str(p);
            return s_str;
        }
        char* Byte_to_ascii(char dec)
        {
            boost::mutex::scoped_lock lock(byte_mutex);
            char *p=new char[3];
            boost::format formater("%X");
            formater % (int)dec;
            memcpy(p,formater.str().c_str(),2);
            p[2]=0;
            return p;
        }
        share_array Byte2_to_ascii_share(char* dec)
        {
            boost::mutex::scoped_lock lock(byte_mutex);
            char *to=new char[5];
            char *p = dec;
            //memset(to,0,5);
            boost::format formater("%X");
            formater % (int)*p;
            memcpy(to,formater.str().c_str(),2);
            ++p;
            to+=2;
            formater % (int)*p;
            memcpy(to,formater.str().c_str(),2);
            to-=2;
            to[5]=0;
            share_array s_str(to);
            return s_str;
        }
        char * Byte2_to_ascii(char* dec)
        {
            boost::mutex::scoped_lock lock(byte_mutex);
            char *to=new char[5];
            char *p = dec;
            //memset(to,0,5);
            boost::format formater("%X");
            formater % (int)*p;
            memcpy(to,formater.str().c_str(),2);
            ++p;
            to+=2;
            formater % (int)*p;
            memcpy(to,formater.str().c_str(),2);
            to-=2;
            to[5]=0;
            return to;
        }
        share_array Byte2_from_ascii_share(char* dec)
        {
            boost::mutex::scoped_lock lock(byte_mutex);
            char *to=new char[2];
            if(!dec)
            {
                memset(to,0,2);
                share_array s_str(to);
                return s_str;
            }
            char *p = dec;
            //memset(to,0,2);
            boost::format formater("%c");
            formater % *p;
            memcpy(to,formater.str().c_str(),1);
            p+=2;
            ++to;
            formater % *p;
            memcpy(to,formater.str().c_str(),1);
            --to;
            share_array s_str(to);
            return s_str;
        }
        char * Byte2_from_ascii(char* dec)
        {
            boost::mutex::scoped_lock lock(byte_mutex);
            if(!dec)
                return NULL;
            char *to=new char[2];
            char *p = dec;
            //memset(to,0,2);
            boost::format formater("%c");
            formater % *p;
            memcpy(to,formater.str().c_str(),1);
            p+=2;
            ++to;
            formater % *p;
            memcpy(to,formater.str().c_str(),1);
            --to;
            return to;
        }
    }
}
}
