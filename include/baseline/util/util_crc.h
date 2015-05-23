#ifndef BASELINE_UTIL_CRC_H
#define BASELINE_UTIL_CRC_H

#include <baseline/util/util_base.h>

#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/string_generator.hpp>

namespace baseline
{
namespace util
{
    namespace crc //: private boost::noncopyable
    {
		static boost::mutex crc_mutex;
        uint8_t CRC5_usb( char * indata, int inlen ); //usb-token
        uint8_t CRC7( char * indata, int inlen ); //multimedia card, SD
        uint8_t CRC8_atm( char * indata, int inlen ); //ATM
        uint8_t CRC8_rohc( char * indata, int inlen ); //
        uint8_t CRC8_ccitt( char * indata, int inlen ); // ???
        uint8_t CRC8_maxim( char * indata, int inlen ); // 1-wire bus
        uint16_t CRC16( char * indata, int inlen ); //CRC-16-IBM
        uint16_t CRC16_ccitt( char * indata, int inlen ); //X25, V.41, Bluetooth, PPP, IrDA,
        uint16_t CRC16_xmodem( char * indata, int inlen ); //XMODEM
        uint16_t CRC16_usb( char * indata, int inlen ); //usb
        uint32_t CRC32( char * indata, int inlen ); //ZIP, RAR, IEEE 802 LAN/FDDI, IEEE 1394, PPP-FCS
        uint32_t CRC32_mpeg2( char * indata, int inlen ); //mpeg2
    };

    //typedef boost::serialization::singleton<crc> so_util_crc;
    //typedef boost::serialization::singleton<b2c> so_util_b2c;
    //typedef boost::serialization::singleton<time> so_util_time;
}

}

#endif // UTIL_H
