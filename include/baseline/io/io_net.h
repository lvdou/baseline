#ifndef BASELINE_IO_NET_H
#define BASELINE_IO_NET_H

#include <baseline/io/io_base.h>

namespace baseline
{
namespace io
{
    class io_net : public io
    {
			private:
				struct io_type_net{};
			public:
				typedef io_type_net c_type;
            //io_net();
            io_net(int io_type);
            virtual ~io_net();

			void init(){type_=type::net;}
            bool reset(){return true;};
            bool open(){return true;};
            bool close(){return true;};
            bool run_prepare(){return true;};
            bool run(){return true;};
            bool run_stop(){return true;};
            bool run_pause(){return true;};
            bool setopts(){return true;};
        protected:
        private:
			//uint32_t type_;
            //bool init(){return true;};
    };
    typedef boost::shared_ptr<io_net> io_net_ptr;
    typedef boost::function< io_net *(std::size_t io_type) > io_net_factory;
}
}
#endif // IO_NET_H
