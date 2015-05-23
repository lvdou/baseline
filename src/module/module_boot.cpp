
#include <baseline/module/module_boot.h>
#include <baseline/module/module_config_file.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/program_options.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

namespace baseline
{
namespace module
{
    bool boot::load_cmd_option(std::size_t argc, char* argv[])
    {
        if(!parse_cmd_opt(argc, argv))
        {
            return false;
        }
        if(!get_filesystem(argv))
        {
            return false;
        }
        return true;
    }

    bool boot::load_config_ini(std::string& file)
    {
        if(!parse_ini_opt(file))
        {
            return false;
        }
        return true;
    }

    bool boot::save_config_ini(std::string& file)
    {
        if(!save_ini_opt(file))
        {
            return false;
        }
        return true;
    }

    bool boot::get_filesystem(char* argv[])
    {
        try
        {
            //set to app config
            config::app::appConfig &appConfig = config::app::so_appConfig::get_mutable_instance();
            appConfig.set("fs.rootPath",boost::filesystem::current_path().string());
            appConfig.set("fs.currentPath",boost::filesystem::absolute(boost::filesystem::path(argv[0]).remove_filename()).string());
            appConfig.set("fs.runfile",boost::filesystem::path(argv[0]).filename().string());
            appConfig.set("fs.tmpPath",boost::filesystem::temp_directory_path().string());

            return true;
        }
        catch (std::exception& e)
        {
            LOG_BASELINE_ERROR << "[boot] get_filesystem  error: " << e.what();
            return false;
        }
        catch(...)
        {
            LOG_BASELINE_ERROR << "[boot] get_filesystem has unknown error! ";
            return false;
        }
    }

    bool boot::parse_cmd_opt(std::size_t argc, char* argv[])
    {
        try
        {
            std::string net_ip_local;
            std::size_t net_port;

            //parse cmd args options
            boost::program_options::options_description general_options("general options");
            general_options.add_options()
                ("help,h", "produce help message");

            boost::program_options::options_description common_options("common options");
            common_options.add_options()
                ("net_ip_local", boost::program_options::value<std::string>(&net_ip_local)->default_value("127.0.0.1"), "set local ip address")
                ("net_port", boost::program_options::value<std::size_t>(&net_port)->default_value(8101), "set default net service port");

            boost::program_options::options_description all_options("All Command Options");
            all_options.add(general_options).add(common_options);

            boost::program_options::variables_map vm;
            boost::program_options::store(boost::program_options::parse_command_line(argc, argv, all_options), vm);
            boost::program_options::notify(vm);

            if(vm.count("help"))
            {
                std::cout<<all_options<<std::endl;
                return false;
            }

            if(vm.count("net_ip_local"))
                LOG_BASELINE_DEBUG<<"net_ip_local="<<vm["net_ip_local"].as<std::string>();
            else
                LOG_BASELINE_DEBUG<<"net_ip_local was not set. default = "<<net_ip_local;

            if(vm.count("net_port"))
                LOG_BASELINE_DEBUG<<"net_port="<<vm["net_port"].as<std::size_t>();
            else
                LOG_BASELINE_DEBUG<<"net_port was not set. default = "<<net_port;

            //set to app config
            config::app::appConfig &appConfig = config::app::so_appConfig::get_mutable_instance();
            appConfig.set("net.net_ip_local",net_ip_local);
            appConfig.set("net.net_port",net_port);

            return true;
        }
        catch (std::exception& e)
        {
            LOG_BASELINE_ERROR << "[boot] parse_cmd_opt error: " << e.what();
            return false;
        }
        catch(...)
        {
            LOG_BASELINE_ERROR << "[boot] parse_cmd_opt has unknown error! ";
            return false;
        }
    }

    bool boot::parse_ini_opt(std::string& file)
    {
        try
        {
            config::app::appConfig &appConfig = config::app::so_appConfig::get_mutable_instance();
            std::string currentPath=appConfig.get<std::string>("fs.currentPath", util::nullstring);
            boost::filesystem::path path1(currentPath);
            path1/="config";
            path1/=file;
            std::string pathfrom=path1.string();
            config_file config_file1;
            if(!config_file1.load_from(pathfrom, config_file::file_type_ini, appConfig))
                return false;

            return true;
        }
        catch (std::exception& e)
        {
            LOG_BASELINE_ERROR << "[boot] parse_ini_opt error: " << e.what();
            return false;
        }
        catch(...)
        {
            LOG_BASELINE_ERROR << "[boot] parse_ini_opt has unknown error! ";
            return false;
        }
    }

    bool boot::save_ini_opt(std::string& file)
    {
        try
        {
            config::app::appConfig &appConfig = config::app::so_appConfig::get_mutable_instance();
            std::string currentPath=appConfig.get<std::string>("fs.currentPath", util::nullstring);
            boost::filesystem::path path1(currentPath);
            path1/="config";
            path1/=file;
            std::string pathfrom=path1.string();
            config_file config_file;
            if(!config_file.save_to(pathfrom, config_file::file_type_ini, appConfig))
                return false;

            return true;
        }
        catch (std::exception& e)
        {
            LOG_BASELINE_ERROR << "[boot] save_ini_opt error: " << e.what();
            return false;
        }
        catch(...)
        {
            LOG_BASELINE_ERROR << "[boot] save_ini_opt has unknown error! ";
            return false;
        }
    }
}
}
