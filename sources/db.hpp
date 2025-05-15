#pragma once
#include "../utils/Utils.hpp"
#include <mutex>
#include <algorithm>
class user_table
{
public:
    user_table(const std::string& ip,const std::string& usr_name ,const std::string& password,
        const std::string& dbname,const uint16_t port=DEFAULT_PORT)
    {
        _mysql=mysql_util::mysql_create(ip,usr_name,password,dbname,port);
        if(_mysql==nullptr)
        {
            ELOG("mysql_create failed");
            exit(-1);
        }
    }
    ~user_table()
    {
        mysql_util::mysql_del(_mysql);
    }
    bool insert(Json::Value& root)
    {
        if(root["user_name"].isNull()||root["password"].isNull())
        {
            DLOG("Please enter user_name and password.");
            return false;
        }
        Json::Value val;
        if(find(root["user_name"].asCString(),val))
        {
            DLOG("User has already existed");
            return false;
        }
#define INSERT_SENTENCE "INSERT INTO user VALUES(null,'%s',SHA2('%s', 256),1000,0,0);"
        char sql[4096]={0};
        std::sprintf(sql,INSERT_SENTENCE,root["user_name"].asCString(),root["password"].asCString());
        //DLOG("%s",sql);
        bool ret=mysql_util::mysql_exec(_mysql,sql);
        if(!ret)
        {
            DLOG("Insertion has failed");
            return false;
        }
        return true;
    }
public:
    bool find(int id,Json::Value& root)
    {  
#define FiND_BY_ID_SENTENCE "SELECT * FROM user WHERE id=%d;"  
        char sql[4096]={0};
        std::sprintf(sql,FiND_BY_ID_SENTENCE,id);
        MYSQL_RES* result =nullptr;
        {
            std::lock_guard<std::mutex> lock(_mutex);
            if(!mysql_util::mysql_exec(_mysql,sql))
            {
                DLOG("Finding error");
                return false;
            }
            result = mysql_store_result(_mysql);
        }
        if(result==nullptr)
        {
            DLOG("Store error");
            return false;
        }
        int num_rows = mysql_num_rows(result);
        if(num_rows==0) 
        {
            DLOG("%d don't exist",id);
            return false;
        }
        if(num_rows!=1)
        {
            DLOG("Data is not uniqued");
            return false;
        }
        MYSQL_ROW row = mysql_fetch_row(result);
        root["id"]=std::stoi(row[0]);
        root["user_name"]=row[1];
        root["score"]=std::stoi(row[3]);
        root["total_count"]=std::stoi(row[4]);
        root["win_count"]=std::stoi(row[5]);
        mysql_free_result(result);
        return true;
    }
    bool find(const std::string& user_name,Json::Value& root)
    {
#define FiND_BY_NAME_SENTENCE "SELECT * FROM user WHERE user_name='%s';"  
        char sql[4096]={0};
        std::sprintf(sql,FiND_BY_NAME_SENTENCE,user_name.c_str());
        MYSQL_RES* result =nullptr;
        {
            std::lock_guard<std::mutex> lock(_mutex);
            if(!mysql_util::mysql_exec(_mysql,sql))
            {
                DLOG("Finding error");
                return false;
            }
            result = mysql_store_result(_mysql);
        }
        if(result==nullptr)
        {
            DLOG("Store error");
            return false;
        }
        int num_rows = mysql_num_rows(result);
        if(num_rows==0) 
        {
            DLOG("%s don't exist",user_name.c_str());
            return false;
        }
        if(num_rows!=1)
        {
            DLOG("Data is not uniqued");
            return false;
        }
        MYSQL_ROW row = mysql_fetch_row(result);
        root["id"]=std::stoi(row[0]);
        root["user_name"]=row[1];
        root["score"]=std::stoi(row[3]);
        root["total_count"]=std::stoi(row[4]);
        root["win_count"]=std::stoi(row[5]);
        mysql_free_result(result);
        return true;
    }
    bool login(const std::string& user_name,const std::string& password,Json::Value& root)
    {
#define LOGIN "SELECT * FROM user WHERE user_name='%s' AND password=SHA2('%s', 256);"  
        char sql[4096]={0};
        std::sprintf(sql,LOGIN,user_name.c_str(),password.c_str());
        MYSQL_RES* result =nullptr;
        {
            std::lock_guard<std::mutex> lock(_mutex);
            if(!mysql_util::mysql_exec(_mysql,sql))
            {
                DLOG("Finding error");
                return false;
            }
            result = mysql_store_result(_mysql);
        }
        if(result==nullptr)
        {
            DLOG("Store error");
            return false;
        }
        int num_rows = mysql_num_rows(result);
        if(num_rows==0){
            DLOG("User_name or password error");
            return false;
        }
        if(num_rows!=1)
        {
            DLOG("Data is not uniqued");
            return false;
        }
        MYSQL_ROW row = mysql_fetch_row(result);
        root["id"]=std::stoi(row[0]);
        root["user_name"]=row[1];
        root["score"]=std::stoi(row[3]);
        root["total_count"]=std::stoi(row[4]);
        root["win_count"]=std::stoi(row[5]);
        mysql_free_result(result);
        return true;
    }
    bool win(const int id)
    {
#define WIN "UPDATE user SET score=score+30,total_count=total_count+1,win_count=win_count+1 WHERE id=%d"
        char sql[4096]={0};
        std::sprintf(sql,WIN,id);
        if(!mysql_util::mysql_exec(_mysql,sql))
        {
            DLOG("Update failed");
            return false;
        }
        return true;
    }
    bool lose(const int id)
    {
#define LOSE "UPDATE user SET score=score-30,total_count=total_count+1 WHERE id=%d"
        char sql[4096]={0};
        std::sprintf(sql,LOSE,id);
        if(!mysql_util::mysql_exec(_mysql,sql))
        {
            DLOG("Update failed");
            return false;
        }
        return true;        
    }
private:
    std::mutex _mutex;
    MYSQL* _mysql;
};