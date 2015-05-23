#ifndef BASELINE_IO_NET_TCP_H
#define BASELINE_IO_NET_TCP_H

#include <baseline/io/io_base.h>

namespace baseline
{
namespace io
{
    namespace net_tcp
    {
        using namespace io;
        namespace libs
        {
            class session
            {
            public:
              session(boost::asio::ip::tcp::socket& socket) : socket_(socket), state_(reading){}

              bool want_read() const;  // Returns true if the third party library wants to be notified when the socket is ready for reading.
              void do_read(boost::system::error_code& ec); // Notify that third party library that it should perform its read operation.
              bool want_write() const;   // Returns true if the third party library wants to be notified when the socket is ready for writing.
              void do_write(boost::system::error_code& ec);    // Notify that third party library that it should perform its write operation.

            private:
              boost::asio::ip::tcp::socket& socket_;
              enum { reading, writing } state_;
              boost::array<char, 128> data_;
              boost::asio::const_buffer write_buffer_;
            };
        }

        class io_net_tcp : public io, public boost::enable_shared_from_this<io_net_tcp>
        {
			private:
				struct io_type_net_tcp{};
			public:
				typedef io_type_net_tcp c_type;
                typedef std::shared_ptr<io_net_tcp> io_net_tcp_ptr;

				explicit io_net_tcp(boost::asio::io_service& io_service) : socket_(io_service), session_(socket_), reading_(false), writing_(false) {init();}
                virtual ~io_net_tcp();

				void init(){type_=type::net_tcp;}
                //bool status_is_running(){return is_running_;};
                //bool status_is_prepared(){return is_prepared_;};
                //bool status_is_stoped(){return is_stoped_;};
                //bool status_is_closed(){return is_closed_;};

                void start();
                static io_net_tcp_ptr create(boost::asio::io_service& io_service){return io_net_tcp_ptr(new io_net_tcp(io_service));}
                boost::asio::ip::tcp::socket& socket(){return socket_;}

            protected:
            private:
				std::size_t type_;
                boost::asio::ip::tcp::socket socket_;
                libs::session session_;
                bool reading_;
                bool writing_;
                void start_operations();
                void handle_read(boost::system::error_code ec);
                void handle_write(boost::system::error_code ec);

        };

        typedef boost::shared_ptr<io_net_tcp> io_net_tcp_ptr;
        typedef boost::function< io_net_tcp *(boost::asio::io_service& io_service)> io_net_tcp_factory; //typedef boost::function< io_net_tcp *(int io_type) > io_net_tcp_factory;
        typedef std::map<std::size_t, io_net_tcp_ptr> io_net_tcp_map;
        typedef std::map<std::size_t, io_net_tcp_ptr>::value_type io_net_tcp_map_eachpair;
    }
}
}
#endif // IO_NET_H
