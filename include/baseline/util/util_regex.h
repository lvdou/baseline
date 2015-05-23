/*******************************

    std::string str="ht tp://w ww.ba idu . com";
    std::string str2="  I was born on 5/30/1973 at 7am.   ";
    std::string str3="5/11/1973";
    LOG_BASELINE_DEBUG<<"regex str to: \""<< util::regex::remove(str,"(\\s|:)+") << "\"";
    util::regex::_match r_match =util::regex::match(str3, util::regex::pattern::regex_str_date_I);
    for(int i=0;i<r_match.size();++i)
    {
        LOG_BASELINE_DEBUG<<"regex match : "<<r_match[i];
    }
    LOG_BASELINE_DEBUG<<"regex is match : "<<util::regex::is_match(str3, util::regex::pattern::regex_str_date_I);

*******************************/


#ifndef BASELINE_UTIL_REGEX_H
#define BASELINE_UTIL_REGEX_H

#include <baseline/util/util_base.h>
#include <boost/xpressive/xpressive.hpp>

namespace baseline
{
namespace util
{
	namespace regex
	{
		typedef boost::xpressive::smatch _match;

		_match match(const std::string& src, const std::string& pattern);
		bool is_match(const std::string& src, const std::string& pattern);
		//std::string replace(std::string src, std::string find, std::string to);
		std::string replace(const std::string& src, const std::string& pattern, const std::string& change);
		std::string remove(const std::string& src, const std::string& pattern);

		static boost::mutex io_mutex;

		namespace pattern
		{
		    const std::string regex_str_character_general="^[a-zA-Z0-9_\u4e00-\u9fa5]+$";  //
		    const std::string regex_str_character_letter="^[A-Za-z]+$";  //纯字母
		    const std::string regex_str_character_number="^[0-9]+$";  //纯数字
		    const std::string regex_str_character_english="^\w+$";  //英文字符
		    const std::string regex_str_characters_chinese="^[u4e00-u9fa5],{0,}$"; //"[\u3000-\u301e\ufe10-\ufe19\ufe30-\ufe44\ufe50-\ufe6b\uff01-\uffee]";  //unicode中文字符
		    const std::string regex_str_characters_doublebyte="[^\x00-\xff]";   //双字节字符（包括中文）
		    const std::string regex_str_account="^[a-zA-Z][a-zA-Z0-9_]{4,15}$";  //匹配帐号是否合法(字母开头，允许5-16字节，允许字母数字下划线)
		    const std::string regex_str_url="[a-zA-z]+://[^\s]*";
		    const std::string regex_str_ip="((2[0-4]\d|25[0-5]|[01]?\d\d?)\.){3}(2[0-4]\d|25[0-5]|[01]?\d\d?)";
		    const std::string regex_str_email="\w+([-+.]\w+)*@\w+([-.]\w+)*\.\w+([-.]\w+)*";
		    const std::string regex_str_QQ="[1-9]\d{4,}";
		    const std::string regex_str_HTML="<(S*?)[^>]*>.*?|<.*? />"; //<(.*)(.*)>.*<\/\1>|<(.*) \/>
		    const std::string regex_str_password="(?=^.{8,}$)(?=.*\d)(?=.*\W+)(?=.*[A-Z])(?=.*[a-z])(?!.*\n).*$";
		    const std::string regex_str_date_H="(\d{4}|\d{2})-((1[0-2])|(0?[1-9]))-(([12][0-9])|(3[01])|(0?[1-9]))";  //年-月-日
		    const std::string regex_str_date_I="(\\d{1,2})([/-])(\\d{1,2})\\2((?:\\d{2}){1,2})";  //"((1[0-2])|(0?[1-9]))/(([12][0-9])|(3[01])|(0?[1-9]))/(\d{4}|\d{2})";  //年/月/日
		    const std::string regex_str_time="((1|0?)[0-9]|2[0-3]):([0-5][0-9])";   //24小时制，小时:分钟
		    const std::string regex_str_phone_chn="(\d{4}-|\d{3}-)?(\d{8}|\d{7})";  //中国固定电话号码
		    const std::string regex_str_phone_mobile_chn="1\d{10}";   //中国移动电话号码
		    const std::string regex_str_zonecode="[1-9]\d{5}";   //邮政编码
		    const std::string regex_str_identity_card_chn="\d{15}(\d\d[0-9xX])?";  //中国身份证15~18位
		    const std::string regex_num_integer_positive="^[1-9]d*|0$";   //正整数、0
		    const std::string regex_num_integer_negative="^-[1-9]d*|0$";   //负整数、0
		    const std::string regex_num_integer="^-?[1-9]\d*$";   //整数
		    const std::string regex_num_float_positive="^[1-9]d*.d*|0.d*[1-9]d*|0?.0+|0$";   //正浮点数、0
		    const std::string regex_num_float_negative="^(-([1-9]d*.d*|0.d*[1-9]d*))|0?.0+|0$";   //负浮点数、0
		    const std::string regex_num_float="^-?([1-9]d*.d*|0.d*[1-9]d*|0?.0+|0)$";   //浮点数
		    const std::string regex_trim_space_all="\\s+";  //所有的空格
		    const std::string regex_trim_tocrypt="(\\s|:)+";  //给加密库过滤字符使用
		}
	}
}
}
#endif
