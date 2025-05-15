#include "db.hpp"
#define IP "127.0.0.1"
#define USR_NAME "dzgs"
#define DBNAME "online_gobang" 

void testdb()
{
    
    //Json::Value root;
    //root["user_name"]="Kay1";
    //root["password"]="123456";
    std::string password("");
    user_table usb(IP,USR_NAME,password,DBNAME);
    //usb.insert(root);
    //usb.login("Kay1","1234656",root);
    //std::string str;
    //json_util::serialize(root,str);
    //DLOG("%s",str.c_str());
    usb.lose(2);
}
int main()
{
    testdb();
    return 0;
}