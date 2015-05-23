
#include <baseline/util/util_string.h>

namespace baseline
{
namespace util
{
	namespace string
	{

        std::string& toUpperCase(std::string& str)
        {
            int nChars = str.length();
            for (int i = 0; i < nChars; i++)
            {
                str[i] = toupper(str[i]);
            }
            return str;
        }

        std::string& toLowerCase(std::string& str)
        {
            int nChars = str.length();
            for (int i = 0; i < nChars; i++)
            {
                str[i] = tolower(str[i]);
            }
            return str;
        }

	}
}
}
