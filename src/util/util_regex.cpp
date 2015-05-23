
#include <baseline/common.h>
#include <baseline/util/util_log.h>
#include <baseline/util/util_regex.h>

namespace baseline
{
namespace util
{
	namespace regex
	{
		_match match(const std::string& src, const std::string& pattern)
		{
			boost::mutex::scoped_lock lock(io_mutex);
			_match what;
			try
			{
				boost::xpressive::sregex rex = boost::xpressive::sregex::compile(pattern);
				boost::xpressive::regex_match(src, what, rex);
			}
			catch(...)
			{
			}
			return what;
		}

        bool is_match(const std::string& src, const std::string& pattern)
        {
			boost::mutex::scoped_lock lock(io_mutex);
			try
			{
			    _match what;
				boost::xpressive::sregex rex = boost::xpressive::sregex::compile(pattern);
				if(!boost::xpressive::regex_match(src, what, rex))
                {
                    return false;
                }
			}
			catch(...)
			{
			    return false;
			}
			return true;
        }

		std::string replace(const std::string& src, const std::string& pattern, const std::string& to)
		{
			boost::mutex::scoped_lock lock(io_mutex);
			std::string output;
			//output.clear();
			try
			{
			    boost::xpressive::sregex re = boost::xpressive::sregex::compile(pattern);
				output = boost::xpressive::regex_replace(src, re, to);
			}
			catch(...)
			{
			}
			return output;
		}

		std::string remove(const std::string& src, const std::string& pattern)
		{
			return replace(src, pattern, "");
		}
	}
}
}
