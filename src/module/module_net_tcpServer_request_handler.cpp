
#include "../../include/module/module_net_tcpServer_request_handler.h"
#include <fstream>
#include <sstream>
#include <string>
#include <boost/lexical_cast.hpp>
#include "../../include/module/module_net_tcpServer_mime_types.h"
#include "../../include/module/module_net_tcpServer_reply.h"
#include "../../include/module/module_net_tcpServer_request.h"
#include "../../include/util/util.h"
#include "../../include/crypt/crypt.h"
#include "../../include/config/config_tcpServer.h"

namespace module
{
    namespace net
    {
        namespace tcpServer
        {
            namespace gongjian
            {
                class SkGenerator
                {
                public:
                    ByteArray* getBytes(std::string param1)
                    {
                        util::Hex cHex;
                        ByteArray *_loc_2 = cHex.toArray("a5d6a07eb3f43252d05fa597b3282eef");
                        ByteArray *_loc_3 = cHex.toArray(param1);
                        ByteArray *_loc_iv = cHex.toArray("5af5b059b0f1f243da248b9dbafc9411");
                        LOG_DEBUG<<"[2][1]";
                        std::size_t _lenkey=_loc_2->length();
                        std::size_t _leniv=_loc_iv->length();
                        crypt::symmetric::aes aes(crypt::symmetric::options::CIPHER_AES_128_CBC,crypt::symmetric::options::PADDING_DISABLE);
                        aes.load_key(_loc_2->get_native(),_lenkey,_loc_iv->get_native(),_leniv);
                        LOG_DEBUG<<"[2][4]";
                        int inlen=_loc_3->length();
                        int outlen=0;
                        unsigned char outdata[inlen*5];
                        aes.decode(_loc_3->get_native(),inlen,outdata,outlen);
                        LOG_DEBUG<<"[2][6]";
                        ByteArray *_loc_5=new ByteArray(outlen+1);
                        _loc_5->writeBytes(outdata,outlen);
                        LOG_DEBUG<<"[2][7]";
                        return _loc_5;
                    }

                    std::string getURI(std::string  param1)
                    {
                        int _loc_2 = 0;
                        std::string _loc_3;
                        _loc_3.clear();

                        _loc_2 = param1.find("?",0);
                        if(!_loc_2)
                            _loc_3 = param1;
                        else
                            _loc_3 = param1.substr(0, _loc_2);

                        _loc_2 = _loc_3.find("//",0);
                        if(!_loc_2)
                            _loc_2 = 0;
                        else
                            _loc_2 += 2;

                        _loc_2 = _loc_3.find("/",_loc_2);
                        if(!_loc_2)
                            return _loc_3;
                        else
                            _loc_2 += 2;
                            return _loc_3.substr(0, _loc_2);
                    }

