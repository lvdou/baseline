#ifndef BASELINE_UTIL_JSON_H
#define BASELINE_UTIL_JSON_H

#pragma once

//#include "../../include/util/util_json_seasy.h"
//#include "../../include/util/util_json_auto.h"    //not completed
#include <baseline/util/util_base.h>
#include <boost/property_tree/ptree.hpp>

namespace baseline
{
namespace util
{
	namespace json
	{
		typedef boost::property_tree::ptree ptree;
		typedef boost::property_tree::ptree::value_type value_type;
		//typedef boost::property_tree::const_iterator iterator;

		bool json_parse(const std::string& strin, ptree& ptout);
		bool json_make(ptree& ptin, std::string& strout);

		bool xml_parse(const std::string& strin, ptree& ptout);
		bool xml_make(ptree& ptin, std::string& strout);

		bool ini_parse(const std::string& strin, ptree& ptout);
		bool ini_make(ptree& ptin, std::string& strout);

		bool get_child(ptree& ptin, const std::string& path, ptree& ptout);

        template<typename ValueType>
        ValueType get(const std::string& path, ptree& ptin, ValueType faultValue)
        {
            static boost::mutex io_mutex;
            boost::mutex::scoped_lock lock(io_mutex);
            try
            {
                return ptin.get<ValueType>(path, faultValue);
            }
            catch(std::exception& err)
            {
                LOG_BASELINE_ERROR<<"[json] get() has error: "<<err.what();
                return faultValue;
            }
            catch(...)
            {
                LOG_BASELINE_ERROR<<"[json] get() has unknown error.";
                return faultValue;
            }
                //return true;
        }

        template<typename ValueType>
        bool set(const std::string& path, ValueType value, ptree& ptin)
        {
            static boost::mutex io_mutex;
            boost::mutex::scoped_lock lock(io_mutex);
            try
            {
                ptin.put( path,value );
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

	}

}
}
#endif //UTIL_JSON_H
