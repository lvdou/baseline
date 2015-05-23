#ifndef BASELINE_OBJECTBRIDGE_H_INCLUDED
#define BASELINE_OBJECTBRIDGE_H_INCLUDED

#include <baseline/common.h>

namespace baseline
{
class ObjectBridge
{
private:
    std::string version_;
    class impl
    {
    };

    boost::shared_ptr<impl> p;

public:
    ObjectBridge(): p(new impl)
    {
    }

    std::string version()
    {
        return p->version_;
    }
};
}
#endif // OBJECTBRIDGE_H_INCLUDED
