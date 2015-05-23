#ifndef BASELINE_IO_H
#define BASELINE_IO_H

#include <baseline/io/io_cache_internal.h>
#include <baseline/io/io_curl.h>
#include <baseline/io/io_file.h>
#include <baseline/io/io_net.h>
#include <baseline/io/io_net_tcp.h>

//#include "../include/ObjectFactory.h"
//#include <boost/functional/factory.hpp>
# include <boost/variant.hpp>
# include <boost/mpl/vector.hpp>
# include <boost/mpl/void.hpp>
# include <boost/mpl/remove.hpp>
# include <boost/mpl/transform.hpp>
# include <boost/mpl/size.hpp>
#include <baseline/ObjectFactory.h>

namespace baseline
{
namespace io
{

    template<typename ObjectType>
    class ioFactory
    {
    public:
        /// 表示默认构造函数的函数对象.
        typedef boost::function< ObjectType* () > CreatorType;//typedef boost::function< ObjectType* (uint32_t io_type) > CreatorType;
        /// 构造函数对应的函数对象的关联容器.
        typedef std::map<std::size_t, CreatorType> ObjectCreator_map;
        typedef boost::shared_ptr<ObjectType> Object_ptr;
        typedef std::map<std::size_t, Object_ptr> Object_map;
        typedef typename Object_map::value_type each_pair;

        explicit ioFactory(const std::size_t& io_type, const CreatorType &creator)
        {
            boost::mutex::scoped_lock lock(io_mutex);
            type_ = io_type;
            //std::cout<<"[ioFactory] : type_ = "<<type_<<std::endl;
            regist_a_factory(io_type, creator);
        }
        ~ioFactory()
        {
            clearObjectMap();
            clearObjectCreatorMap();
        }

        /// 注册子类对象的构造函数信息.
        void regist_a_factory(const std::size_t& io_type, const CreatorType &creator)
        {
            //boost::mutex::scoped_lock lock(io_mutex);
            objectCreatorMap_[io_type] = creator;
        }

        Object_ptr makeObject()
        {
            boost::mutex::scoped_lock lock(io_mutex);
            typename ObjectCreator_map::const_iterator iter = objectCreatorMap_.find(type_);
            if (iter == objectCreatorMap_.end())
            {
            }
            else
            {
                Object_ptr obj_ptr((iter->second)());
                addObjectMap(obj_ptr);
                return obj_ptr;
            }
        }

        Object_ptr findObject(const std::size_t& seq)
        {
            boost::mutex::scoped_lock lock(io_mutex);
            typename Object_map::const_iterator iter = objectMap_.find(seq);
            if (iter == objectMap_.end())
            {
            }
            else
            {
                return iter->second;
            }
        }

        void eraseObjet(const std::size_t& seq)
        {
            boost::mutex::scoped_lock lock(process_mutex);
            typename Object_map::const_iterator iter = objectMap_.find(seq);
            if (iter == objectMap_.end())
            {
            }
            else
            {
                objectMap_.erase(seq);
            }
        }

        void cleanObjets()
        {
            boost::mutex::scoped_lock lock(process_mutex);
            BOOST_FOREACH(each_pair &node, objectMap_)
            {
                if((node.second)->status_is_closed() && (node.second)->status_is_stoped() && (node.second).use_count()==1)
                {
                    //std::cout<<"clean object, seq="<<(node.second)->seq()<<" share_count="<<(node.second).use_count()<<std::endl;
                    objectMap_.erase((node.second)->seq());
                }
            }
        }

        Object_map getObjectMap()
        {
            boost::mutex::scoped_lock lock(io_mutex);
                return objectMap_;
        }

    private:
        boost::mutex io_mutex;
        boost::mutex process_mutex;
        ObjectCreator_map objectCreatorMap_;
        Object_map objectMap_;
        std::size_t type_;
        void addObjectMap(const Object_ptr &obj_ptr)
        {
            boost::mutex::scoped_lock lock(process_mutex);
            objectMap_[obj_ptr->seq()] = obj_ptr;
        }
        void clearObjectMap()
        {
                boost::mutex::scoped_lock lock(process_mutex);
                if(!objectMap_.empty())
                    objectMap_.clear();
        }
        void clearObjectCreatorMap()
        {
                boost::mutex::scoped_lock lock(process_mutex);
                if(!objectCreatorMap_.empty())
                    objectCreatorMap_.clear();
        }
    };

}
}
#endif // IO_MANAGER_H