                    ByteArray* Md5(std::string param1)
                    {
                        crypt::hashs::hasher _loc_2(crypt::hashs::options::CIPHER_MD5);
                        LOG_DEBUG<<"[3][1] 1";
                        util::Hex cHex;
                        ByteArray *_loc_3 = NULL;
                        _loc_3 = cHex.toArray(cHex.fromString(param1));
                        unsigned int _lenout=0;
                        unsigned char _dout[256]={0};
                        LOG_DEBUG<<"[3][1] 2";
                        _loc_2.encode(_loc_3->get_native(), _loc_3->length(), _dout, _lenout);
                        LOG_DEBUG<<"[3][1] 3";
                        _loc_3->reset();
                        _loc_3->writeBytes(_dout, _lenout);
                        return _loc_3;
                    }
                    ByteArray* encryptASE(ByteArray* param1, ByteArray*  key)
                    {
                        crypt::symmetric::aes aes(crypt::symmetric::options::CIPHER_AES_128_CBC,crypt::symmetric::options::PADDING_DISABLE);
                        LOG_DEBUG<<"[3][3] 1";
                        std::size_t _lenkey=key->length();
                        unsigned char _iv[256]={0};
                        std::size_t _leniv=0;
                        while (_leniv < 15)
                        {
                            _iv[_leniv] = 0;
                            _leniv += 1;
                        }
                        LOG_DEBUG<<"[3][3] 3";
                        aes.load_key(key->get_native(),_lenkey,_iv,_leniv);
                        LOG_DEBUG<<"[3][3] 4";
                        int inlen=param1->length();
                        unsigned char indata[inlen];
                        param1->readBytes(indata,inlen);
                        int outlen=0;
                        unsigned char outdata[inlen*3];
                        LOG_DEBUG<<"[3][3] 5";
                        aes.encode(indata,inlen,outdata,outlen);
                        ByteArray *_loc_5=new ByteArray(2048);
                        _loc_5->writeBytes(outdata,outlen);
                        LOG_DEBUG<<"[3][3] 6";
                        return _loc_5;
                    }
                    std::string genByVersion2(std::string param2, ByteArray *param3, std::string param4)
                    {
                        ByteArray *_loc_5=new ByteArray(2048);
                        _loc_5->writeByte(2);
                        _loc_5->writeByte(0);
                        _loc_5->writeByte(0);

                        int32_t _loc_6 = util::time::getclock_us()/ 1000; //int(new Date().time / 1000);
                        _loc_5->writeNumber(_loc_6);

                        LOG_DEBUG<<"[3][1]";
                        unsigned char _hash_gets[512]={0};
                        ByteArray *_loc_7 = Md5(getURI(param2) + param4);
                        unsigned char _char_7[2048]={0};
                        _loc_7->readBytes(_char_7,_loc_7->length());
                        LOG_DEBUG<<"[3][2]";
                        int _loc_8 = 0;
						while(_loc_8 < 8)
						{
							_char_7[_loc_8] = _char_7[_loc_8] ^ _char_7[_loc_8 + 8];
							_loc_5->writeByte(_char_7[_loc_8]);
							_loc_8++;
						}
						delete _loc_7;
						LOG_DEBUG<<"[3][3]";
                        _loc_5->writeByte(0);
                        ByteArray *_loc_9 = encryptASE(_loc_5, param3);    //AES加密
                        LOG_DEBUG<<"[3][4]";
                        util::Hex cHex;
                        std::string outstring=cHex.fromArray(_loc_9);
                        util::string::toUpperCase(outstring);
                        LOG_DEBUG<<"[3][5]";
                        return outstring;
                    }
                };

                std::string getPlayUrl(std::string currChannel)
                {
                    LOG_DEBUG<<"getPlayUrl";
					config::tcpServer::secret &conf_secret = config::tcpServer::so_secret::get_mutable_instance();
                    std::string _loc_3;
                    ByteArray *_loc_4=NULL;
                    //sss_loc_4=new ByteArray(2048);
                    int _loc_2= 0;   //0=autoBPS, 1=normalBPS，2=highBPS，3=superBPS  //var _loc_2:* = MLOC.handleSharedObject(MLOC.BPS) as int;
                    _loc_3 = currChannel;   //autoBPS="fmt=x264_0k_flv&sora=15"
                    _loc_3 = conf_secret.get<std::string>("streamHostArr") + _loc_3;     //MLOC.streamHostArr[0].network = （动态获取）流媒体服务的网址
                    LOG_DEBUG<<"[1][0]";
                    //计算sk
                    SkGenerator _loc_5;
                    LOG_DEBUG<<"[2][0]";
					_loc_4 = _loc_5.getBytes(conf_secret.get<std::string>("key"));    //对key做AES解码
					LOG_DEBUG<<"[3][0]";
					std::string _loc_6 = _loc_5.genByVersion2(_loc_3, _loc_4, conf_secret.get<std::string>("ip"));    //计算sk
					//_loc_6=util::string::toLowerCase(_loc_6);
                    LOG_DEBUG<<"[4][0]";
                    _loc_3 = _loc_3 + "&sk=" + _loc_6;
                    LOG_DEBUG<<"[5][0]";
                    _loc_3 += "&app=pcflash";
                    return _loc_3;
                }
            }

