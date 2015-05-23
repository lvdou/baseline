#ifndef BASELINE_UTIL_HEX_H
#define BASELINE_UTIL_HEX_H

#include <baseline/util/util_base.h>
#include <baseline/util/util_string.h>
#include <baseline/util/util_regex.h>

#include <boost/array.hpp>
#include <boost/shared_array.hpp>


/* =====================

int vint;
char* vchar;

vchar = (char*)&vint    //int -> char*
vint = (int&)vchar      //char* -> int

=======================*/

namespace baseline
{
namespace util
{
    //typedef boost::shared_array<char> share_array;

    namespace hex //: private boost::noncopyable
    {
        using namespace util;

        static boost::mutex byte_mutex;
        static boost::mutex hex_mutex;
        static boost::mutex string_mutex;

        template<typename ValueType>
        ValueType hton_number(ValueType &number)
        {
            static boost::mutex io_mutex;
            boost::mutex::scoped_lock lock(io_mutex);
            std::size_t i=sizeof(ValueType);
            char out[i];
            char *p=(char*)&number;
            for(;i>0;--i)
            {
                out[i-1]=*p;
                ++p;
            }
            //i=sizeof(ValueType);
            return (ValueType&)out;
        }
        template<typename ValueType>
        ValueType ntoh_number(ValueType &number)
        {
            return hton_number<ValueType>(number);
        }
        share_array hton_share(char* indata, int inlen);
        share_array ntoh_share(char* indata, int inlen);
        char* hton(char* indata, int inlen);
        char* ntoh(char* indata, int inlen);

        //share_array hex_to_array_text_boost(char* indata, int inlen);
        //share_array hex_from_array_text_boost(char* indata, int inlen);
        share_array hex_to_ascii_share(char* indata, int inlen);
        char* hex_to_ascii(char* indata, int inlen);
        std::string hex_to_ascii_string(char* indata, int inlen);
        share_array hex_from_ascii_share(char* indata, int inlen);
        char* hex_from_ascii(char* indata, int inlen);

        share_array stringToArray_share(std::string strin);
        char* stringToArray(std::string strin);

        share_array Byte_to_ascii_share(char dec);
        char* Byte_to_ascii(char dec);

        share_array Byte2_to_ascii_share(char* dec);
        char * Byte2_to_ascii(char* dec);
        share_array Byte2_from_ascii_share(char* dec);
        char * Byte2_from_ascii(char* dec);

        void Byte_to_ascii(char from, char* to);
        void Byte_from_ascii(char *from, char* to);

        template<typename T=char>
        class _Hex
        {
        public:
            _Hex():size_(0),length_(0)
            {
                pArray_=new T[nBufSize];
                size_=nBufSize;
            }
            ~_Hex()
            {
                delete pArray_;
            }

            T & operator[] (std::size_t index) //std::ptrdiff_t
            {
                BOOST_ASSERT(pArray_ != 0);
                BOOST_ASSERT(index >= 0);
                return pArray_[index];
            }
            const T & operator[] (std::size_t index) const //std::ptrdiff_t
            {
                BOOST_ASSERT(pArray_ != 0);
                BOOST_ASSERT(index >= 0);
                return pArray_[index];
            }

            std::size_t size(){return size_;}
            std::size_t length(){return length_;}
            void clear()
            {
                boost::mutex::scoped_lock lock(io_mutex);
                length_=0;
            }
            void reset()
            {
                boost::mutex::scoped_lock lock(io_mutex);
                resize(nBufSize,true);
                length_=0;
                size_=0;
            }
            bool empty()
            {
                if(length_>0)
                    return false;
                else
                    return true;
            }

