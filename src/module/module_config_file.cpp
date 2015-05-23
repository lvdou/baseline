
#include <baseline/module/module_config_file.h>

namespace baseline
{
namespace module
{
    bool config_file::load_from(std::string& fromfile, std::size_t file_type, config::base::config& config)
    {
        try
        {
            util::json::ptree PTree;
            if(file_type==file_type_json)
            {
                if(!util::json::json_parse(fromfile, PTree))
                    return false;
            }
            else if(file_type==file_type_ini)
            {
                if(!util::json::ini_parse(fromfile, PTree))
                    return false;
            }
            else if(file_type==file_type_xml)
            {
                if(!util::json::xml_parse(fromfile, PTree))
                    return false;
            }
            else
                return false;

            std::string str;
            util::json::json_make(PTree, str);
            config.load_json(str);

        }
        catch(std::exception& err)
        {
            LOG_BASELINE_ERROR<<"[module] config_file::load_from() has error: "<<err.what();
            return false;
        }
        catch(...)
        {
            LOG_BASELINE_ERROR<<"[module] config_file::load_from() has unknown error.";
            return false;
        }
        return true;
    }

    bool config_file::save_to(std::string& tofile, std::size_t file_type, config::base::config& config)
    {
        try
        {
            util::json::ptree PTree;
            std::string str=config.save_json();
            util::json::json_parse(str,PTree);

            if(file_type==file_type_json)
            {
                if(!util::json::json_make(PTree, tofile))
                    return false;
            }
            else if(file_type==file_type_ini)
            {
                if(!util::json::ini_make(PTree, tofile))
                    return false;
            }
            else if(file_type==file_type_xml)
            {
                if(!util::json::xml_make(PTree, tofile))
                    return false;
            }
            else
                return false;
        }
        catch(std::exception& err)
        {
            LOG_BASELINE_ERROR<<"[module] config_file::save_to() has error: "<<err.what();
            return false;
        }
        catch(...)
        {
            LOG_BASELINE_ERROR<<"[module] config_file::save_to() has unknown error.";
            return false;
        }
        return true;
    }
}
}
