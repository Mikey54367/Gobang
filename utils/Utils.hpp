#pragma once
#include <cstdio>
#include <iostream>
#include <ctime>
#include <mysql/mysql.h>
#include <string>
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

class mysql_util{
public:
    static MYSQL* mysql_create(const std::string& ip,const std::string& usr_name ,const std::string& password,
        const std::string& dbname,const uint16_t port)
    {

    }
    static bool mysql_exec(MYSQL* mysql,const std::string& command)
    {

    }
    static bool mysql_del(MYSQL* mysql){}
};