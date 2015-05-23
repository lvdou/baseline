#ifndef BASELINE_UTIL_LOG_H
#define BASELINE_UTIL_LOG_H

#include <baseline/common.h>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/attributes/named_scope.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/format.hpp>

namespace baseline
{
namespace util
{

    //------- log ------------
    namespace log
    {
        static boost::log::sources::severity_logger<boost::log::trivial::severity_level> slg;
        void init();
        void set_quite_mode();
        void set_not_quite_mode();
    }

    //------------- log end -----------------

}
}

#define LOG_BASELINE_INFO  BOOST_LOG_SEV(baseline::util::log::slg, boost::log::trivial::info)
#define LOG_BASELINE_DEBUG BOOST_LOG_SEV(baseline::util::log::slg, boost::log::trivial::debug)
#define LOG_BASELINE_ERROR BOOST_LOG_SEV(baseline::util::log::slg, boost::log::trivial::error)
#define LOG_BASELINE_WARN  BOOST_LOG_SEV(baseline::util::log::slg, boost::log::trivial::warning)

#endif // UTIL_LOG_H
