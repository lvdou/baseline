
#include <baseline/util/util_time.h>
#include <boost/date_time.hpp>
#include  <chrono>
#include  <iomanip>

namespace baseline
{
namespace util
{
    namespace time
    {
        //static boost::mutex time_mutex;
		//static boost::mutex timenow_mutex;

        uint64_t getclock_us()
        {
            static boost::mutex time_mutex;
            boost::mutex::scoped_lock lock(time_mutex);
            return (uint64_t)std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            /*
            #ifdef _WIN32
                struct _timeb tb;
                _ftime(&tb);
                return (tb.time * 1000) + tb.millitm;
            #else
                struct timeval tv;
                gettimeofday(&tv, NULL);
                return (tv.tv_sec <<32) + tv.tv_usec;
            #endif
            */
        }
        std::time_t now()
        {
            static boost::mutex time_mutex;
            boost::mutex::scoped_lock lock(time_mutex);
            /*
            boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
            //LOG_BASELINE_DEBUG<<"[now] get local ptime: "<<boost::posix_time::to_iso_extended_string(now);
            std::tm tm1=boost::posix_time::to_tm(now);

            //boost::posix_time::ptime pin = boost::posix_time::from_time_t(mktime(&tm1));
            //pin+=boost::posix_time::hours(8);
            //LOG_BASELINE_DEBUG<<"[now] load local ptime: "<<boost::posix_time::to_iso_extended_string(pin);
            return mktime(&tm1);
            */
            return std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        }

        std::time_t make(char* time)
        {
            static boost::mutex time_mutex;
            boost::mutex::scoped_lock lock(time_mutex);
            try
            {
                if(!time) return 0;
                std::time_t t = std::time(NULL);

                /* this c++11 only gcc 5.1 support
                std::stringstream ss(time);
                ss.imbue(std::locale("en_US.UTF8"));
                ss >> std::get_time(&t, "%Y-%b-%d %H:%M:%S");
                return mktime(&t);
                */

                boost::posix_time::ptime pt = boost::posix_time::time_from_string(time);
                std::tm tm1=boost::posix_time::to_tm(pt);
                return mktime(&tm1);
            }
            catch(std::exception& err)
            {
                //LOG_BASELINE_ERROR<< "time_make: "<< err.what();
                return 0;
            }
            catch(...)
            {
                //LOG_BASELINE_ERROR<< "time_make: unknown error!";
                return 0;
            }
            return 0;
        }

