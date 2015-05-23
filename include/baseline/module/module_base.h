#ifndef BASELINE_MODULE_BASE_H
#define BASELINE_MODULE_BASE_H

#include <baseline/common.h>
#include <boost/aligned_storage.hpp>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/variant.hpp>
#include <baseline/util/util.h>
#include <baseline/config/config_base.h>
#include <baseline/config/config_appConfig.h>

namespace baseline
{
namespace module
{
    static std::size_t t_seq=0;

    namespace type
    {
        enum{base=0,net=10};
    }

    class module
    {
        public:
            //explicit module(int type): seq_(++t_seq), type_(type), uuid_ (util::so_util_uuid::get_mutable_instance().getUUID()){}
            explicit module(...): seq_(++t_seq), type_(type::base), uuid_ (util::uuid::by_rand()){}
            virtual ~module(){};
            virtual std::size_t const type()=0;
            virtual std::size_t const seq()=0;
            virtual std::string const uuid()=0;
            virtual bool status_is_running()=0;
            virtual bool status_is_prepared()=0;
            virtual bool status_is_stoped()=0;
            virtual bool status_is_closed()=0;

        protected:
            std::size_t type_;
            std::size_t seq_;
            std::string uuid_;
            bool is_running_;
            bool is_prepared_;
            bool is_stoped_;
            bool is_closed_;

        private:
    };

    typedef std::shared_ptr<module> module_ptr;
    typedef boost::function< module *()> module_factory; //typedef boost::function< io *(int io_type) > io_factory;
    typedef std::map<std::size_t, module_ptr> module_map;
    typedef std::map<std::size_t, module_ptr>::value_type module_map_eachpair;
}
}
#endif // MODULE_H
