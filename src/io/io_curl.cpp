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
		std::string io_net_curl::m_curPage = "";//��ǰ�������ҳԴ��
		CURL* io_net_curl::m_curl = NULL;

		/************************************************************************/
		/* ��������Initialize
		/* ��  ��: ��ʼ��libcurl��
		/* ����ֵ���ɹ�������true��ʧ�ܣ�����false
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
		/* ��������WriteFunc
		/* ��  ��: libcurl����������׼�ĺ�����
		/*               size_t function( void *ptr, size_t size, size_t nmemb, void *userdata);
		/*               ���ṩ��ʽ����ҳ���ݵĻ���
		/*               curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteFunc);
		/* ����ֵ������buffer�Ĵ�С
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
		/* ��������GetPage
		/* ��  ��: ����url��ץȡ��Ӧ����ҳԴ�룻ʹ����libcurl��
		/* ����ֵ���ɹ������ذ�����ҳԴ����ַ�����ʧ�ܣ����ؿ��ַ���
		/************************************************************************/
		bool io_net_curl::getPage(
																 const std::string& urlStr, //url�ַ���
																 std::string& page             //������������ض�Ӧҳ��Դ��
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
		/* ��������Cleanup
		/* ��  ��: �����ڴ�
		/* ����ֵ����
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