            request_handler::request_handler(const std::string& doc_root) : doc_root_(doc_root)
            {
            }

            void request_handler::handle_request(const request& req, reply& rep)
            {
                  // Decode url to path.
                  std::string request_path;
                  if (!url_decode(req.uri, request_path))
                  {
                        rep = reply::stock_reply(reply::bad_request);
                        return;
                  }

                  // Request path must be absolute and not contain "..".
                  if (request_path.empty() || request_path[0] != '/' || request_path.find("..") != std::string::npos)
                  {
                        rep = reply::stock_reply(reply::bad_request);
                        return;
                  }

                  // If path ends in slash (i.e. is a directory) then add "index.html".
                  if (request_path[request_path.size() - 1] == '/')
                  {
                        request_path += "index.html";
                  }

                  // Determine the file extension.
                  std::size_t last_slash_pos = request_path.find_last_of("/");
                  std::size_t last_dot_pos = request_path.find_last_of(".");
                  std::string extension;
                  if (last_dot_pos != std::string::npos && last_dot_pos > last_slash_pos)
                  {
                        extension = request_path.substr(last_dot_pos + 1);
                  }

                //process to do rules for gongjian
                request_path=util::regex::remove(request_path,"(\\s|:|/)+");
                std::cout<<"[gongjian] path: "<<request_path<<std::endl;

                //config::tcpServer::channel &conf_channel = config::tcpServer::so_channel::get_mutable_instance();
                std::string str_channel="/live/"+request_path+"?fmt=x264_700k_flv&size=720x576";
                //conf_channel.get<std::string>("/live/"+"ip")
                //if(request_path.compare("/cctv1")==0)
                std::string str_gongjian=gongjian::getPlayUrl(str_channel);
                if(str_gongjian.length()>0)
                {
                    std::cout<<"[gongjian] compared."<<std::endl;
                    rep.status = reply::moved_temporarily;
                    rep.headers.resize(1);
                    rep.headers[0].name = "Location";
					rep.headers[0].value = str_gongjian ;//"http://www.baidu.com"; //str_gongjian;
                    LOG_DEBUG<<"change to: "<< str_gongjian;

                    return;
                }
//                else
//                {
//                    std::cout<<"[gongjian] not compare."<<std::endl;
//                }


                  // Open the file to send back.
                  std::string full_path = doc_root_ + request_path;
                  std::ifstream is(full_path.c_str(), std::ios::in | std::ios::binary);
                  if (!is)
                  {
                        rep = reply::stock_reply(reply::not_found);
                        return;
                  }

                  // Fill out the reply to be sent to the client.
                  rep.status = reply::ok;
                  char buf[512];
                  while (is.read(buf, sizeof(buf)).gcount() > 0)
                        rep.content.append(buf, is.gcount());
                  rep.headers.resize(2);
                  rep.headers[0].name = "Content-Length";
                  rep.headers[0].value = boost::lexical_cast<std::string>(rep.content.size());
                  rep.headers[1].name = "Content-Type";
                  rep.headers[1].value = mime_types::extension_to_type(extension);
            }

            bool request_handler::url_decode(const std::string& in, std::string& out)
            {
                out.clear();
                out.reserve(in.size());
                for (std::size_t i = 0; i < in.size(); ++i)
                {
                    if (in[i] == '%')
                    {
                        if (i + 3 <= in.size())
                        {
                            int value = 0;
                            std::istringstream is(in.substr(i + 1, 2));
                            if (is >> std::hex >> value)
                            {
                                out += static_cast<char>(value);
                                i += 2;
                            }
                            else
                            {
                                return false;
                            }
                        }
                        else
                        {
                            return false;
                        }
                    }
                    else if (in[i] == '+')
                    {
                        out += ' ';
                    }
                    else
                    {
                        out += in[i];
                    }
                }
                return true;
            }
        }
    }
}