        std::time_t make(char* date, char* stime)
        {
            static boost::mutex time_mutex;
            boost::mutex::scoped_lock lock(time_mutex);
            try
            {
                if((!date) || (!stime))
                    return 0;
                //LOG_BASELINE_DEBUG << "in date:"<< date <<" time:" <<stime;
                std::string sdate;
                sdate += date;
                sdate += " ";
                sdate += stime;
                if(strlen(stime)<=5)
                    sdate += ":00";

                /* this c++11 only gcc 5.1 support
                std::tm t;
                std::stringstream ss(sdate);
                ss.imbue(std::locale("en_US.UTF8"));
                ss >> std::get_time(&t, "%Y-%b-%d %H:%M:%S");
                return mktime(&t);
                */

                boost::posix_time::ptime pt = boost::posix_time::time_from_string(sdate);
                std::tm tm1=boost::posix_time::to_tm(pt);
                return mktime(&tm1);
            }
            catch(std::exception& err)
            {
                //LOG_BASELINE_ERROR<< "time_make: "<< err.what();
                return 0;
            }
            catch(...)
            {
                //LOG_BASELINE_ERROR<< "time_make: unknown error!";
                return 0;
            }
            return 0;
        }
        int equip(std::time_t intime)
        {
            static boost::mutex time_mutex;
            boost::mutex::scoped_lock lock(time_mutex);
            try
            {
                if(!intime)
                    return -1;
                int s_gap=0;
                std::chrono::time_point<std::chrono::system_clock> pin = std::chrono::system_clock::from_time_t(intime);
                std::chrono::time_point<std::chrono::system_clock> pnow = std::chrono::system_clock::now();
                s_gap =  std::chrono::duration_cast<std::chrono::seconds>( pnow - pin ).count();
                if(s_gap+10>0)
                    return s_gap;
                else if (s_gap<0)
                    return s_gap;
                else
                    return 0;
            }
            catch(std::exception& err)
            {
                //LOG_BASELINE_ERROR<< "time_equip: "<< err.what();
                return -1;
            }
            catch(...)
            {
                //LOG_BASELINE_ERROR<< "time_equip: unknown error!";
                return -1;
            }
            return -1;
        }
        int equip(std::time_t intime, std::time_t nowtime)
        {
            static boost::mutex time_mutex;
            boost::mutex::scoped_lock lock(time_mutex);
            try
            {
                if(!intime)
                    return -1;
                int s_gap=0;
                std::chrono::time_point<std::chrono::system_clock> pin = std::chrono::system_clock::from_time_t(intime);
                std::chrono::time_point<std::chrono::system_clock> pnow = std::chrono::system_clock::from_time_t(nowtime);
                s_gap =  std::chrono::duration_cast<std::chrono::seconds>( pin - pnow ).count();
                return s_gap;
            }
            catch(std::exception& err)
            {
                //LOG_BASELINE_ERROR<< "time_equip: "<< err.what();
                return -1;
            }
            catch(...)
            {
                //LOG_BASELINE_ERROR<< "time_equip: unknown error!";
                return -1;
            }
            return -1;
        }
        std::time_t add(std::time_t intime, int addseconds)
        {
            static boost::mutex time_mutex;
            boost::mutex::scoped_lock lock(time_mutex);
            try
            {
                if(!intime)
                    return 0;
                std::chrono::time_point<std::chrono::system_clock> pin = std::chrono::system_clock::from_time_t(intime);
                pin += std::chrono::seconds(addseconds);
                std::time_t t_pin=std::chrono::system_clock::to_time_t(pin);
                return mktime(std::localtime(&t_pin));
            }
            catch(std::exception& err)
            {
                //LOG_BASELINE_ERROR<< "time_equip: "<< err.what();
                return 0;
            }
            catch(...)
            {
                //LOG_BASELINE_ERROR<< "time_equip: unknown error!";
                return 0;
            }
            return 0;
        }
        std::string tostring(std::time_t intime)
        {
            static boost::mutex time_mutex;
            boost::mutex::scoped_lock lock(time_mutex);
            try
            {
                if(!intime)
                    return 0;

                /* this c++11 only gcc 5.1 support
                std::chrono::time_point<std::chrono::system_clock> pin = std::chrono::system_clock::from_time_t(intime);
                std::stringstream ss;
                ss.imbue(std::locale("en_US.UTF8"));
                ss << std::put_time(std::localtime(&intime),"%Y-%b-%d %H:%M:%S");
                std::string str;
                ss >> str;
                return str;
                */

                boost::posix_time::ptime pin = boost::posix_time::from_time_t(intime);
                pin += boost::posix_time::hours(8);
                std::string result = boost::posix_time::to_iso_extended_string(pin);
                if(result.size()>50)
                    result = result.substr(0, 50);
                result.replace(result.begin()+10,result.begin()+11," ");
                return result;
            }
            catch(std::exception& err)
            {
                //LOG_BASELINE_ERROR<< "time_equip: "<< err.what();
                return NULL;
            }
            catch(...)
            {
                //LOG_BASELINE_ERROR<< "time_equip: unknown error!";
                return NULL;
            }
        }

        //timer{}
        timer::timer(): start_time_(std::chrono::high_resolution_clock::now()){}
        timer::~timer(){}
        void timer::restart(){start_time_ = std::chrono::high_resolution_clock::now();}

        int64_t timer::elapsed() const
        {
            return std::chrono::duration_cast<std::chrono::milliseconds> (std::chrono::high_resolution_clock::now() - start_time_).count();
        }

        int64_t timer::elapsed_micro() const
        {
            return std::chrono::duration_cast<std::chrono::microseconds> (std::chrono::high_resolution_clock::now() - start_time_).count();
        }

        int64_t timer::elapsed_nano() const
        {
            return std::chrono::duration_cast<std::chrono::nanoseconds> (std::chrono::high_resolution_clock::now() - start_time_).count();
        }

        int64_t timer::elapsed_seconds() const
        {
            return std::chrono::duration_cast<std::chrono::seconds> (std::chrono::high_resolution_clock::now() - start_time_).count();
        }

        int64_t timer::elapsed_minutes() const
        {
            return std::chrono::duration_cast<std::chrono::minutes> (std::chrono::high_resolution_clock::now() - start_time_).count();
        }

        int64_t timer::elapsed_hours() const
        {
            return std::chrono::duration_cast<std::chrono::hours> (std::chrono::high_resolution_clock::now() - start_time_).count();
        }
    }



}
}

