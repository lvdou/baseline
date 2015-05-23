#ifndef BASELINE_MODULE_BOOT_H
#define BASELINE_MODULE_BOOT_H

#include <baseline/module/module_base.h>

namespace baseline
{
namespace module
{
    class boot
    {
    public:
        boot(){};
        ~boot(){};

        bool load_cmd_option(std::size_t argc, char* argv[]);
        bool load_config_ini(std::string& file);
        bool save_config_ini(std::string& file);
        bool check_register(){};

    private:
        bool parse_cmd_opt(std::size_t argc, char* argv[]);
        bool parse_ini_opt(std::string& file);
        bool save_ini_opt(std::string& file);
        bool get_filesystem(char* argv[]);
    };
}
}
#endif // MODULE_BOOT_H
