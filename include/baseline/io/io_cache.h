#ifndef BASELINE_IO_CACHE_INTERNAL_H
#define BASELINE_IO_CACHE_INTERNAL_H

#include <baseline/io/io_base.h>

namespace io
{
    namespace cache
    {
		namespace native
		{
			using namespace io;

/*
			template<typename ObjectType>
			class cache
			{
			private:
				struct io_type_cache{};
			public:
				typedef io_type_cache c_type;
					typedef boost::shared_ptr<ObjectType> ptr_;
					typedef std::map<std::size_t, ptr_> map_;
					typedef typename map_::value_type pair_;

					cache();
					virtual ~cache();

					void init(){type_=type::cache;}
					bool add(const ptr_ &ptr)
					{
						boost::mutex::scoped_lock lock(io_mutex);
						//maps[ptr_->seq()] = obj_ptr;
					}
					bool add(ptr_ indata);
					bool del(ptr_ inpdata);
					bool clear();
					ptr_* find(std::size_t seq);
					map_ maps;


					void clear()
					{
						boost::mutex::scoped_lock lock(io_mutex);
						if(!maps.empty())
							maps.clear();
					}

				protected:
				private:
					//uint32_t type_;
					boost::mutex io_mutex;
			};
*/
		}
	}
}
#endif
