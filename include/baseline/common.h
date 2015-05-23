#ifndef BASELINE_COMMON_H_INCLUDED
#define BASELINE_COMMON_H_INCLUDED

//#define WIN32

#include <ctime>
#include <cstdio>
//#include <time.h>
#include <cstddef>   //For std::size_t,std::nullptr_t
#include <iostream>
#include <locale>
#include <cstdlib>
//#include <stddef.h>
//#include <stdarg.h>
//#include <stdlib.h>
//#include <memory.h>
#include <memory>
//#include <sys/types.h>
//#include <sys/timeb.h>
//#include <cassert>
#include <cerrno>
#include <emmintrin.h>
#include <stdexcept>
#include <map>
#include <set>
#include <vector>
#include <string>
#include <sstream>
//#include <strstream>
//#include <unistd.h>
#include <algorithm>
 #include <functional>
 #include <fstream>
 #include <iterator>
#include <numeric>
#include <cctype>
#include <stack>
#include <chrono>
#include <thread>

//#if defined(WIN32) || defined(__WIN32__)
    //#pragma comment (lib, "ws2_32.lib")
    //#include <winsock2.h>
    //#include <windows.h>
    //#include <ws2tcpip.h>
    //#include <sys/types.h>
    //#include <io.h>
//#endif

#include <boost/function.hpp>
#include <boost/functional/factory.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/noncopyable.hpp>
#include <boost/bind.hpp>
//#include <boost/thread.hpp>
#include <boost/shared_array.hpp>
#include <boost/array.hpp>
//#include <boost/smart_ptr.hpp>
//#include <boost/shared_ptr.hpp>
//#include <boost/weak_ptr.hpp>
#include <boost/array.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/foreach.hpp>
#include <boost/serialization/singleton.hpp>
#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>

#include <cassert>
#include <boost/assert.hpp>
#include <boost/type_traits.hpp>
#include <boost/static_assert.hpp>

/*
#if defined(WIN32) || defined(__WIN32__)
	#define SLEEP_MICRO(a)		Sleep(a)
#else
	#define SLEEP_MICRO(a)		usleep(a*1000)
#endif
*/
#define SLEEP_MICRO(a) std::this_thread::sleep_for(std::chrono::milliseconds(a));

typedef unsigned char  BYTE;
#define byte BYTE

#endif // COMMON_H_INCLUDED
