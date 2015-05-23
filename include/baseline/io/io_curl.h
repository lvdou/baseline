#ifndef BASELINE_IO_NET_CURL_H
#define BASELINE_IO_NET_CURL_H

#include <baseline/io/io_base.h>
#define CURL_STATICLIB
#include <curl/curl.h>

//#pragma comment(lib,"libcurl.dll.a")
//#pragma comment (lib, "ws2_32.lib")
//#pragma comment (lib, "wldap32.lib")

namespace baseline
{
namespace io
{
    namespace net_curl
    {
        using namespace io;

		class io_net_curl : public io, public boost::enable_shared_from_this<io_net_curl>
		{
			private:
				struct io_type_net_curl{};
			public:
				typedef io_type_net_curl c_type;
				typedef boost::shared_ptr<io_net_curl> io_net_curl_ptr;

				explicit io_net_curl(){init();}
				virtual ~io_net_curl(void){}

                void init();
				static bool getPage(const std::string& urlStr, std::string& page);
				static void cleanup();

			private:
				//uint32_t type_;
				static std::string m_curPage;
				static CURL *m_curl;
				static size_t writeFunc(char *data, std::size_t size, std::size_t nmemb, std::string *writerData);

		};

        typedef boost::shared_ptr<io_net_curl> io_net_curl_ptr;
        //typedef boost::function< io_net_curl *(boost::asio::io_service& io_service)> io_net_curl_factory;
		typedef boost::function< io_net_curl *() > io_net_curl_factory;
        typedef std::map<std::size_t, io_net_curl_ptr> io_net_curl_map;
        typedef std::map<std::size_t, io_net_curl_ptr>::value_type io_net_curl_map_eachpair;
	}
}
}
#endif // IO_CURL_H
