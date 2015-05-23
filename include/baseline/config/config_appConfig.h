/************************
example:

    config::app::appConfig &conf_appConfig = config::app::so_appConfig::get_mutable_instance();

    std::string fmt_appConfig=conf_appConfig.save_json();
    LOG_BASELINE_DEBUG<<"appConfig:\r\n"<<fmt_appConfig;

    conf_appConfig.set("is_big_endian",true);
    conf_appConfig.set("is_bits",64);
    fmt_appConfig=conf_appConfig.save_json();

    LOG_BASELINE_DEBUG<<"appConfig:\r\n"<<fmt_appConfig;

************************/


#ifndef BASELINE_CONFIG_APPCONFIG_H_INCLUDED
#define BASELINE_CONFIG_APPCONFIG_H_INCLUDED

#include <baseline/config/config_base.h>
#include <baseline/util/util.h>

namespace baseline
{
namespace config
{
	namespace app
	{
	    using namespace baseline::config::base;

		class appConfig : public config
		{
		public:
		    bool is_big_endian;
		    std::size_t is_bits;
		    //std::string g_locale;

			appConfig():is_bits(0),is_big_endian(false)
			{
                clear();
			}

            void clear()
            {
                set("is_big_endian",check_big());
                set("is_bits",util::is_bits());
                set("locale","en_US.UTF8");
            }

        private:
            const char array_big_1[2]={0x00,0x01};
            bool check_big()
            {
                uint16_t vint=1;
                if((uint16_t&)array_big_1==vint)
                    return true;
                return false;
            }
		};

		typedef boost::serialization::singleton<appConfig> so_appConfig;
	}
}
}
#endif // CONFIG_APPCONFIG_H_INCLUDED
