#ifndef BASELINE_UTIL_TIME_H
#define BASELINE_UTIL_TIME_H

#include <baseline/util/util_base.h>
//#include <boost/date_time/posix_time/posix_time.hpp>

namespace baseline
{
namespace util
{
    namespace time //: private boost::noncopyable
    {
        uint64_t getclock_us();
        std::time_t now();
        std::time_t make(char* date, char* stime);
        std::time_t make(char* time);  //"%Y-%b-%d %H:%M:%S"
        int equip(std::time_t intime);
        int equip(std::time_t intime, std::time_t nowtime);
        std::time_t add(std::time_t intime, int addseconds);
        std::string tostring(std::time_t intime);

        class timer
        {
        public:
		            timer();
		            ~timer();
		            void restart();

		            int64_t elapsed_nano() const;
		            int64_t elapsed_micro() const;
		            int64_t elapsed() const;
		            int64_t elapsed_seconds() const;
		            int64_t elapsed_minutes() const;
		            int64_t elapsed_hours() const;

        private:
		            std::chrono::high_resolution_clock::time_point start_time_;
        };
    };


    //typedef boost::serialization::singleton<crc> so_util_crc;
    //typedef boost::serialization::singleton<b2c> so_util_b2c;
    //typedef boost::serialization::singleton<time> so_util_time;
}

}

#endif // UTIL_H
