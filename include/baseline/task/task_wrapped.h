#ifndef BASELINE_TASK_WRAPPED_H
#define BASELINE_TASK_WRAPPED_H

#include <iostream>
#include <boost/thread/thread.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <boost/serialization/singleton.hpp>
#include <boost/make_shared.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <memory>
//#include "../include/json.h"

namespace baseline
{
namespace detail
{

    template <class T>
    class task_wrapped
    {
    private:
        T task_unwrapped_;

    public:
        explicit task_wrapped(const T& task_unwrapped)
            : task_unwrapped_(task_unwrapped)
            {}

        void operator()() const
        {
            try
            {
                boost::this_thread::interruption_point();
            }
            catch(const boost::thread_interrupted&)
            {
            }

            try
            {
                //do task
                task_unwrapped_();
            }
            catch(const std::exception& e)
            {
                std::cerr<<"Exception: "<<e.what()<<"\n";
            }
            catch(const boost::thread_interrupted&)
            {
                std::cerr<<"Thread interrupted\n";
            }
            catch(...)
            {
                std::cerr<<"Unknown exception\n";
            }
        }
    };

    typedef boost::asio::deadline_timer::duration_type duration_type;
    template <class Functor>
    class timer_task: public task_wrapped<Functor>
    {
    private:
        typedef task_wrapped<Functor> base_t;
        std::shared_ptr<boost::asio::deadline_timer> timer_;

    public:
        template<class Time>
        explicit timer_task(boost::asio::io_service& ios, const Time& duration_or_time, const Functor& task_unwrapped)
            : base_t (task_unwrapped), timer_(boost::make_shared<boost::asio::deadline_timer>(boost::ref(ios), duration_or_time))
        {}

        void push_task()
        {
            timer_->async_wait(*this);
        }

        void operator()(const boost::system::error_code& error) const
        {
            if(!error)
            {
                base_t::operator()();
            }
            else
            {
                std::cerr << error << '\n';
            }
        }
    };

    typedef boost::asio::deadline_timer::duration_type duration_type;
    template <class Functor>
    class cmd_task: public timer_task<Functor>
    {
    private:
        typedef task_wrapped<Functor> base_t;
        std::shared_ptr<boost::asio::deadline_timer> timer_;
        //boost::shared_ptr<s_json_> param_;

    public:
        template<class Time, class Param>
        explicit cmd_task(boost::asio::io_service& ios,  const Time& duration_or_time, const Param& input_param, const Functor& task_unwrapped)
            : base_t (task_unwrapped), timer_(boost::make_shared<boost::asio::deadline_timer>(boost::ref(ios), duration_or_time))  //, param_(boost::make_shared<s_json_>(boost::ref(ios), input_param))
        {}
    };


    template <class T>
    inline task_wrapped<T> make_task_wrapped(const T& task_unwrapped)
    {
        return task_wrapped<T> (task_unwrapped);
    }

    template<class Time, class Functor>
    inline timer_task<Functor> make_timer_task(boost::asio::io_service& ios, const Time& duration_or_time, const Functor& task_unwrapped)
    {
        return timer_task<Functor> (ios, duration_or_time, task_unwrapped);
    }

    template<class Time, class Param, class Functor>
    inline cmd_task<Functor> make_cmd_task(boost::asio::io_service& ios, const Time& duration_or_time, const Param& input_param, const Functor& task_unwrapped)
    {
        return cmd_task<Functor> (ios, duration_or_time, input_param, task_unwrapped);
    }
}

}
#endif // TASK_WRAPPED_H
