#ifndef BASELINE_IO_FILE_H
#define BASELINE_IO_FILE_H

#include <baseline/io/io_base.h>

namespace baseline
{
namespace io
{
    namespace file
    {
        using namespace io;
        class io_file : public io
        {
			private:
				struct io_type_file{};
			public:
				typedef io_type_file c_type;
                explicit io_file();
                //explicit io_file(int io_type);
                virtual ~io_file();

				void init(){type_=type::file;}
                bool is_exist(std::string& file);
                bool is_modified(std::string& file, std::time_t last_read_time);
                util::share_array read(std::string& fromfile, std::size_t readlen);
                bool save(std::string& tofile, char* buf, std::size_t buflen);
                std::size_t size(std::string& file);
                const std::string& generate_path(std::string& file);

            protected:
            private:
                boost::mutex exist_mutex;
        };
        typedef boost::shared_ptr<io_file> io_file_ptr;
        typedef boost::function< io_file *()> io_file_factory; //typedef boost::function< io_file *(int io_type) > io_file_factory;
        typedef std::map<std::size_t, io_file_ptr> io_file_map;
        typedef std::map<std::size_t, io_file_ptr>::value_type io_file_map_eachpair;
    }
}
}
#endif // IO_FILE_H
