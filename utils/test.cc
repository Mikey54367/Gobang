#include "Utils.hpp"
#define IP "127.0.0.1"
#define USR_NAME "dzgs"
#define DBNAME "gobang" 
void testMysql()
{
    std::string password;
    std::cin>>password;
    MYSQL* mysql=mysql_util::mysql_create(IP,USR_NAME,password,DBNAME);
    if(mysql==nullptr)
        exit(-1);
    std::string sql = "insert stu values(null,'Kay',18,53,69,88)";
    if(!mysql_util::mysql_exec(mysql,sql)) exit(-1);
    mysql_util::mysql_del(mysql);
}
void testJson()
{
    Json::Value root;
    root["name"] = "Mike";
    root["age"] = 18;
    root["score"].append(90);
    root["score"].append(90.5);
    root["score"].append(88.0);
    std::string str;
    json_util::serialize(root,str);
    std::cout<<str<<std::endl;
    Json::Value val;
    json_util::deserialize(val,str);
    std::cout<<"name: "<<root["name"].asString()<<std::endl;
    std::cout<<"age: "<<root["age"].asInt()<<std::endl;
    int sz=root["score"].size();
    for(int i=0;i<sz;++i)
    {
        std::cout<<"score: "<<root["score"][i].asDouble()<<std::endl; 
    }
}
void testString()
{
    std::string str("..222.123...123....123....");
    std::vector<std::string> v;
    string_util::split(str,".",v);
    for(auto& it:v)
    {
        DLOG("%s",it.c_str());
    }
}
void testFile()
{
    std::cout<<file_util::readFile("./makefile")<<std::endl;
}
int main()
{
    testFile();
    return 0;
}