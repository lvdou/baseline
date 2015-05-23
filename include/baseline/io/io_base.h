#ifndef BASELINE_IO_BASE_H
#define BASELINE_IO_BASE_H


#include <baseline/common.h>
#include <baseline/util/util.h>

#include <boost/aligned_storage.hpp>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/variant.hpp>

//static
namespace baseline
{
namespace io
{
    static std::size_t t_seq=0;

    namespace type
    {
        enum{base=0,net=10,net_tcp=11,net_udp=12,net_curl=19,serial=20,file=30,cache=40,log=90};
    }

    class io  // : public boost::enable_shared_from_this<io>, public boost::static_visitor<int>
    {
	private:
		struct io_type_common{};
    public:
		typedef io_type_common c_type;
        //explicit io(int io_type): uuid_(util::uuid::by_rand()), seq_(util::uuid::u_hash_single(uuid_)){}
        explicit io(): type_(type::base), uuid_(util::uuid::by_rand())
		{
			seq_=util::uuid::u_hash_single(uuid_);
			is_running_=false;
			is_prepared_=false;
			is_stoped_=true;
			is_closed_=false;
			reading_=false;
			writing_=false;
		};
        virtual ~io(){};

		virtual void init()=0;
		std::size_t const type(){return type_;};
        std::size_t const seq(){return seq_;};
        std::string const uuid(){return uuid_;};
        bool status_running(){return is_running_;};
        bool status_prepared(){return is_prepared_;};
        bool status_stoped(){return is_stoped_;};
        bool status_closed(){return is_closed_;};
		bool status_reading(){return reading_;};
        bool status_writing(){return writing_;};

    protected:
        boost::mutex io_mutex;
		std::size_t type_;
        std::size_t seq_;
        std::string uuid_;
        bool is_running_;
        bool is_prepared_;
        bool is_stoped_;
        bool is_closed_;
        bool reading_;
        bool writing_;

    };

    typedef boost::shared_ptr<io> io_ptr;
    typedef boost::function< io *()> io_factory; //typedef boost::function< io *(int io_type) > io_factory;
    typedef std::map<std::size_t, io_ptr> io_map;
    typedef std::map<std::size_t, io_ptr>::value_type io_map_eachpair;
}
}
#endif // IO_H
