
#include <baseline/util/util_crc.h>
#include <boost/crc.hpp>

namespace baseline
{
namespace util
{
    namespace crc
    {
        uint8_t CRC5_usb( char * indata, int inlen )
        {
            boost::mutex::scoped_lock lock(crc_mutex);
            try
            {
                boost::crc_optimal<5, 0x05, 0x1F, 0x1F, true, true>  crc;
                crc.process_bytes(indata, inlen);
                //crc = std::for_each( indata, indata + inlen, crc );
                return (uint8_t)crc();
            }
            catch(...)
            {
                return 0;
            }
        }
        uint8_t CRC7( char * indata, int inlen )
        {
            boost::mutex::scoped_lock lock(crc_mutex);
            try
            {
                boost::crc_optimal<7, 0x09, 0, 0, false, false>  crc;
                crc.process_bytes(indata, inlen);
                //crc = std::for_each( indata, indata + inlen, crc );
                return (uint8_t)crc();
            }
            catch(...)
            {
                return 0;
            }
        }
        uint8_t CRC8_maxim( char * indata, int inlen )
        {
            boost::mutex::scoped_lock lock(crc_mutex);
            try
            {
                boost::crc_optimal<8, 0x31, 0, 0, true, true>  crc;
                crc.process_bytes(indata, inlen);
                //crc = std::for_each( indata, indata + inlen, crc );
                return (uint8_t)crc();
            }
            catch(...)
            {
                return 0;
            }
        }
        uint8_t CRC8_ccitt( char * indata, int inlen )
        {
            boost::mutex::scoped_lock lock(crc_mutex);
            try
            {
                boost::crc_optimal<8, 0x07, 0xFF, 0, true, true>  crc;
                crc.process_bytes(indata, inlen);
                //crc = std::for_each( indata, indata + inlen, crc );
                return (uint8_t)crc();
            }
            catch(...)
            {
                return 0;
            }
        }
        uint8_t CRC8_atm( char * indata, int inlen )
        {
            boost::mutex::scoped_lock lock(crc_mutex);
            try
            {
                boost::crc_optimal<8, 0x07, 0, 0, false, false>  crc;
                crc.process_bytes(indata, inlen);
                //crc = std::for_each( indata, indata + inlen, crc );
                return (uint8_t)crc();
            }
            catch(...)
            {
                return 0;
            }
        }
        uint16_t CRC16( char * indata, int inlen )
        {
            boost::mutex::scoped_lock lock(crc_mutex);
            try
            {
                //boost::crc_optimal<16, 0x8005, 0, 0, true, true> crc;
                boost::crc_16_type  crc;
                crc.process_bytes(indata, inlen);
                //crc = std::for_each( indata, indata + inlen, crc );
                return (uint16_t)crc();
            }
            catch(...)
            {
                return 0;
            }
        }
        uint16_t CRC16_ccitt( char * indata, int inlen )
        {
            boost::mutex::scoped_lock lock(crc_mutex);
            try
            {
                //boost::crc_optimal<16, 0x1021, 0xFFFF, 0, false, false>  crc;
                boost::crc_ccitt_type crc;
                crc.process_bytes(indata, inlen);
                //crc = std::for_each( indata, indata + inlen, crc );
                return (uint16_t)crc();
            }
            catch(...)
            {
                return 0;
            }
        }
        uint16_t CRC16_xmodem( char * indata, int inlen )
        {
            boost::mutex::scoped_lock lock(crc_mutex);
            try
            {
                //boost::crc_optimal<16, 0x8408, 0, 0, true, true>  crc;
                boost::crc_xmodem_type crc;
                crc.process_bytes(indata, inlen);
                //crc = std::for_each( indata, indata + inlen, crc );
                return (uint16_t)crc();
            }
            catch(...)
            {
                return 0;
            }
        }
        uint16_t CRC16_usb( char * indata, int inlen )
        {
            boost::mutex::scoped_lock lock(crc_mutex);
            try
            {
                boost::crc_optimal <16, 0x8005, 0xFFFF, 0xFFFF, true, true> crc ;
                crc.process_bytes(indata, inlen);
                //crc = std::for_each( indata, indata + inlen, crc );
                return (uint16_t)crc();
            }
            catch(...)
            {
                return 0;
            }
        }
        uint32_t CRC32( char * indata, int inlen )
        {
            boost::mutex::scoped_lock lock(crc_mutex);
            try
            {
                //boost::crc_optimal<32, 0x04C11DB7, 0xFFFFFFFF, 0xFFFFFFFF, true, true>  crc;
                boost::crc_32_type crc;
                crc.process_bytes(indata, inlen);
                //crc = std::for_each( indata, indata + inlen, crc );
                return (uint32_t)crc();
            }
            catch(...)
            {
                return 0;
            }
        }
        uint32_t CRC32_mpeg2( char * indata, int inlen )
        {
            boost::mutex::scoped_lock lock(crc_mutex);
            try
            {
                boost::crc_optimal<32, 0x04C11DB7, 0xFFFFFFFF, 0, false, false>  crc;
                crc.process_bytes(indata, inlen);
                //crc = std::for_each( indata, indata + inlen, crc );
                return (uint32_t)crc();
            }
            catch(...)
            {
                return 0;
            }
        }
    }

}
}
