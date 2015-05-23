
#include "../../include/module/module_net_tcpServer.h"

namespace module
{
    namespace net
    {
        namespace tcpServer
        {
            tcpServer::tcpServer(const std::string& address, const std::string& port, const std::string& doc_root, std::size_t io_service_pool_size)
                : io_service_pool_(io_service_pool_size), signals_(io_service_pool_.get_io_service()), acceptor_(io_service_pool_.get_io_service()), new_connection_(), request_handler_(doc_root), seq_(++t_seq), type_(type::net), uuid_ (util::uuid::by_rand())
            {
                signals_.add(SIGINT);
                signals_.add(SIGTERM);
                #if defined(SIGQUIT)
                    signals_.add(SIGQUIT);
                #endif // defined(SIGQUIT)
                signals_.async_wait(boost::bind(&tcpServer::handle_stop, this));


                boost::asio::ip::tcp::resolver resolver(acceptor_.get_io_service());
                boost::asio::ip::tcp::resolver::query query(address, port);
                boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(query);
                acceptor_.open(endpoint.protocol());
                acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true)); // Open the acceptor with the option to reuse the address (i.e. SO_REUSEADDR).
                acceptor_.bind(endpoint);
                acceptor_.listen();

                start_accept();
            }

            void tcpServer::run()
            {
                io_service_pool_.run();
            }

            void tcpServer::start_accept()
            {
                new_connection_.reset(new connection(io_service_pool_.get_io_service(), request_handler_));
                acceptor_.async_accept(new_connection_->socket(), boost::bind(&tcpServer::handle_accept, this, boost::asio::placeholders::error));
            }

            void tcpServer::handle_accept(const boost::system::error_code& e)
            {
                if (!e)
                {
                    new_connection_->start();
                }
                start_accept();
            }

            void tcpServer::handle_stop()
            {
                io_service_pool_.stop();
            }
        }
    }
}
