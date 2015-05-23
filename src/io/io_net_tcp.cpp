
#include <baseline/util/util.h>
#include <baseline/io/io_net_tcp.h>

namespace baseline
{
namespace io
{
    namespace net_tcp
    {
        namespace libs
        {
            bool session::want_read() const
            {
                return state_ == reading;
            }

            bool session::want_write() const
            {
                return state_ == writing;
            }

            void session::do_read(boost::system::error_code& ec)
            {
                if (std::size_t len = socket_.read_some(boost::asio::buffer(data_), ec))
                {
                    write_buffer_ = boost::asio::buffer(data_, len);
                    state_ = writing;
                }
            }

            void session::do_write(boost::system::error_code& ec)
            {
                if (std::size_t len = socket_.write_some(boost::asio::buffer(write_buffer_), ec))
                {
                    write_buffer_ = write_buffer_ + len;
                    state_ = boost::asio::buffer_size(write_buffer_) > 0 ? writing : reading;
                }
            }
        }

        void io_net_tcp::handle_write(boost::system::error_code ec)
        {
            writing_ = false;
            if (!ec) session_.do_write(ec);
            if (!ec || ec == boost::asio::error::would_block)
                start_operations();
            else
                socket_.close();
        }

        void io_net_tcp::handle_read(boost::system::error_code ec)
        {
            reading_ = false;
            if (!ec) session_.do_read(ec);
            if (!ec || ec == boost::asio::error::would_block)
                start_operations();
            else
                socket_.close();
        }

        void io_net_tcp::start()
        {
            socket_.non_blocking(true); // Put the socket into non-blocking mode.
            start_operations();
        }

        void io_net_tcp::start_operations()
        {
            if (session_.want_read() && !reading_)
            {
                reading_ = true;
                socket_.async_read_some( boost::asio::null_buffers(), boost::bind(&io_net_tcp::handle_read, shared_from_this(), boost::asio::placeholders::error));
            }

            if (session_.want_write() && !writing_)
            {
              writing_ = true;
              socket_.async_write_some(boost::asio::null_buffers(), boost::bind(&io_net_tcp::handle_write, shared_from_this(), boost::asio::placeholders::error));
            }
        }

        io_net_tcp::~io_net_tcp()
        {
        }

    }
}
}
