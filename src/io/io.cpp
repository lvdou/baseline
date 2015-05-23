#include "stdafx.h"
#include "../../include/io/io.h"
#include "../../include/util/util.h"

namespace io
{
    static int t_sn=0;

    io::io(int io_type)
    {
         sn_=++t_sn;
        //util_t::uuid uuid(util_t::so_util_uuid::get_mutable_instance());
        uuid_ =  util::so_util_uuid::get_mutable_instance().getUUID();
        type_=io_type;
    }

    io::io()
    {
         sn_=++t_sn;
        //util_t::uuid uuid(util_t::so_util_uuid::get_mutable_instance());
        uuid_ =  util::so_util_uuid::get_mutable_instance().getUUID();
        type_=type::net;
    }

    /*
    io::~io()
    {
        //dtor
    }
    */
}
