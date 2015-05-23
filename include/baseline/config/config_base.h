/*******************
example:

	try
	{
        std::string jsonstr;
        config::base::config jtest;
        jtest.load_json(jsonstr);

        LOG_BASELINE_DEBUG<<"config format: \r\n"<<jtest.printFormat();

        std::string path("sub_obj2.z.sub_obj3 string");
        LOG_BASELINE_DEBUG<<"config 'sub_obj2.z.sub_obj3 string'="<<jtest.get<std::string>("sub_obj2.z.sub_obj3 string");

        jtest.set(path,"lvdou changed.");
        path="int";
        jtest.set(path,999);

        std::string str_conf=jtest.save_json();
        LOG_BASELINE_DEBUG<<"str_conf :"<<str_conf;
	}
    catch(std::exception& err)
    {
        LOG_BASELINE_ERROR<<"[config] has error: "<<err.what();
    }
    catch(...)
    {
        LOG_BASELINE_ERROR<<"[config] has unknown error: ";
    }

*******************/



#ifndef BASELINE_CONFFIG_BASE_H_INCLUDED
#define BASELINE_CONFFIG_BASE_H_INCLUDED

#include <baseline/common.h>
#include <baseline/util/util_log.h>
#include <baseline/util/util_json.h>
//#include "../../include/config/config_tcpServer.h"
//#include "../../include/config/config_appConfig.h"

namespace baseline
{
namespace config
{
	namespace base
	{
	    using namespace util::json;

        //template<typename ObjectType, ValueType>
        class config
        {
        public:
            explicit config(){};
            virtual ~config(){};

            //template<typename Tstring>
            bool load_json(std::string &jsonstr);

            std::string save_json();

            template<typename ValueType>
            ValueType get(const std::string &path, ValueType faultValue)
            //ValueType get(const std::string &path)
            {
                boost::mutex::scoped_lock lock(io_mutex);
                try
                {
                    return util::json::get<ValueType>(path, PTree, faultValue);
                    //return PTree.get<ValueType>(path);
                }
                catch(std::exception& err)
                {
                    LOG_BASELINE_ERROR<<"[config] get() has error: "<<err.what();
                    return faultValue;
                }
                catch(...)
                {
                    LOG_BASELINE_ERROR<<"[config] get() has unknown error.";
                    return faultValue;
                }
                //return true;
            }

            boost::property_tree::ptree& get_PTree(const std::string &path)
            {
                //return &PTree;
            }

            template<typename ValueType>
            bool set(const std::string& path, ValueType value)
            {
                boost::mutex::scoped_lock lock(io_mutex);
                try
                {
                    PTree.put( path,value );
                }
                catch(std::exception& err)
                {
                    LOG_BASELINE_ERROR<<"[config] set() has error: "<<err.what();
                    return false;
                }
                catch(...)
                {
                    LOG_BASELINE_ERROR<<"[config] set() has unknown error.";
                    return false;
                }
                return true;
            }

            void remove(){};

            std::string printFormat();

        protected:
            boost::property_tree::ptree PTree;
            boost::mutex io_mutex;
        };
	}
}
}
#endif // UTIL_CONFIG_H_INCLUDED
