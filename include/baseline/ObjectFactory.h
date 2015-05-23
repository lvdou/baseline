#ifndef BASELINE_OBJECTFACTORY_H_INCLUDED
#define BASELINE_OBJECTFACTORY_H_INCLUDED

#include <baseline/common.h>

namespace baseline
{
/*
// 工厂模式泛型实现.
// 限制: 生成的对象必须为通过默认构造函数来构造.
// 当然你也可以扩展这个模板让它支持更多参数的构造函数.
template<typename IdType, typename ObjectType>
class ObjectFactory
{
public:
    // 表示默认构造函数的函数对象.
    //typedef boost::function< ObjectType* ((int io_type)) > factoryType;
    // 构造函数对应的函数对象的关联容器.
    //typedef std::map<IdType, factoryType> objectFactory_map;
    typedef boost::shared_ptr<ObjectType> object_ptr;
    typedef std::map<uint32_t, object_ptr> object_map;

    ObjectFactory()
    {
        seq_=0;
    }

    ~ObjectFactory()
    {
    }

    // 注册子类对象的构造函数信息.
    uint32_t registerObjectFactory(const ObjectType &creator)
    {
        seq_++;
        object_ptr obj_ptr(creator);
        objectMap_[seq_] = obj_ptr;
        return seq_;
    }
    // 通过默认构造函数在堆上创建一个新的对象实例. 使用new生成.

    factoryType * getFactory(const IdType& id)
    {
        typename objectFactory_map::const_iterator iter = objectFactoryMap_.find(id);
        if (iter == objectFactoryMap_.end())
        {
            //objectFactoryMap_[id] = creator;
            return NULL;
        }
        else
        {
            return (iter->second)();
        }
    }

    ObjectType * getObject(const IdType& id)
    {
        typename object_map::const_iterator iter = objectMap_.find(id);
        if (iter == objectMap_.end())
        {
            return NULL;
        }
        else
        {
            return (iter->second)();
        }
    }


private:
    object_map objectMap_;
    //object_map objectmap_;
    uint32_t seq_;
};


template<typename IdType, typename ObjectType>
class ObjectMap
{
public:
    // 表示默认构造函数的函数对象.
    typedef boost::shared_ptr<ObjectType> object_ptr;

    // 构造函数对应的函数对象的关联容器.
    typedef std::map<IdType, object_ptr> object_map;

    // 注册子类对象的构造函数信息.
    void instert(object_ptr)
    {
        object_map.insert(std::make_pair(sn_, object_ptr));
    }

    // 通过默认构造函数在堆上创建一个新的对象实例. 使用new生成.
    ObjectType * makeObject(const IdType& id)
    {
        typename ObjectCreator_map::const_iterator iter = objectCreatorMap_.find(id);
        if (iter == objectCreatorMap_.end())
        {
            return NULL;
        }
        else
        {
            return (iter->second)();
        }

    }

private:
    uint32_t sn_;
};
*/

/*
template<typename IdType, typename ObjectType>
class ObjectFactory
{
public:
    /// 表示默认构造函数的函数对象.
    typedef boost::function< ObjectType* () > CreatorType;

    /// 构造函数对应的函数对象的关联容器.
    typedef std::map<IdType, CreatorType> ObjectCreator_map;

    /// 注册子类对象的构造函数信息.
    void RegisterObjectCreator(const IdType& id, const CreatorType &creator)
    {
        objectCreatorMap_[id] = creator;
    }

    /// 通过默认构造函数在堆上创建一个新的对象实例. 使用new生成.
    ObjectType * MakeObject(const IdType& id)
    {

        typename ObjectCreator_map::const_iterator iter = objectCreatorMap_.find(id);
        if (iter == objectCreatorMap_.end())
        {
            return NULL;
        }
        else
        {
            return (iter->second)();
        }

    }

private:
    ObjectCreator_map objectCreatorMap_;
};

*/
}
#endif // OBJECTFACTORY_H_INCLUDED
