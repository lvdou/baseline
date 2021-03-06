
#include "../../include/io/io_log.h"
#include <boost/log/expressions.hpp>
#include <boost/log/expressions/keyword.hpp>
#include <boost/log/common.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/attributes/timer.hpp>
#include <boost/log/attributes/named_scope.hpp>
#include <boost/log/attributes/scoped_attribute.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

namespace io
{
    //log
    namespace log
    {
        static int vInited=0;
        typedef boost::log::sinks::synchronous_sink< boost::log::sinks::text_ostream_backend > console_sink;
        typedef boost::log::sinks::synchronous_sink< boost::log::sinks::text_file_backend> text_sink;

        void init()
        {
            if(vInited)
                return;
            boost::log::formatter formatter= boost::log::expressions::format("%1% | %2% | %3% | %4% | %5%")
                 % boost::log::expressions::attr<unsigned int>("LineID")
                 % boost::log::expressions::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d %H:%M:%S")
                 % boost::log::expressions::attr<boost::log::attributes::current_thread_id::value_type >("ThreadID")
                 % boost::log::trivial::severity
                 % boost::log::expressions::smessage;
            boost::log::add_common_attributes();
            boost::shared_ptr< console_sink > console_sink=boost::log::add_console_log();
            /*
            boost::shared_ptr< text_sink > file_sink=boost::log::add_file_log
            (
                boost::log::keywords::file_name="/tmp/transplus/%Y-%m-%d_%4N_%H:%M:%S.log",      //文件名
                //boost::log::keywords::rotation_size=10*1024*1024,       //单个文件限制大小1M
                boost::log::keywords::time_based_rotation=boost::log::sinks::file::rotation_at_time_point(boost::date_time::Monday,0,0,0)    //每天重建
                //boost::log::keywords::time_based_rotation=boost::log::sinks::file::rotation_at_time_interval(boost::posix_time::minutes(1))
                //boost::log::keywords::target="logs",        //文件夹名
                //boost::log::keywords::max_size=50*1024*1024,    //文件夹所占最大空间50M
                //boost::log::keywords::min_free_space=100*1024*1024  //磁盘最小预留空间100M
            );
            file_sink->locked_backend()->set_file_collector(boost::log::sinks::file::make_collector(
                boost::log::keywords::target="logs",        //文件夹名
                boost::log::keywords::max_size=50*1024*1024,    //文件夹所占最大空间50M
                boost::log::keywords::min_free_space=100*1024*1024  //磁盘最小预留空间100M
            ));
            */
            //file_sink->set_filter(boost::log::expressions::attr< boost::log::trivial::severity_level >("Severity") <= boost::log::trivial::warning);
            //file_sink->locked_backend()->scan_for_files();
            //file_sink->set_formatter(formatter);
            console_sink->set_formatter(formatter);

            //file_sink->locked_backend()->auto_flush(true);
            console_sink->locked_backend()->auto_flush(false);
            //boost::log::core::get()->add_sink(file_sink);
            boost::log::core::get()->add_sink(console_sink);
            ++vInited;
        }

        void set_quite_mode()
        {
            boost::log::core::get()->set_filter(boost::log::expressions::attr< boost::log::trivial::severity_level >("Severity") >= boost::log::trivial::warning);
        }
        void set_not_quite_mode()
        {
            boost::log::core::get()->set_filter(boost::log::expressions::attr< boost::log::trivial::severity_level >("Severity") <= boost::log::trivial::error);
        }
    }
}