            ByteArray* toArray(std::string param1)
            {
                ByteArray *_loc_2=new ByteArray(2048);
                int _loc_3=0;
                try
                {
                    param1 = util::regex::remove(param1,"(\\s|:)+");
                    param1 = util::string::toUpperCase(param1);
                    //LOG_BASELINE_DEBUG<<"param1="<<param1;
                    if(param1.length()==1)
                    {
                        param1 = "0" + param1;
                    }
                    _loc_3 = 0;
                    while (_loc_3 < param1.length())
                    {
                        //LOG_BASELINE_DEBUG<<"param1.substr("<<_loc_3<<", 2)="<<param1.substr(_loc_3, 2);
                        //char to[2048]={0};
                        char *to=hex_from_ascii((char*)param1.c_str(),param1.length());
                        //Byte_from_text((char*)param1.substr(_loc_3, 2).c_str(),to);
                        //LOG_BASELINE_DEBUG<<"to="<<to;
                        //(*_loc_2)[_loc_3/2] = boost::lexical_cast<int16_t>(param1.substr(_loc_3, 2));
                        _loc_2->writeBytes((unsigned char*)to,param1.length()*2);
                        _loc_3 = _loc_3 + 2;
                    }
                    return _loc_2;
                }
                catch(std::exception& err)
                {
                    LOG_BASELINE_ERROR<<"[toArray] has error: "<<err.what();
                    return _loc_2=NULL;
                }
                catch(...)
                {
                    LOG_BASELINE_ERROR<<"[toArray] has unknown error: ";
                    return _loc_2=NULL;
                }
            }
            std::string toString(std::string param1)
            {
                _ByteArray p;
                char str[size_]={0};
                p=toArray(param1);
                p.readBytes((unsigned char*)str,p.length());
                std::string out(str);
                return out;
            }
            std::string fromArray(ByteArray *param1, bool param2 = false)
            {
                std::string _loc_3="";
                int _loc_4 =0;
                while (_loc_4 < param1->length())
                {
                    char get=(*param1)[_loc_4];
                    std::string gets=hex_to_ascii_string(&get, 1);
                    _loc_3 = _loc_3 + gets;
                    if (param2)
                    {
                        if (_loc_4 < (param1->length() - 1))
                        {
                            _loc_3 = _loc_3 + ":";
                        }
                    }
                    _loc_4 = _loc_4 + 1;
                }
                return _loc_3;
            }
            std::string fromString(std::string param1, bool param2= false)
            {
                ByteArray _loc_3(2048);
                _loc_3.writeBytes((unsigned char*)param1.c_str(),param1.length());
                return fromArray(&_loc_3, param2);
            }

            int writeByte(T *datain)
            {
                boost::mutex::scoped_lock lock(io_mutex);
                try
                {
                    if(!datain)return 0;
                    if(!(resize(1,false)<0))
                    {
                        T *p=pArray_+length_;
                        memcpy(p,datain,1);
                        length_+=1;
                        return 1;
                    }
                    return -1;
                }
                catch(...)
                {
                    return -1;
                }
            }

            int write(T *datain, std::size_t datalen)
            {
                boost::mutex::scoped_lock lock(io_mutex);
                try
                {
                    if((!datain)||!(datalen>0))return 0;
                    if(!(resize(datalen,false)<0))
                    {
                        T *p=pArray_+length_;
                        //if(appConfig.get<bool>("is_big_endian")==true)
                        memcpy(p,datain,datalen);
                        //else
                        //    memcpy(p,util::hex::hton(datain,datalen).get(),datalen);
                        length_+=datalen;
                        return 1;
                    }
                    return -1;
                }
                catch(...)
                {
                    return -1;
                }
            }

            boost::shared_array<T> get_shared()
            {
                //boost::mutex::scoped_lock lock(io_mutex);
                //T *ar=new T[length_+1];
                //memcpy(ar,pArray_,length_);
                //ar[length_]=0;
                return boost::shared_array<T>(get());
            }

            T* get()
            {
                boost::mutex::scoped_lock lock(io_mutex);
                T *ar=new char[length_+1];
                memcpy(ar,pArray_,length_);
                ar[length_]=0;
                return ar;
            }

        private:
            static const int nBufSize = 1500;
            std::size_t size_;
            std::size_t length_;
            T * pArray_;
            boost::mutex io_mutex;
            //config::app::appConfig &appConfig = config::app::so_appConfig::get_mutable_instance();

            int resize(std::size_t datalen, bool is_force)
            {
                try
                {
                    if((datalen > size_)||(datalen+length_ > size_)||is_force)
                    {
                        std::size_t tosize=0;
                        if(is_force)
                            tosize=datalen;
                        else
                            tosize=datalen+length_+nBufSize;

                        T *p=pArray_;
                        pArray_=new T[tosize];
                        if(!is_force) memcpy(pArray_,p,length_);
                        delete[] p;
                        size_=tosize;
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }

                }
                catch(...)
                {
                    return -1;
                }
            }
        };



    }

    typedef hex::_Hex<char> Hex;
    //typedef boost::serialization::singleton<crc> so_util_crc;
    //typedef boost::serialization::singleton<b2c> so_util_b2c;
    //typedef boost::serialization::singleton<time> so_util_time;
}

}
#endif // UTIL_H
