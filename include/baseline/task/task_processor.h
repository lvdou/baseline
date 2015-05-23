#ifndef BASELINE_TASK_PROCESSOR_H
#define BASELINE_TASK_PROCESSOR_H

#ifndef CLS_TASKS_PROCESSOR_H
#define CLS_TASKS_PROCESSOR_H

#include <baseline/task/task_wrapped.h>
#include <baseline/util/util.h>

namespace baseline
{
class tasks_processor: private boost::noncopyable
{
    typedef boost::asio::deadline_timer::time_type time_type;
    typedef boost::asio::deadline_timer::duration_type duration_type;

public:
    boost::asio::io_service ios_;
    boost::asio::io_service::work work_;
    tasks_processor()
        : ios_(), work_(ios_)
    { }

    template<class T>
    inline void push_task(const T& task_unwrapped)
    {
        ios_.post(detail::make_task_wrapped(task_unwrapped));
    }

    template<class Functor>
    void run_at(time_type time, const Functor& F)
    {
        detail::make_timer_task(ios_, time, F).push_task();
    }

    template<class Functor>
    void run_after(duration_type duration, const Functor& F)
    {
        detail::make_timer_task(ios_, duration, F).push_task();
    }

    void start()
    {
        ios_.run();
    }

    void stop()
    {
        ios_.stop();
    }

    void start_multiple(std::size_t threads_count=0)
    {
        if(!threads_count)
        {
            threads_count = (std::max)(static_cast<int>(boost::thread::hardware_concurrency()),1);
            //threads_count=1000;
        }
        --threads_count;
        boost::thread_group tg;
        for(std::size_t i=0; i<threads_count;++i)
        {
            tg.create_thread(boost::bind(&boost::asio::io_service::run, boost::ref(ios_)));
            boost::thread::sleep(boost::get_system_time() + boost::posix_time::microseconds(100));
        }
        ios_.run();
        tg.join_all();
    }

/*
    tcp_connection_ptr
    (const char* addr, unsigned short port_num)
    {
        return tcp_connection_ptr(
            ios_, boost::asio::ip::tcp::endpoint(
                boost::asio::ip::address_v4::from_string(addr), port_num
                )
            )
    }
    */
};

typedef boost::serialization::singleton<tasks_processor> single_tasks_processor;
}
#endif // CLS_TASKS_PROCESSOR_H

#endif // TASK_PROCESSOR_H
