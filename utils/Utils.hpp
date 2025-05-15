#pragma once
#include <cstdio>
#include <iostream>
#include <ctime>
#include <mysql/mysql.h>
#include <string>
#include <vector>
#include <jsoncpp/json/json.h>
#include <sstream>
#include <memory>
#include <fstream>
#include <stdexcept>
#include<websocketpp/config/asio_no_tls.hpp>
#include<websocketpp/server.hpp>
#include<unordered_map>
typedef websocketpp::server<websocketpp::config::asio> wsServer_t;
#include <mutex>
#define IFO 0
#define DEB 1
#define ERR 2
#define DEFAULT_LOG_LEVEL IFO
#define LOG(level,format,...) do{\
    if(level<DEFAULT_LOG_LEVEL) break;\
    std::time_t currentTime = std::time(nullptr);\
    std::tm* localTime = std::localtime(&currentTime);\
    std::fprintf(stdout,"[%02d:%02d:%02d %s %d] " format "\n",localTime->tm_hour, localTime->tm_min, localTime->tm_sec,\
        __FILE__,__LINE__,##__VA_ARGS__);\
    }while(0)
#define ILOG(format,...) LOG(IFO,format,##__VA_ARGS__)
#define DLOG(format,...) LOG(DEB,format,##__VA_ARGS__)
#define ELOG(format,...) LOG(ERR,format,##__VA_ARGS__)
#define DEFAULT_PORT 0
class mysql_util{
public:
    static MYSQL* mysql_create(const std::string& ip,const std::string& usr_name ,const std::string& password,
        const std::string& dbname,const uint16_t port=DEFAULT_PORT)
    {
        //1.初始化mysql句柄
        MYSQL* mysql=mysql_init(NULL);
        if(mysql==NULL)
        {
            ELOG("init failed\n");
            return nullptr;
        }
        //2.连接服务器
        if(mysql_real_connect(mysql,ip.c_str(),usr_name.c_str(),password.c_str(),dbname.c_str(),port,NULL,0)==NULL)
        {
            ELOG("connect failed: %s",mysql_error(mysql));
            mysql_close(mysql);
            return nullptr;
        }
        //3.设置客户端字符集
        if(mysql_set_character_set(mysql,"utf8")!=0)
        {
            ELOG("set character failed: %s",mysql_error(mysql));
            mysql_close(mysql);
            return nullptr;
        }
        return mysql;
    }
    static bool mysql_exec(MYSQL* mysql,const std::string& command)
    {
        if(mysql==nullptr)
        {
            ELOG("Mysql is nullptr");
        }
        int ret=mysql_query(mysql,command.c_str());
        if(ret!=0)
        {
            ELOG("%s",command.c_str());
            ELOG("mysql query failed: %s",mysql_error(mysql));
            //mysql_close(mysql);
            return false;
        }
        return true;
    }
    static void mysql_del(MYSQL* mysql)
    {
        mysql_close(mysql);
    }
};

class json_util
{
public:
    static bool serialize(const Json::Value& root,std::string& str)
    {
        Json::StreamWriterBuilder swb;
        std::unique_ptr<Json::StreamWriter> pw(swb.newStreamWriter());
        std::stringstream ss;
        int ret=pw->write(root,&ss);
        if(ret!=0)
        {
            ELOG("serialize failed");
            return false;
        }
        str=ss.str();
        return true;
    }
    static bool deserialize(Json::Value& root,const std::string& str)
    {
        Json::CharReaderBuilder crb;
        std::unique_ptr<Json::CharReader> crp(crb.newCharReader());
        std::string err;
        bool ret=crp->parse(str.c_str(),str.c_str()+str.size(),&root,&err);
        if(ret==false)
        {
            //std::cout<<"Deserialze failed: "<<err<<std::endl;
            ELOG("Deserialze failed: %s",err.c_str());
            return false;
        }
        return true;
    }
};
class string_util
{
public:
    static int split(const std::string& src,const std::string& sep,std::vector<std::string>& res)
    {
        int index=0,pos=0;
        while(index<src.size())
        {
            pos=src.find(sep,index);
            if(pos==src.npos)
            {
                res.emplace_back(src.substr(index));
                break;
            }
            if(pos==index)
            {
                index+=sep.size();
                continue;
            }
            res.emplace_back(src.substr(index,pos-index));
            index=pos+sep.size();
        }
        return res.size();
    }
};
class file_util
{
public:
    static std::string readFile(const std::string& filePath) {
        std::ifstream file(filePath, std::ios::binary);
        
        if (!file.is_open()) {
            throw std::runtime_error("无法打开文件: " + filePath);
        }
        
        // 使用RAII确保文件流自动关闭
        try {
            // 获取文件大小
            file.seekg(0, std::ios::end);
            size_t size = file.tellg();
            
            // 分配足够内存并读取内容
            std::string content(size, '\0');
            file.seekg(0);
            file.read(&content[0], size);
            
            // 检查读取是否成功
            if (!file) {
                throw std::runtime_error("读取文件失败: " + filePath);
            }
            
            return content;
        } catch (...) {
            // 确保异常发生时文件流被关闭
            file.close();
            throw;
        }
    }
};