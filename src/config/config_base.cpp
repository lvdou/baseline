
#include <baseline/config/config_base.h>

namespace baseline
{
namespace config
{
	namespace base
	{
            bool config::load_json(std::string &jsonstr)
            {
                boost::mutex::scoped_lock lock(io_mutex);
                try
                {
                    LOG_BASELINE_DEBUG<<"load json: "<<jsonstr;
                    if(util::json::json_parse(jsonstr, PTree))
                        return true;
                    return false;
                }
                catch(...)
                {
                    return false;
                }
            }

            std::string config::save_json()
            {
                boost::mutex::scoped_lock lock(io_mutex);
                std::string str;
                util::json::json_make(PTree, str);
                return str;
            }

            std::string config::printFormat()
            {
                //std::cout<<pair<int,const ptree&,> (0, PTree) std::endl;
                boost::mutex::scoped_lock lock(io_mutex);
                try
                {
                    std::string fmt;
                    util::json::json_make(PTree, fmt);
                    return fmt;
                }
                catch(std::exception& err)
                {
                    LOG_BASELINE_ERROR<<"[config] printFormat() has error: "<<err.what();
                    throw err.what();
                }
                catch(...)
                {
                    LOG_BASELINE_ERROR<<"[config] printFormat() has unknown error.";
                    throw "[config] printFormat() has unknown error!";
                }
            }
	}
}
}
