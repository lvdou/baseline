#include <baseline/common.h>
#include <baseline/io/io_curl.h>
#include <baseline/util/util.h>

namespace baseline
{
namespace io
{
    namespace net_curl
    {
		//static member variable define
		std::string io_net_curl::m_curPage = "";//当前保存的网页源码
		CURL* io_net_curl::m_curl = NULL;

		/************************************************************************/
		/* 函数名：Initialize
		/* 功  能: 初始化libcurl库
		/* 返回值：成功，返回true；失败，返回false
		/************************************************************************/
		void io_net_curl::init()
		{
			type_=type::net_curl;
			curl_global_init(CURL_GLOBAL_ALL);
			m_curl = curl_easy_init();
			if (m_curl)
			{
				curl_easy_setopt(m_curl, CURLOPT_FOLLOWLOCATION, 1L);
				curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, writeFunc);
				curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, &m_curPage);
			}
			else
			{
				//MessageBoxA(NULL,"GetPageByURL::Initialize Failed!", "GetPageByURL::Initialize", MB_ICONERROR);
				std::cout<<"io_net_curl::init Failed!"<<std::endl;
				return;
			}
			return;
		}

		/************************************************************************/
		/* 函数名：WriteFunc
		/* 功  能: libcurl会调用这个标准的函数，
		/*               size_t function( void *ptr, size_t size, size_t nmemb, void *userdata);
		/*               以提供格式化网页数据的机会
		/*               curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteFunc);
		/* 返回值：返回buffer的大小
		/************************************************************************/
		size_t io_net_curl::writeFunc(char *data, size_t size, size_t nmemb, std::string *writerData)
		{
			if (writerData == NULL)
				return 0;
			size_t len = size*nmemb;
			writerData->append(data, len);

			return len;
		}

		/************************************************************************/
		/* 函数名：GetPage
		/* 功  能: 根据url，抓取对应的网页源码；使用了libcurl库
		/* 返回值：成功，返回包含网页源码的字符串；失败，返回空字符串
		/************************************************************************/
		bool io_net_curl::getPage(
																 const std::string& urlStr, //url字符串
																 std::string& page             //输出参数，返回对应页面源码
																 )
		{
			//_ASSERT("" != urlStr);
			if(!m_curl)
			{
				//MessageBoxA(NULL,"You must initialize curl first!", "GetPageByURL", MB_ICONERROR);
				std::cout<<"You must init curl first!"<<std::endl;
				return false;
			}

			m_curPage.clear();
			curl_easy_setopt(m_curl, CURLOPT_URL, urlStr.c_str());
			CURLcode res = curl_easy_perform(m_curl);
			if(res != CURLE_OK)
			{
				//Utility::MessageBox(NULL, "GetPageByURL::GetPage", MB_ICONERROR, "Get the page of %s failed!", urlStr.c_str());
				std::cout<<"getPageByURL::getPage : Get the page of '"<<urlStr<<"' failed!"<<std::endl;
				return false;
			}
			else
			{
				page = m_curPage;
				return true;
			}
		}

		/************************************************************************/
		/* 函数名：Cleanup
		/* 功  能: 清理内存
		/* 返回值：无
		/************************************************************************/
		void io_net_curl::cleanup()
		{
			if(m_curl)
			{
				/* always cleanup */
				curl_easy_cleanup(m_curl);
				m_curl = NULL;
			}
		}


	}
}
}
