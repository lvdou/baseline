#ifndef BASELINE_MODULE_CONFIG_FILE_H
#define BASELINE_MODULE_CONFIG_FILE_H

#include <baseline/module/module_base.h>

namespace baseline
{
namespace module
{
    class config_file
    {
        public:
            enum file_type{file_type_json=1,file_type_ini,file_type_xml};

            config_file(){};
            virtual ~config_file(){};

            bool load_from(std::string& fromfile, std::size_t file_type, config::base::config& config);
            bool save_to(std::string& tofile, std::size_t file_type, config::base::config& config);

        protected:
        private:
            boost::mutex io_mutex;
    };
}
}
#endif // MODULE_CONFIG_FILE_H
