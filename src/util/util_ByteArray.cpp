
#include <baseline/util/util_ByteArray.h>

namespace baseline
{
namespace util
{
    namespace hex //: private boost::noncopyable
    {

        _ByteArray::_ByteArray(std::size_t maxSize): wPos(0),rPos(0),_maxSize(0)
        {
            _maxSize=maxSize;
            _bytes = new unsigned char[_maxSize];

            Endian cpuEndian = getCPUEndian();
            _sysEndian = cpuEndian;
            setEndian(ENDIAN_BIG, ENDIAN_BIG);
        }

        _ByteArray::~_ByteArray()
        {
            delete _bytes;
        }

        std::size_t _ByteArray::length()
        {
            return wPos - rPos;
        }
        std::size_t _ByteArray::length_available()
        {
            return _maxSize - wPos;
        }

        void _ByteArray::reset()
        {
            wPos=rPos=0;
        }
        void _ByteArray::resize()
        {
            std::size_t ret=wPos-rPos;

            if(ret>0)
            {
                char save[ret];
                memcpy(save,_bytes+rPos,ret);
                wPos=rPos=0;
                memcpy(_bytes,save,ret);
                wPos+=ret;
            }
        }

        void _ByteArray::check_read(std::size_t& wantlen)
        {
            std::size_t backlen=0;
            std::size_t ret=wPos-rPos;
            if(ret>0)
            {
                if(ret<wantlen)
                {
                    wantlen=ret;
                }
                else
                {
                    if(wantlen==0) wantlen=ret;
                }
                return;
            }
            else if(ret<0)
            {
                wPos=rPos=0;
            }
            wantlen=0;
            return;
        }

        void _ByteArray::check_write(std::size_t& wantlen)
        {
            std::size_t backlen=0;
            std::size_t ret=_maxSize-wPos;
            if(ret>0)
            {
                if(ret<wantlen)
                {
                    if(!(wantlen>(_maxSize-(wPos-rPos))))
                    {
                        resize();
                    }
                    else if(wantlen>_maxSize)
                    {
                        wPos=rPos=0;
                        wantlen=_maxSize;
                    }
                    else
                    {
                        wPos=rPos=0;
                    }
                }
                else
                {
                    //if(wantlen==0) wantlen=ret;
                }
                return;
            }
            else
            {
                wPos=rPos=0;
            }
            //wantlen=0;
            return;
        }

        void _ByteArray::writeByte(unsigned char val)
        {
            writeBytes(&val, 1);
        }

        void _ByteArray::writeBytes(unsigned char *val, std::size_t size, std::size_t offset)
        {
            if(size==0) size=strlen((const char*)val);
            check_write(size);
            unsigned char *desByte = _bytes;
            desByte+=wPos;
            memcpy(desByte, val+offset, size);
            wPos += size;
        }
        void _ByteArray::writeBytes(_ByteArray *val, std::size_t, std::size_t offset)
        {
            unsigned int len=val->length();
            val->readBytes(_bytes,len,0);
        }

        void _ByteArray::writeString(std::string * val)
        {
            write4BChars((char *)val->c_str());
        }

        void _ByteArray::writeChars(char * val)
        {
            writeBytes((unsigned char *)val, strlen(val), 0);
        }

        void _ByteArray::write4BChars(char * val)
        {
            std::size_t size = strlen(val);
            writeNumber(size);
            writeBytes((unsigned char *)val, size, 0);
        }

        void _ByteArray::writeBool(bool val)
        {
            writeBytes((unsigned char *)&val, 1, 0);
        }

        unsigned char _ByteArray::readByte()
        {
            unsigned char *val;
            readBytes(val, 1);
            return *val;
        }

        void _ByteArray::readBytes(unsigned char *val, std::size_t size, std::size_t offset)
        {
            check_read(size);
            //unsigned char *desByte = val + offset;
            unsigned char *srcByte = _bytes;
            srcByte+=rPos;
            memcpy(val + offset, srcByte, size);
            rPos += size;
        }
        void _ByteArray::readBytes(_ByteArray *val, std::size_t size, std::size_t offset)
        {
            unsigned int len=length();
            val->writeBytes(_bytes,len,offset);
        }

        void _ByteArray::readChars(char *val, std::size_t size, std::size_t offset)
        {
            readBytes((unsigned char*)val, size, offset);
        }

        std::string _ByteArray::readString()
        {
            std::size_t size;
            readNumber(&size);
            char chars[size + 1];
            readBytes((unsigned char*)chars, size, 0);
            std::string str(chars);
            return str;
        }

        void _ByteArray::read4BChars(char *val)
        {
            std::size_t size;
            readNumber(&size);
            readBytes((unsigned char*)val, size, 0);
        }

        void _ByteArray::readBool(bool * val)
        {
            readBytes((unsigned char*)val, 1, 0);
        }

        unsigned char* _ByteArray::get_native()
        {
            return _bytes;
        }

        void _ByteArray::setEndian(Endian writeEndian, Endian readEndian)
        {
            this->_writeEndian = writeEndian;
            this->_readEndian = readEndian;
        }

        Endian _ByteArray::getCPUEndian()
        {
            union
            {
                short s;
                char c[sizeof(short)];
            } un;
            un.s = 0x0102;
            if (un.c[0] == 0x2)
            {
                return ENDIAN_LIT;
            }
            else
            {
                return ENDIAN_LIT;
            }
        }

        void _ByteArray::reversalArray(byte * bytes, std::size_t size)
        {
            for (std::size_t i = 0; i < size / 2; i++)
            {
                byte tb;
                tb = *(bytes + i);
                *(bytes + i) = *(bytes + size - i - 1);
                *(bytes + size - i - 1) = tb;
            }
        }


    }
}
}
