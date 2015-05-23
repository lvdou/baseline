
#include <baseline/util/util_log.h>
#include <baseline/io/io_file.h>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

namespace baseline
{
namespace io
{
    namespace file
    {
        io_file::io_file()
        {
            init();
        }

        //io_file::io_file(int io_type)
        //{
        //    init();
        //}

        io_file::~io_file()
        {
            //dtor
        }

        std::size_t io_file::size(std::string& file)
        {
            boost::mutex::scoped_lock lock(io_mutex);
            boost::filesystem::path path(file);
            return boost::filesystem::file_size(path);
        }

        bool io_file::is_modified(std::string& file, std::time_t last_read_time)
        {
            boost::mutex::scoped_lock lock(io_mutex);
            int fsize=0;
            try
            {
                boost::filesystem::path path(file);
                std::time_t now_f_wtime = boost::filesystem::last_write_time(path);
                if(now_f_wtime>last_read_time)
                    return true;

                return false;
            }
            catch(std::exception& err)
            {
                LOG_BASELINE_ERROR << "flist::read " << err.what();
                return 0;
            }
            catch(...)
            {
                LOG_BASELINE_ERROR << "flist::read read file: " << file <<  " has unknown error!";
                return 0;
            }
            return fsize;
        }

        util::share_array io_file::read(std::string& fromfile, std::size_t readlen)
        {
            boost::mutex::scoped_lock lock(io_mutex);
            try
            {
                boost::filesystem::path path(fromfile);
                std::size_t f_size;
                boost::filesystem::ifstream f_file(path, boost::filesystem::ifstream::binary|boost::filesystem::ifstream::in|boost::filesystem::ifstream::ate);
                f_size=f_file.tellg();
                if((f_size>readlen)&&(readlen!=0)) f_size=readlen;

                f_file.seekg (0, boost::filesystem::ifstream::beg);
                char *buf=new char[f_size];
                f_file.read (buf, f_size);
                f_file.close();
                return util::share_array(buf);
            }
            catch(std::exception& err)
            {
                LOG_BASELINE_ERROR << "io_file: read() " << err.what();
                //boost::shared_ptr(std::nullptr_t);
                //return util::share_array(std::nullptr_t);
            }
            catch(...)
            {
                LOG_BASELINE_ERROR << "io_file: read files '" << fromfile <<"' has unknown error!";
                //return (util::share_array)nullptr;
            }
            //return (util::share_array)nullptr;
        }

        bool io_file::save(std::string& tofile, char* buf, std::size_t buflen)
        {
            boost::mutex::scoped_lock lock(io_mutex);
            try
            {
                std::size_t f_size;
                boost::filesystem::path topath(tofile);
                boost::filesystem::ofstream to_file(topath, boost::filesystem::ofstream::binary|boost::filesystem::ofstream::out|boost::filesystem::ofstream::app);
                to_file.write(buf, buflen);
                to_file.close();

                /*
                t_string.clear();
                t_string += "chmod 666 ";
                t_string += fromfile;
                system(t_string.c_str());
                t_string.clear();
                t_string += "chgrp tomcat ";
                t_string += fromfile;
                system(t_string.c_str());
                */
            }
            catch(std::exception& err)
            {
                LOG_BASELINE_ERROR << "io_file: save() " << err.what();
                return false;
            }
            catch(...)
            {
                LOG_BASELINE_ERROR << "io_file: save() has unknown error!";
                return false;
            }
            return true;
        }

        const std::string& generate_path(std::string& file)
        {
            static boost::mutex generate_mutex;
            boost::mutex::scoped_lock lock(generate_mutex);
            boost::filesystem::path path(file);
            return path.generic_string();
        }

        bool io_file::is_exist(std::string& file)
        {
            boost::mutex::scoped_lock lock(exist_mutex);
            try
            {
                if(!&file)
                    throw 1;
                if(file.length()<=0)
                    return false;

                boost::filesystem::path path(file);
                if(!boost::filesystem::exists(path))
                {
                    //LOG_BASELINE_ERROR << "flist::isExist file: " << file <<  " not exist";
                    return false;
                }
                if(boost::filesystem::is_directory(path))
                {
                    //LOG_BASELINE_ERROR << "flist::isExist file: " << file <<  " is directory";
                    return false;
                }
                if(boost::filesystem::is_empty(path))
                {
                    //LOG_BASELINE_ERROR << "flist::isExist file: " << file <<  " is empty";
                    //return false;
                }
            }
            catch(std::exception& err)
            {
                LOG_BASELINE_ERROR << "]is_exist() " << err.what();
                return false;
            }
            catch(...)
            {
                LOG_BASELINE_ERROR << "[]is_exist() check file: '" << file <<  "' has unknown error!";
                return false;
            }
            return true;
        }
    }
}
}
