#include "db.hpp"
#include "online.hpp"
#define IP "127.0.0.1"
#define USR_NAME "dzgs"
#define DBNAME "online_gobang" 
#define PASSWORD "daango123123"

void testdb()
{
    
    //Json::Value root;
    //root["user_name"]="Kay1";
    //root["password"]="123456";
    std::string password("");
    user_table usb(IP,USR_NAME,PASSWORD,DBNAME);
    //usb.insert(root);
    //usb.login("Kay1","1234656",root);
    //std::string str;
    //json_util::serialize(root,str);
    //DLOG("%s",str.c_str());
    usb.lose(2);
}
void test_online()
{
    online_manager om;
    wsServer_t::connection_ptr con;
    om.enter_hall_room(1,con);
    if(om.is_in_hall_room(1))
    {
        DLOG("IN");
    }
    else
    {
        DLOG("NOT IN");
    }
    om.exit_hall_room(1);
    if(om.is_in_hall_room(1))
    {
        DLOG("IN");
    }
    else
    {
        DLOG("NOT IN");
    }
}
int main()
{
    //testdb();
    test_online();
    return 0;
}