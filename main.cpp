#include <boost/algorithm/hex.hpp>

#include <baseline/common.h>
#include <baseline/io/io.h>
#include <baseline/util/util.h>
#include <baseline/crypt/crypt.h>
#include <baseline/config/config.h>
#include <baseline/module/module.h>

#include <boost/variant/multivisitors.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <baseline/task/task_processor.h>
#include <baseline/task/task_wrapped.h>

#include <boost/mpl/list.hpp>
#include <boost/statechart/event.hpp>
#include <boost/statechart/state_machine.hpp>
#include <boost/statechart/simple_state.hpp>
#include <boost/statechart/transition.hpp>
#include <boost/statechart/custom_reaction.hpp>

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

using namespace std;

int main(int argc, char* argv[])
{

    baseline::util::time::timer timer1;

    std::string hstr;
    for(int i=0;i<1000000;++i)
    {
        //util::hex::hex_to_string(hashout,hashlen);
        //char *p=util::hex::hex_to_text(hashout,hashlen);
        //hstr=p;
        //delete p;
        //LOG_BASELINE_DEBUG<<"str="<<hstr<<" get="<<util::hex::hex_to_string(hashout,hashlen);
        //crypt_md5.encode(oristring,hashstring);
        //LOG_BASELINE_DEBUG<<"MD5_3 is:"<<hashstring;
        //LOG_BASELINE_DEBUG<<"MD5_4 is:"<<crypt_md5.encode(oristring);

        //char* ptr_nativ=new char[100];
        //char* array_nativ=new char[100];
        //memcpy(array_nativ,"1234567890",10);
        char array_nativ[1500];//={"1234567890"};
        memset(array_nativ,0xFF,1500);
        baseline::util::Hex hexer;
        //hexer[0]=0x61;
        //hexer[1]=0x62;
        hexer.write(array_nativ,1500);
        hexer.write(array_nativ,1500);
        //LOG_BASELINE_DEBUG<<"[new array]: hexer="<<hexer;
        //std::string str="";
        //str.append(array_nativ,1500);
        //str.append(array_nativ,1500);
        //str+="1234567890";
/*
        char* ptr_nativ=new char[100];
        char* array_nativ=new char[100];
        memcpy(array_nativ,"1234567890",10);
        memcpy(ptr_nativ,"1234567890",10);
        boost::shared_ptr<char> s_ptr(ptr_nativ);
        baseline::util::share_array s_array(array_nativ);
        std::string ss="1234567890";
        std::string s64;
        std::string uns64;
        baseline::util::Base64 b64er;
        b64er.encode(ss,s64);
        b64er.decode(s64,uns64);
*/

        //LOG_BASELINE_DEBUG<<"[new string]: "<<*newstring("lvdou");
        //SLEEP_MICRO(1);
        //util::hex::hton<float>(vdouble1,4);
        //htonl(f);
        //newstring("lvdou");
        //LOG_BASELINE_DEBUG<<"[new array]: [0]="<<s_array[0]<<s_array[1]<<s_array[2]<<s_array[3]<<s_array[4]<<s_array[5]<<"  hash="<<baseline::util::hash(uns64)<<" s64="<<s64<<" uns64="<<uns64;
        //LOG_BASELINE_DEBUG<<"[new array]: "<<newarray("lvdou1lvdou2",sizeof("lvdou1lvdou2")).get();
//*/
    }

    LOG_BASELINE_DEBUG<<"times run : "<<timer1.elapsed_seconds()<<"."<<timer1.elapsed_micro()<<" s";

  for(;;)
  {
	SLEEP_MICRO(1000);
  }

    return 0;
}

