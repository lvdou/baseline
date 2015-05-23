#ifndef BASELINE_UTIL_BYTEARRAY_H
#define BASELINE_UTIL_BYTEARRAY_H

#include <baseline/common.h>
#include <baseline/util/util_log.h>

namespace baseline
{
namespace util
{
    namespace hex //: private boost::noncopyable
    {
        enum Endian{ ENDIAN_LIT=0, ENDIAN_BIG, ENDIAN_IGNORE };

        class _ByteArray  //default BIG_ENDIAN
        {
        private:
            unsigned char *_bytes;

            Endian _writeEndian;
            Endian _readEndian;
            Endian _sysEndian;

            std::size_t wPos;
            std::size_t rPos;
            std::size_t _maxSize;

            void check_read(std::size_t& wantlen);
            void check_write(std::size_t& wantlen);
            void resize();

        public:
            _ByteArray(std::size_t maxSize=512);
            virtual ~_ByteArray();

            //-----Write-------------
            void writeByte(unsigned char val);
            void writeBytes(unsigned char *val, std::size_t size=0, std::size_t offset = 0);
            void writeBytes(_ByteArray *val, std::size_t size=0, std::size_t offset = 0);
            template <class Number>
            std::size_t writeNumber(Number val)
            {
                unsigned char bytes[sizeof(val)];
                std::size_t size = numberToBytes(val, bytes, _writeEndian);
                writeBytes(bytes, size);
                return size;
            }

            void writeString(std::string *val);
            void writeChars(char * val);
            void write4BChars(char * val);
            void writeBool(bool val);

            //-----Read-------------
            int readInt();
            unsigned char readByte();
            void readBytes(unsigned char *val, std::size_t size=0, std::size_t offset = 0);
            void readBytes(_ByteArray *val, std::size_t size=0, std::size_t offset = 0);
            template <class Number>
            std::size_t readNumber(Number *val)
            {
                std::size_t size = sizeof(*val);
                unsigned char bytes[size];
                readBytes(bytes, size);
                bytesToNumber(val, bytes, _readEndian);
                return size;
            }

            void readChars(char * val, std::size_t size, std::size_t offset = 0);
            void read4BChars(char * val);
            std::string readString();
            void readBool(bool * val);

            //-----Utils-------------
            unsigned char & operator[] (std::size_t index) //std::ptrdiff_t
            {
                BOOST_ASSERT(_bytes != 0);
                BOOST_ASSERT(index >= 0);
                return _bytes[rPos+index];
            }
            const unsigned char & operator[] (std::size_t index) const //std::ptrdiff_t
            {
                BOOST_ASSERT(_bytes != 0);
                BOOST_ASSERT(index >= 0);
                return _bytes[rPos+index];
            }

            std::size_t length();
            std::size_t length_available();

            void reset();
            unsigned char* get_native();

            void setEndian(Endian writeEndian, Endian readEndian);

            template <class T>
            std::size_t numberToBytes(T val, unsigned char *bytes, Endian endian)
            {
                std::size_t size = sizeof(val);

                *(T*) bytes = val;

                if(!(endian == ENDIAN_IGNORE || endian == _sysEndian))
                {
                    reversalArray(bytes, size);
                }
                return size;
            }
            template <class T>
            std::size_t bytesToNumber(T val, unsigned char *bytes, Endian endian)
            {
                std::size_t size = sizeof(*val);

                if(!(endian == ENDIAN_IGNORE || endian == _sysEndian))
                {
                    reversalArray(bytes, size);
                }

                *val = *(T)bytes;
                return size;
            }

            //-----Static-------------
            static Endian getCPUEndian();
            static void reversalArray(unsigned char * bytes, std::size_t size);
        };

    }
}

typedef util::hex::_ByteArray ByteArray;
}

#define ByteArray baseline::ByteArray

#endif // UTIL_BYTEARRAY_H
