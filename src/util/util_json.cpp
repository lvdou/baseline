/*************************

	util::json::ptree pt;

    std::string jsonstr="......";
    util::json::json_parse(jsonstr, pt);

    std::string str_tmp;
    util::json::json_make(pt, str_tmp);
    LOG_BASELINE_DEBUG<<"pt :"<<str_tmp;

    //pair<int,const util::json::pt_root ptree&> (0, &pt);

    pt.put("int",2);
    pt.put("float",2.1f);
    pt.put("bool",false);
    pt.put("string","lvdou");
    pt.put("string",NULL);
    util::json::json_make(pt, str_tmp);
    LOG_BASELINE_DEBUG<<"new pt :"<<str_tmp;

    int x=pt.get<int>("sub_obj2.int");
    LOG_BASELINE_DEBUG<<"json sub_obj2.int: "<<pt.get<string>("sub_obj2.z.sub_obj3 string");
    LOG_BASELINE_DEBUG<<"json sub_obj2.int: "<<pt.get<int>("sub_obj2.z.sub_obj3 int");


    std::set<string> m_modules;
    util::json::ptree array1;
    std::string path="array";
    if(util::json::get_child(pt, path, array1))
    {
        BOOST_FOREACH(const util::json::ptree::value_type &v, pt)
        {
            try
            {
                //std::stringstream s;
                //boost::property_tree::json_parser::write_json(s, v.second);
                //std::string image_item = s.str();
                LOG_BASELINE_DEBUG<<"[json] first:"<<v.first <<" second:"<<v.second.data();
            }
            catch(std::exception& err)
            {
                LOG_BASELINE_ERROR<<"[json] "<< err.what();;
            }
        }

    }
    else
        LOG_BASELINE_ERROR<<"[json] no child ptree";

*************************/

#include <baseline/util/util_json.h>
#include <sstream>
#include <locale>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ini_parser.hpp>

namespace baseline
{
namespace util
{
	namespace json
	{
	    //static boost::mutex io_mutex;
		bool json_parse(const std::string& strin, ptree& ptout)
		{
		    static boost::mutex io_mutex;
			boost::mutex::scoped_lock lock(io_mutex);
			if(strin.length()<=0)return false;
			try
			{
			    //LOG_BASELINE_DEBUG<<"[json] parse: in="<<strin;
				std::istringstream isstream(strin);
				boost::property_tree::json_parser::read_json(isstream, ptout);
			}
			catch(std::exception& err)
			{
                LOG_BASELINE_ERROR<< "json parse(): has error, " << err.what();
				return false;
            }
            catch(...)
            {
                LOG_BASELINE_ERROR << "json parse(): has unknown error!\n";
				return false;
            }
			return true;
		}

		bool json_make(ptree& ptin, std::string& strout)
		{
		    static boost::mutex io_mutex;
			boost::mutex::scoped_lock lock(io_mutex);
			//if(&ptin)return false;
			try
			{
			    //LOG_BASELINE_DEBUG<<"[json] make: in="<<strout;
				std::ostringstream osstream;
				boost::property_tree::json_parser::write_json(osstream, ptin);
				strout=osstream.str();
			}
			catch(std::exception& err)
			{
                LOG_BASELINE_ERROR<< "json make(): has error, " << err.what();
				return false;
            }
            catch(...)
            {
                LOG_BASELINE_ERROR << "json make(): has unknown error!\n";
				return false;
            }
			return true;
		}

		bool xml_parse(const std::string& strin, ptree& ptout)
		{
		    static boost::mutex io_mutex;
			boost::mutex::scoped_lock lock(io_mutex);
			if(strin.length()<=0)return false;
			try
			{
			    //LOG_BASELINE_DEBUG<<"[json] parse: in="<<strin;
				std::istringstream isstream(strin);
				boost::property_tree::xml_parser::read_xml(isstream, ptout);
			}
			catch(std::exception& err)
			{
                LOG_BASELINE_ERROR<< "xml parse(): has error, " << err.what();
				return false;
            }
            catch(...)
            {
                LOG_BASELINE_ERROR << "xml parse(): has unknown error!\n";
				return false;
            }
			return true;
		}
		bool xml_make(ptree& ptin, std::string& strout)
		{
		    static boost::mutex io_mutex;
			boost::mutex::scoped_lock lock(io_mutex);
			//if(&ptin)return false;
			try
			{
			    //LOG_BASELINE_DEBUG<<"[json] make: in="<<strout;
				std::ostringstream osstream;
				boost::property_tree::xml_parser::write_xml(osstream, ptin);
				strout=osstream.str();
			}
			catch(std::exception& err)
			{
                LOG_BASELINE_ERROR<< "xml make(): has error, " << err.what();
				return false;
            }
            catch(...)
            {
                LOG_BASELINE_ERROR << "xml make(): has unknown error!\n";
				return false;
            }
			return true;
		}

		bool ini_parse(const std::string& strin, ptree& ptout)
		{
		    static boost::mutex io_mutex;
			boost::mutex::scoped_lock lock(io_mutex);
			if(strin.length()<=0)return false;
			try
			{
			    //LOG_BASELINE_DEBUG<<"[json] parse: in="<<strin;
				std::istringstream isstream(strin);
				boost::property_tree::ini_parser::read_ini(isstream, ptout);
			}
			catch(std::exception& err)
			{
                LOG_BASELINE_ERROR<< "ini parse(): has error, " << err.what();
				return false;
            }
            catch(...)
            {
                LOG_BASELINE_ERROR << "ini parse(): has unknown error!\n";
				return false;
            }
			return true;
		}
		bool ini_make(ptree& ptin, std::string& strout)
		{
		    static boost::mutex io_mutex;
			boost::mutex::scoped_lock lock(io_mutex);
			//if(&ptin)return false;
			try
			{
			    //LOG_BASELINE_DEBUG<<"[json] make: in="<<strout;
				std::ostringstream osstream;
				boost::property_tree::ini_parser::write_ini(osstream, ptin);
				strout=osstream.str();
			}
			catch(std::exception& err)
			{
                LOG_BASELINE_ERROR<< "ini make(): has error, " << err.what();
				return false;
            }
            catch(...)
            {
                LOG_BASELINE_ERROR << "ini make(): has unknown error!\n";
				return false;
            }
			return true;
		}

        bool get_child(ptree& ptin, const std::string& path, ptree& ptout)
        {
            static boost::mutex io_mutex;
			boost::mutex::scoped_lock lock(io_mutex);
			//if(&ptin)return false;
			try
			{
			     ptout = ptin.get_child(path);
			     return true;
			}
			catch(std::exception& err)
			{
                LOG_BASELINE_ERROR<< "json get_child(): has error, " << err.what();
				//return (ptree&)NULL;
				return false;
            }
            catch(...)
            {
                LOG_BASELINE_ERROR << "json get_child(): has unknown error!\n";
				//return (ptree&)NULL;
				return false;
            }
			//return (ptree&)NULL;
			return false;
        }


	}
}
}
